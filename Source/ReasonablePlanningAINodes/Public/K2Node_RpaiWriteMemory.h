// Copyright Troll Purse, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "K2Node_RpaiWriteMemory.generated.h"

UCLASS(MinimalAPI)
class UK2Node_RpaiWriteMemory : public UK2Node
{
	GENERATED_BODY()

private:
	FNodeTextCache CachedTitle;

protected:
	UPROPERTY()
		UScriptStruct* StructType;

public:
	static FName MemoryStructInputPin;

	static FName WriteStructInputPin;

	FORCEINLINE UScriptStruct* GetStructType() const { return StructType; }

	//~ Begin UObject Interface
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	//~ End UObject Interface

	//~ Begin UEdGraphNode Interface
	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	//~ End UEdGraphNode Interface

	//~ Begin UK2Node Interface
	virtual void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	virtual void PreloadRequiredAssets() override;

	virtual bool IsNodePure() const override { return false; }
	//~ End UK2Node Interface

	UEdGraphPin* GetMemoryStructInputPin() const;
	UEdGraphPin* GetThenOutputPin() const;
	UEdGraphPin* GetInvalidOutputPin() const;
};