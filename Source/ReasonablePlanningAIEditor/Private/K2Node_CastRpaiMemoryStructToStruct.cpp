// Copyright Epic Games, Inc. All Rights Reserved.


#include "K2Node_CastRpaiMemoryStructToStruct.h"

#include "Core/RpaiTypes.h"
#include "BlueprintActionFilter.h"
#include "BlueprintFieldNodeSpawner.h"
#include "BlueprintNodeBinder.h"
#include "BlueprintNodeSpawner.h"
#include "KismetCompiler.h"
#include "EdGraphUtilities.h"

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
	return FindPin(*MemoryStructInputPin.ToString(), EGPD_Input);
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
	FCreatePinParams InputParams;
	InputParams.bIsReference = true;
	InputParams.bIsConst = true;

	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, FRpaiMemoryStruct::StaticStruct(), *MemoryStructInputPin.ToString(), InputParams);
	if (StructType != nullptr)
	{
		CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Struct, StructType, UEdGraphSchema_K2::PN_ReturnValue);
	}
}

void UK2Node_CastRpaiMemoryStructToStruct::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	Super::ExpandNode(CompilerContext, SourceGraph);
	auto InputPin = GetMemoryStructInputPin();
	if (InputPin != nullptr)
	{

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
		if (Context.NetMap.Find(Net) == nullptr)
		{
			FBPTerminal* Term = Context.CreateLocalTerminalFromPinAutoChooseScope(Net, Context.NetNameMap->MakeValidName(Net));
			Context.NetMap.Add(Net, Term);
		}

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
	return new FKCHandler_CastMemoryStructToStruct(CompilerContext);
}