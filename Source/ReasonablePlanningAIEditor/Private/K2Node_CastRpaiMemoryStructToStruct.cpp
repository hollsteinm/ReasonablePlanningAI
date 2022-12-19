// Copyright Epic Games, Inc. All Rights Reserved.


#include "K2Node_CastRpaiMemoryStructToStruct.h"

#include "Core/RpaiTypes.h"
#include "RpaiBPLibrary.h"
#include "BlueprintActionFilter.h"
#include "BlueprintFieldNodeSpawner.h"
#include "BlueprintNodeBinder.h"
#include "BlueprintNodeSpawner.h"
#include "KismetCompiler.h"
#include "EdGraphUtilities.h"
#include "K2Node_CallFunction.h"
#include "K2Node_IfThenElse.h"

FName UK2Node_CastRpaiMemoryStructToStruct::MemoryStructInputPin = FName(TEXT("MemoryStruct"));

void UK2Node_CastRpaiMemoryStructToStruct::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	bool bIsDirty = false;
	FName PropertyName = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UK2Node_CastRpaiMemoryStructToStruct, StructType))
	{
		bIsDirty = true;
	}

	if (bIsDirty)
	{
		ReconstructNode();
		GetGraph()->NotifyGraphChanged();
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void UK2Node_CastRpaiMemoryStructToStruct::PreloadRequiredAssets()
{
	Super::PreloadRequiredAssets();
	PreloadObject(StructType);
}

UEdGraphPin* UK2Node_CastRpaiMemoryStructToStruct::GetMemoryStructInputPin() const
{
	return FindPin(MemoryStructInputPin, EGPD_Input);
}

UEdGraphPin* UK2Node_CastRpaiMemoryStructToStruct::GetThenOutputPin() const
{
	return FindPin(UEdGraphSchema_K2::PN_Then, EGPD_Output);
}

UEdGraphPin* UK2Node_CastRpaiMemoryStructToStruct::GetInvalidOutputPin() const
{
	return FindPin(UEdGraphSchema_K2::PN_Else, EGPD_Output);
}

FText UK2Node_CastRpaiMemoryStructToStruct::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (StructType == nullptr)
	{
		return NSLOCTEXT("Rpai", "ReadMemoryNullStructTitle", "Read Memory as <unknown struct>");
	}
	else if (CachedTitle.IsOutOfDate(this))
	{
		FFormatNamedArguments Args;
		Args.Add(TEXT("StructName"), FText::FromName(StructType->GetFName()));
		CachedTitle.SetCachedText(FText::Format(NSLOCTEXT("Rpai", "ReadMemoryAsStruct", "Read Memory As {StructName}"), Args), this);
	}
	return CachedTitle;
}

void UK2Node_CastRpaiMemoryStructToStruct::AllocateDefaultPins()
{
	if (!bRegisterNets)
	{
		CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
	}
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, FRpaiMemoryStruct::StaticStruct(), MemoryStructInputPin);
	if (StructType != nullptr)
	{
		if (!bRegisterNets)
		{
			auto Then = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);
			Then->PinFriendlyName = NSLOCTEXT("Rpai", "ThenPin", "Valid");
			auto Else = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Else);
			Else->PinFriendlyName = NSLOCTEXT("Rpai", "InvalidPin", "Invalid");
		}
		CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Struct, StructType, UEdGraphSchema_K2::PN_ReturnValue);
	}
}

void UK2Node_CastRpaiMemoryStructToStruct::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	Super::ExpandNode(CompilerContext, SourceGraph);
	auto InputPin = GetMemoryStructInputPin();
	if(StructType && InputPin != nullptr && !bRegisterNets)
	{
		const UEdGraphSchema_K2* Schema = CompilerContext.GetSchema();

		const FName ValidateFunctionName = GET_MEMBER_NAME_CHECKED(URpaiBPLibrary, IsSafeToReadAs);
		const UFunction* Function = URpaiBPLibrary::StaticClass()->FindFunctionByName(ValidateFunctionName);
		check(NULL != Function);

		UK2Node_CallFunction* CallValidation = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
		CallValidation->SetFromFunction(Function);
		CallValidation->AllocateDefaultPins();

		UEdGraphPin* IsSafeFunctionStructTypeInputPin = CallValidation->FindPinChecked(TEXT("StructType"));
		Schema->TrySetDefaultObject(*IsSafeFunctionStructTypeInputPin, StructType);
		check(IsSafeFunctionStructTypeInputPin->DefaultObject == StructType);

		UEdGraphPin* IsSafeFunctionMemoryInputPin = CallValidation->FindPinChecked(TEXT("Memory"));
		CompilerContext.MovePinLinksToIntermediate(*InputPin, *IsSafeFunctionMemoryInputPin);

		UEdGraphPin* IsSafeFunctionBoolOutputPin = CallValidation->FindPinChecked(UEdGraphSchema_K2::PN_ReturnValue);

		UK2Node_IfThenElse* IfThenElseNode = CompilerContext.SpawnIntermediateNode<UK2Node_IfThenElse>(this, SourceGraph);
		IfThenElseNode->AllocateDefaultPins();

		bool bConnected = Schema->TryCreateConnection(IsSafeFunctionBoolOutputPin, IfThenElseNode->GetConditionPin());
		check(bConnected);

		CompilerContext.MovePinLinksToIntermediate(*GetThenOutputPin(), *IfThenElseNode->GetThenPin());
		CompilerContext.MovePinLinksToIntermediate(*GetInvalidOutputPin(), *IfThenElseNode->GetElsePin());
		CompilerContext.MovePinLinksToIntermediate(*GetExecPin(), *IfThenElseNode->GetExecPin());


		UK2Node_CastRpaiMemoryStructToStruct* ReadCastNode = CompilerContext.SpawnIntermediateNode<UK2Node_CastRpaiMemoryStructToStruct>(this, SourceGraph);
		ReadCastNode->StructType = StructType;
		ReadCastNode->bRegisterNets = true;
		ReadCastNode->AllocateDefaultPins();

		UEdGraphPin* CastInputPin = ReadCastNode->GetMemoryStructInputPin();
		CompilerContext.MovePinLinksToIntermediate(*InputPin, *CastInputPin);

		UEdGraphPin* OrgReturnPin = FindPinChecked(UEdGraphSchema_K2::PN_ReturnValue);
		UEdGraphPin* NewReturnPin = ReadCastNode->FindPinChecked(UEdGraphSchema_K2::PN_ReturnValue);
		CompilerContext.MovePinLinksToIntermediate(*OrgReturnPin, *NewReturnPin);

		BreakAllNodeLinks();
	}
}

void UK2Node_CastRpaiMemoryStructToStruct::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	struct GetMenuActions_Utils
	{
		static void SetNodeStructType(UEdGraphNode* NewNode, FFieldVariant /*StructField*/, TWeakObjectPtr<UScriptStruct> NonConstStructPtr)
		{
			UK2Node_CastRpaiMemoryStructToStruct* StructNode = CastChecked<UK2Node_CastRpaiMemoryStructToStruct>(NewNode);
			StructNode->StructType = NonConstStructPtr.Get();
			StructNode->bRegisterNets = false;
		}

		static void OverrideCategory(FBlueprintActionContext const& Context, IBlueprintNodeBinder::FBindingSet const& /*Bindings*/, FBlueprintActionUiSpec* UiSpecOut, TWeakObjectPtr<UScriptStruct> StructPtr)
		{
			for (UEdGraphPin* Pin : Context.Pins)
			{
				UScriptStruct* PinStruct = Cast<UScriptStruct>(Pin->PinType.PinSubCategoryObject.Get());
				if ((PinStruct != nullptr) && (StructPtr.Get() == PinStruct) && (Pin->Direction == EGPD_Output))
				{
					UiSpecOut->Category = NSLOCTEXT("Rpai", "EmptyFunctionCategory", "|");
					break;
				}
			}
		}
	};

	UClass* NodeClass = GetClass();
	ActionRegistrar.RegisterStructActions(FBlueprintActionDatabaseRegistrar::FMakeStructSpawnerDelegate::CreateLambda([NodeClass](const UScriptStruct* Struct) -> UBlueprintNodeSpawner*
		{
			UBlueprintFieldNodeSpawner* NodeSpawner = nullptr;
			if (Struct && UEdGraphSchema_K2::IsAllowableBlueprintVariableType(Struct, false))
			{
				NodeSpawner = UBlueprintFieldNodeSpawner::Create(NodeClass, const_cast<UScriptStruct*>(Struct));
				check(NodeSpawner != nullptr);
				TWeakObjectPtr<UScriptStruct> NonConstStructPtr = MakeWeakObjectPtr(const_cast<UScriptStruct*>(Struct));
				NodeSpawner->SetNodeFieldDelegate = UBlueprintFieldNodeSpawner::FSetNodeFieldDelegate::CreateStatic(GetMenuActions_Utils::SetNodeStructType, NonConstStructPtr);
				NodeSpawner->DynamicUiSignatureGetter = UBlueprintFieldNodeSpawner::FUiSpecOverrideDelegate::CreateStatic(GetMenuActions_Utils::OverrideCategory, NonConstStructPtr);
			}
			return NodeSpawner;
		}));
}

class FKCHandler_CastMemoryStructToStruct : public FNodeHandlingFunctor
{
public:
	FKCHandler_CastMemoryStructToStruct(FKismetCompilerContext& InCompilerContext)
		: FNodeHandlingFunctor(InCompilerContext)
	{
	}

	virtual void RegisterNets(FKismetFunctionContext& Context, UEdGraphNode* Node) override
	{
		FNodeHandlingFunctor::RegisterNets(Context, Node); //handle literals

		auto MyNode = CastChecked<UK2Node_CastRpaiMemoryStructToStruct>(Node);

		auto InPin = MyNode->GetMemoryStructInputPin();
		auto Net = FEdGraphUtilities::GetNetFromPin(InPin);
		FBPTerminal* SourceTerm = nullptr;
		if (Context.NetMap.Find(Net) == nullptr)
		{
			SourceTerm = Context.CreateLocalTerminalFromPinAutoChooseScope(Net, Context.NetNameMap->MakeValidName(Net));
			Context.NetMap.Add(Net, SourceTerm);
		}
		check(SourceTerm != nullptr);

		UEdGraphPin* OutPin = Node->FindPinChecked(UEdGraphSchema_K2::PN_ReturnValue, EGPD_Output);
		Net = FEdGraphUtilities::GetNetFromPin(OutPin);
		if (ensure(Context.NetMap.Find(OutPin) == nullptr))
		{
			FBPTerminal* Term = Context.CreateLocalTerminalFromPinAutoChooseScope(OutPin, Context.NetNameMap->MakeValidName(Net));
			Context.NetMap.Add(OutPin, Term);
		}
	}
};

FNodeHandlingFunctor* UK2Node_CastRpaiMemoryStructToStruct::CreateNodeHandler(FKismetCompilerContext& CompilerContext) const
{
	return bRegisterNets ? new FKCHandler_CastMemoryStructToStruct(CompilerContext) : new FNodeHandlingFunctor(CompilerContext);
}