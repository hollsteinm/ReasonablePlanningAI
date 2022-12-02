// Copyright Troll Purse, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "EdGraph/EdGraphNodeUtils.h"
#include "Internationalization/Text.h"
#include "K2Node.h"
#include "KismetCompilerMisc.h"
#include "Textures/SlateIcon.h"
#include "UObject/Class.h"
#include "UObject/NameTypes.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ObjectPtr.h"
#include "UObject/UObjectGlobals.h"

#include "K2Node_CastRpaiMemoryStructToStruct.generated.h"

class FBlueprintActionDatabaseRegistrar;
class FKismetCompilerContext;
class FNodeHandlingFunctor;
class FString;
class UEdGraph;
class UEdGraphPin;
class UObject;
struct FLinearColor;
struct FObjectPreSaveContext;

UCLASS(BlueprintType, Blueprintable)
class REASONABLEPLANNINGAIEDITOR_API UK2Node_CastRpaiMemoryStructToStruct : public UK2Node
{
	GENERATED_UCLASS_BODY()

public:
	static const FName ConvertToStructInputPinName;
	static const FName ConvertToStructSuccessPinName;

	UPROPERTY()
	TObjectPtr<UScriptStruct> TargetStructType;

	// UObject interface
	virtual void PreSave(FObjectPreSaveContext SaveContext) override;
	virtual void Serialize(FArchive& Ar) override;
	// End of UObject interface

	//~ Begin UEdGraphNode Interface
	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	virtual void ValidateNodeDuringCompilation(class FCompilerResultsLog& MessageLog) const override;
	virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;
	virtual void PreloadRequiredAssets() override;
	virtual void PostPlacedNewNode() override;
	virtual FString GetPinMetaData(FName InPinName, FName InKey) override;
	virtual bool HasExternalDependencies(TArray<class UStruct*>* OptionalOutput = nullptr) const override;
	//~ End  UEdGraphNode Interface

	//~ Begin K2Node Interface
	virtual bool NodeCausesStructuralBlueprintChange() const override { return false; }
	virtual bool IsNodePure() const override { return true; }
	virtual bool DrawNodeAsVariable() const override { return false; }
	virtual class FNodeHandlingFunctor* CreateNodeHandler(class FKismetCompilerContext& CompilerContext) const override;
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	virtual FText GetMenuCategory() const override;
	virtual bool ShouldShowNodeProperties() const override { return true; }
	virtual void ValidateNodeDuringCompilation(class FCompilerResultsLog& MessageLog) const override;
	virtual void ConvertDeprecatedNode(UEdGraph* Graph, bool bOnlySafeChanges) override;
	virtual ERedirectType DoPinsMatchForReconstruction(const UEdGraphPin* NewPin, int32 NewPinIndex, const UEdGraphPin* OldPin, int32 OldPinIndex) const override;
	virtual FString GetFindReferenceSearchString() const override;
	virtual bool IsActionFilteredOut(class FBlueprintActionFilter const& Filter) override;
	//~ End K2Node Interface

	UEdGraphPin* GetValidCastPin() const;
	UEdGraphPin* GetInvalidCastPin() const;
	UEdGraphPin* GetCastResultPin() const;
	UEdGraphPin* GetCastSourcePin() const;
	UEdGraphPin* GetBoolSuccessPin() const;

protected:
	FNodeTextCache CachedNodeTitle;
};