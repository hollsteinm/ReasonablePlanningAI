// Copyright Epic Games, Inc. All Rights Reserved.


#include "K2Node_CastRpaiMemoryStructToStruct.h"

#include "Core/RpaiTypes.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintFieldNodeSpawner.h"
#include "Containers/Map.h"
#include "Delegates/Delegate.h"
#include "EdGraph/EdGraphPin.h"
#include "EdGraphSchema_K2.h"
#include "EdGraphUtilities.h"
#include "EditorCategoryUtils.h"
#include "HAL/PlatformMath.h"
#include "Internationalization/Internationalization.h"
#include "K2Node_CallFunction.h"
#include "Kismet/KismetNodeHelperLibrary.h"
#include "Kismet2/CompilerResultsLog.h"
#include "KismetCompiledFunctionContext.h"
#include "KismetCompiler.h"
#include "KismetCompilerMisc.h"
#include "Misc/AssertionMacros.h"
#include "Styling/AppStyle.h"
#include "Templates/Casts.h"
#include "UObject/Field.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

class FString;
class UBlueprintNodeSpawner;
struct FBPTerminal;
struct FLinearColor;

const FName UK2Node_CastRpaiMemoryStructToStruct::ConvertToStructInputPinName(TEXT("MemoryAsStruct"));
const FName UK2Node_CastRpaiMemoryStructToStruct::ConvertToStructSuccessPinName(TEXT("bSuccess"));

UK2Node_CastRpaiMemoryStructToStruct::UK2Node_CastRpaiMemoryStructToStruct(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, TargetStructType(nullptr)
{
}

void UK2Node_CastRpaiMemoryStructToStruct::ValidateNodeDuringCompilation(class FCompilerResultsLog& MessageLog) const
{
	Super::ValidateNodeDuringCompilation(MessageLog);
}

void UK2Node_CastRpaiMemoryStructToStruct::AllocateDefaultPins()
{
	Super::AllocateDefaultPins();
	const bool StructHasNewerVersion = TargetStructType && TargetStructType->HasAnyFlags(EObjectFlags::RF_NewerVersionExists);
	if (StructHasNewerVersion)
	{
		Message_Error(FString::Printf(TEXT("Node '%s' references obsolete struct '%s'", *GetPathName(), *TargetStructType->GetPathName())));
	}
	ensure(!StructHasNewerVersion);

	const UEdGraphSchema_K2* K2Schema = Cast<UEdGraphSchema_K2>(GetSchema());
	check(K2Schema != nullptr);

	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_CastSucceeded);
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_CastFailed);

	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Wildcard, UScriptStruct::StaticClass(), ConvertToStructInputPinName);

	if (TargetStructType)
	{
		//TODO: Display Property Pins
	}

	UEdGraphPin* BoolSuccessPin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Boolean, ConvertToStructSuccessPinName);
}

FText UK2Node_CastRpaiMemoryStructToStruct::GetTooltipText() const
{
	return NSLOCTEXT("RpaiEditor", "CastRpaiMemoryStructToStruct_Tooltip", "Cast RpaiMemoryStruct to Struct");
}

FText UK2Node_CastRpaiMemoryStructToStruct::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (!TargetStructType)
	{
		return NSLOCTEXT("RpaiEditor", "CastRpaiMemoryStructToStruct_BadTitle", "Bad convert node");
	}
	else if (CachedNodeTitle.IsOutOfDate(this))
	{
		FFormatNamedArguments Args;
		Args.Add(TEXT("TargetName"), FText::FromString(TargetStructType->GetName()));

		CachedNodeTitle.SetCachedText(FText::Format(NSLOCTEXT("RpaiEditor", "CastRpaiMemoryStructToStruct_ConvertTo", "Convert To {TargetName}"), Args), this);
	}
	return CachedNodeTitle;
}

FSlateIcon UK2Node_CastRpaiMemoryStructToStruct::GetIconAndTint(FLinearColor& OutColor) const
{
	static FSlateIcon Icon(FAppStyle::GetAppStyleSetName(), "GraphEditor.Struct_16x");
	return Icon;
}

void UK2Node_CastRpaiMemoryStructToStruct::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	Super::GetMenuActions(ActionRegistrar);
	UClass* Action = GetClass();
	if (ActionRegistrar.IsOpenForRegistration(Action))
	{
		UBlueprintNodeSpawner* Spawner = UBlueprintNodeSpawner::Create(GetClass());
		check(Spawner != nullptr);
		ActionRegistrar.AddBlueprintAction(Action, Spawner);
	}
}

FText UK2Node_CastRpaiMemoryStructToStruct::GetMenuCategory() const
{
	return FEditorCategoryUtils::GetCommonCategory(FCommonEditorCategory::AI);
}

UEdGraphPin* UK2Node_CastRpaiMemoryStructToStruct::GetValidCastPin() const
{
	return FindPin(UEdGraphSchema_K2::PN_CastSucceeded, EGPD_Output);
}

UEdGraphPin* UK2Node_CastRpaiMemoryStructToStruct::GetInvalidCastPin() const
{
	return FindPin(UEdGraphSchema_K2::PN_CastFailed, EGPD_Output);
}

UEdGraphPin* UK2Node_CastRpaiMemoryStructToStruct::GetCastResultPin() const
{
	if (TargetStructType)
	{
		auto Predicate = [this](const UEdGraphPin* Pin) -> bool
		{
			return Pin->PinType.PinSubCategoryObject == TargetStructType &&
				Pin->Direction == EGPD_Output &&
				Pin->PinName.ToString().StartsWith(UEdGraphSchema_K2::PN_CastedValuePrefix);
		};
		return *Pins.FindByPredicate(Predicate);
	}
	return nullptr;
}

UEdGraphPin* UK2Node_CastRpaiMemoryStructToStruct::GetCastSourcePin() const
{
	return FindPin(ConvertToStructInputPinName, EGPD_Input);
}

UEdGraphPin* UK2Node_CastRpaiMemoryStructToStruct::GetBoolSuccessPin() const
{
	return FindPin(ConvertToStructSuccessPinName, EGPD_Output);
}

void UK2Node_CastRpaiMemoryStructToStruct::PreSave(FObjectPreSaveContext SaveContext)
{

}

void UK2Node_CastRpaiMemoryStructToStruct::Serialize(FArchive& Ar)
{

}

void UK2Node_CastRpaiMemoryStructToStruct::PreloadRequiredAssets()
{
	PreloadObject(TargetStructType);
	Super::PreloadRequiredAssets();
}

void UK2Node_CastRpaiMemoryStructToStruct::PostPlacedNewNode()
{

}

FString UK2Node_CastRpaiMemoryStructToStruct::GetPinMetaData(FName InPinName, FName InKey)
{

}

bool UK2Node_CastRpaiMemoryStructToStruct::HasExternalDependencies(TArray<class UStruct*>* OptionalOutput = nullptr) const
{

}

void UK2Node_CastRpaiMemoryStructToStruct::ConvertDeprecatedNode(UEdGraph* Graph, bool bOnlySafeChanges)
{

}

ERedirectType UK2Node_CastRpaiMemoryStructToStruct::DoPinsMatchForReconstruction(const UEdGraphPin* NewPin, int32 NewPinIndex, const UEdGraphPin* OldPin, int32 OldPinIndex) const
{

}

FString UK2Node_CastRpaiMemoryStructToStruct::GetFindReferenceSearchString() const
{

}

bool UK2Node_CastRpaiMemoryStructToStruct::IsActionFilteredOut(class FBlueprintActionFilter const& Filter)
{

}

// Handler
class FKCHandler_CastRpaiMemoryStructToStruct : public FNodeHandlingFunctor
{
public:
	FKCHandler_CastRpaiMemoryStructToStruct(FKismetCompilerContext& InCompilerContext)
		: FNodeHandlingFunctor(InCompilerContext)
	{
	}

	FBPTerminal* RegisterInputTerm(FKismetFunctionContext& Context, UK2Node_CastRpaiMemoryStructToStruct* Node)
	{
		check(NULL != Node);

		if (NULL == Node->TargetStructType)
		{
			CompilerContext.MessageLog.Error(*NSLOCTEXT("RpaiEditor", "BreakStruct_UnknownStructure_Error", "Unknown structure to break for @@").ToString(), Node);
			return NULL;
		}

		//Find input pin
		UEdGraphPin* InputPin = NULL;
		for (int32 PinIndex = 0; PinIndex < Node->Pins.Num(); ++PinIndex)
		{
			UEdGraphPin* Pin = Node->Pins[PinIndex];
			if (Pin && (EGPD_Input == Pin->Direction))
			{
				InputPin = Pin;
				break;
			}
		}
		check(NULL != InputPin);

		//Find structure source net
		UEdGraphPin* Net = FEdGraphUtilities::GetNetFromPin(InputPin);
		check(NULL != Net);

		FBPTerminal** FoundTerm = Context.NetMap.Find(Net);
		FBPTerminal* Term = FoundTerm ? *FoundTerm : NULL;
		if (NULL == Term)
		{
			// Dont allow literal
			if ((Net->Direction == EGPD_Input) && (Net->LinkedTo.Num() == 0))
			{
				CompilerContext.MessageLog.Error(*NSLOCTEXT("RpaiEditor", "InvalidNoInputStructure_Error", "No input structure to break for @@").ToString(), Net);
				return NULL;
			}
			// standard register net
			else
			{
				Term = Context.CreateLocalTerminalFromPinAutoChooseScope(Net, Context.NetNameMap->MakeValidName(Net));
			}
			Context.NetMap.Add(Net, Term);
		}
		UStruct* StructInTerm = Cast<UStruct>(Term->Type.PinSubCategoryObject.Get());
		if (NULL == StructInTerm || !StructInTerm->IsChildOf(Node->TargetStructType))
		{
			CompilerContext.MessageLog.Error(*NSLOCTEXT("RpaiEditor", "BreakStruct_NoMatch_Error", "Structures don't match for @@").ToString(), Node);
		}

		return Term;
	}

	void RegisterOutputTerm(FKismetFunctionContext& Context, UScriptStruct* StructType, UEdGraphPin* Net, FBPTerminal* ContextTerm)
	{
		if (FProperty* BoundProperty = FindFProperty<FProperty>(StructType, Net->PinName))
		{
			if (BoundProperty->HasAnyPropertyFlags(CPF_Deprecated) && Net->LinkedTo.Num())
			{
				FText Message = FText::Format(NSLOCTEXT("RpaiEditor", "BreakStruct_DeprecatedField_Warning", "@@ : Member '{0}' of struct '{1}' is deprecated.")
					, BoundProperty->GetDisplayNameText()
					, StructType->GetDisplayNameText());
				CompilerContext.MessageLog.Warning(*Message.ToString(), Net->GetOuter());
			}

			FBPTerminal* Term = Context.CreateLocalTerminalFromPinAutoChooseScope(Net, Net->PinName.ToString());
			Term->bPassedByReference = ContextTerm->bPassedByReference;
			Term->AssociatedVarProperty = BoundProperty;
			Context.NetMap.Add(Net, Term);
			Term->Context = ContextTerm;

			if (BoundProperty->HasAnyPropertyFlags(CPF_BlueprintReadOnly))
			{
				Term->bIsConst = true;
			}
		}
		else
		{
			CompilerContext.MessageLog.Error(TEXT("Failed to find a struct member for @@"), Net);
		}
	}

	virtual void RegisterNets(FKismetFunctionContext& Context, UEdGraphNode* InNode) override
	{
		UK2Node_CastRpaiMemoryStructToStruct* Node = Cast<UK2Node_CastRpaiMemoryStructToStruct>(InNode);
		check(Node);

		if (FBPTerminal* StructContextTerm = RegisterInputTerm(Context, Node))
		{
			for (UEdGraphPin* Pin : Node->Pins)
			{
				if (Pin && EGPD_Output == Pin->Direction)
				{
					RegisterOutputTerm(Context, Node->TargetStructType, Pin, StructContextTerm);
				}
			}
		}
	}
};

FNodeHandlingFunctor* UK2Node_CastRpaiMemoryStructToStruct::CreateNodeHandler(class FKismetCompilerContext& CompilerContext) const
{
	return new FKCHandler_CastRpaiMemoryStructToStruct(CompilerContext);
}