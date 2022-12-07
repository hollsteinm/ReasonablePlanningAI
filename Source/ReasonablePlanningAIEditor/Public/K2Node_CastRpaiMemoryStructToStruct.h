// Copyright Troll Purse, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "K2Node_CastRpaiMemoryStructToStruct.generated.h"

UCLASS()
class REASONABLEPLANNINGAIEDITOR_API UK2Node_CastRpaiMemoryStructToStruct : public UK2Node
{
	GENERATED_UCLASS_BODY()

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UScriptStruct> StructType;

public:
	static FName MemoryStructInputPin;
	static FName StructTypeInputPin;

#if WITH_EDITOR
	//~ Begin UEdGraphNode Interface
	virtual void AllocateDefaultPins() override;
	//~ End UEdGraphNode Interface

	//~ Begin UK2Node Interface
	virtual void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	virtual FNodeHandlingFunctor* UK2Node_CastRpaiMemoryStructToStruct::CreateNodeHandler(FKismetCompilerContext& CompilerContext) const override;

	virtual bool IsNodePure() const override { return true; }
	//~ End UK2Node Interface

	UEdGraphPin* GetMemoryStructInputPin() const;
#endif
};