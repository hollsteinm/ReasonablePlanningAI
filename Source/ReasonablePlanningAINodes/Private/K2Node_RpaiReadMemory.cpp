// Copyright Epic Games, Inc. All Rights Reserved.


#include "K2Node_RpaiReadMemory.h"

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

FName UK2Node_RpaiReadMemory::MemoryStructInputPin = FName(TEXT("MemoryStruct"));

void UK2Node_RpaiReadMemory::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	bool bIsDirty = false;
	FName PropertyName = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UK2Node_RpaiReadMemory, StructType))
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

void UK2Node_RpaiReadMemory::PreloadRequiredAssets()
{
	Super::PreloadRequiredAssets();
	PreloadObject(StructType);
}

UEdGraphPin* UK2Node_RpaiReadMemory::GetMemoryStructInputPin() const
{
	return FindPin(MemoryStructInputPin, EGPD_Input);
}

UEdGraphPin* UK2Node_RpaiReadMemory::GetThenOutputPin() const
{
	return FindPin(UEdGraphSchema_K2::PN_Then, EGPD_Output);
}

UEdGraphPin* UK2Node_RpaiReadMemory::GetInvalidOutputPin() const
{
	return FindPin(UEdGraphSchema_K2::PN_Else, EGPD_Output);
}

FText UK2Node_RpaiReadMemory::GetNodeTitle(ENodeTitleType::Type TitleType) const
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

void UK2Node_RpaiReadMemory::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, FRpaiMemoryStruct::StaticStruct(), MemoryStructInputPin);
	if (StructType != nullptr)
	{
		auto Then = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);
		Then->PinFriendlyName = NSLOCTEXT("Rpai", "ThenPin", "Valid");
		auto Else = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Else);
		Else->PinFriendlyName = NSLOCTEXT("Rpai", "InvalidPin", "Invalid");
		CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Struct, StructType, UEdGraphSchema_K2::PN_ReturnValue);
	}
}

void UK2Node_RpaiReadMemory::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	Super::ExpandNode(CompilerContext, SourceGraph);
	auto InputPin = GetMemoryStructInputPin();
	if(StructType && InputPin != nullptr)
	{
		const UEdGraphSchema_K2* Schema = CompilerContext.GetSchema();

		const FName ReadMemoryFunctionName = GET_FUNCTION_NAME_CHECKED(URpaiBPLibrary, ReadMemory);
		const UFunction* ReadMemoryFunction = URpaiBPLibrary::StaticClass()->FindFunctionByName(ReadMemoryFunctionName);
		check(NULL != ReadMemoryFunction);

		UK2Node_CallFunction* ReadMemory = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
		ReadMemory->SetFromFunction(ReadMemoryFunction);
		ReadMemory->AllocateDefaultPins();

		UEdGraphPin* OriginalOutStructPin = FindPinChecked(UEdGraphSchema_K2::PN_ReturnValue, EGPD_Output);
		UEdGraphPin* ReadMemoryOutParamStruct = ReadMemory->FindPinChecked(TEXT("OutStruct"));
		ReadMemoryOutParamStruct->PinType = OriginalOutStructPin->PinType;
		ReadMemoryOutParamStruct->PinType.PinSubCategoryObject = OriginalOutStructPin->PinType.PinSubCategoryObject;
		CompilerContext.MovePinLinksToIntermediate(*OriginalOutStructPin, *ReadMemoryOutParamStruct);

		UEdGraphPin* ReadMemoryMemoryInputPin = ReadMemory->FindPinChecked(TEXT("Memory"));
		CompilerContext.MovePinLinksToIntermediate(*InputPin, *ReadMemoryMemoryInputPin);

		CompilerContext.MovePinLinksToIntermediate(*GetExecPin(), *ReadMemory->GetExecPin());

		UK2Node_IfThenElse* IfThenElse = CompilerContext.SpawnIntermediateNode<UK2Node_IfThenElse>(this, SourceGraph);
		IfThenElse->AllocateDefaultPins();

		Schema->TryCreateConnection(ReadMemory->GetReturnValuePin(), IfThenElse->GetConditionPin());
		Schema->TryCreateConnection(ReadMemory->GetThenPin(), IfThenElse->GetExecPin());

		CompilerContext.MovePinLinksToIntermediate(*GetThenOutputPin(), *IfThenElse->GetThenPin());
		CompilerContext.MovePinLinksToIntermediate(*GetInvalidOutputPin(), *IfThenElse->GetElsePin());

		BreakAllNodeLinks();
	}
}

void UK2Node_RpaiReadMemory::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	struct GetMenuActions_Utils
	{
		static void SetNodeStructType(UEdGraphNode* NewNode, FFieldVariant /*StructField*/, TWeakObjectPtr<UScriptStruct> NonConstStructPtr)
		{
			UK2Node_RpaiReadMemory* StructNode = CastChecked<UK2Node_RpaiReadMemory>(NewNode);
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
