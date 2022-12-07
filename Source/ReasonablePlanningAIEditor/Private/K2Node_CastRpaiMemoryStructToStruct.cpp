// Copyright Epic Games, Inc. All Rights Reserved.


#include "K2Node_CastRpaiMemoryStructToStruct.h"

#include "Core/RpaiTypes.h"
#include "BlueprintActionFilter.h"
#include "BlueprintFieldNodeSpawner.h"
#include "BlueprintNodeBinder.h"
#include "BlueprintNodeSpawner.h"
#include "KismetCompiler.h"
#include "K2Node_BreakStruct.h"
#include "EdGraphUtilities.h"

FName UK2Node_CastRpaiMemoryStructToStruct::MemoryStructInputPin = FName(TEXT("MemoryStruct"));
FName UK2Node_CastRpaiMemoryStructToStruct::MemoryStructInputPin = FName(TEXT("AsStructType"));

UK2Node_CastRpaiMemoryStructToStruct::UK2Node_CastRpaiMemoryStructToStruct(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, StructType(nullptr)
{
}


UEdGraphPin* UK2Node_CastRpaiMemoryStructToStruct::GetMemoryStructInputPin() const
{
	return FindPin(*MemoryStructInputPin.ToString(), EGPD_Input);
}

void UK2Node_CastRpaiMemoryStructToStruct::AllocateDefaultPins()
{
	FCreatePinParams InputParams;
	InputParams.bIsReference = true;
	InputParams.bIsConst = true;

	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, FRpaiMemoryStruct::StaticStruct(), *MemoryStructInputPin.ToString(), InputParams);
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, UScriptStruct::StaticClass(), *StructTypeInputPin.ToString(), InputParams);
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, StructType, UEdGraphSchema_K2::PN_ReturnValue);
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
	UClass* ActionKey = GetClass();
	if (ActionRegistrar.IsOpenForRegistration(ActionKey))
	{
		UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
		check(NodeSpawner != nullptr);

		ActionRegistrar.AddBlueprintAction(ActionKey, NodeSpawner);
	}
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

		FBPTerminal** ValueSource = Context.NetMap.Find(Net);
		check(ValueSource && *ValueSource);
		UEdGraphPin* OutPin = Node->FindPinChecked(UEdGraphSchema_K2::PN_ReturnValue);
		if (ensure(Context.NetMap.Find(OutPin) == nullptr))
		{
			FBPTerminal* ValueSourceCopy = *ValueSource;
			Context.NetMap.Add(OutPin, ValueSourceCopy);
		}
	}
};

FNodeHandlingFunctor* UK2Node_CastRpaiMemoryStructToStruct::CreateNodeHandler(FKismetCompilerContext& CompilerContext) const
{
	return new FKCHandler_CastMemoryStructToStruct(CompilerContext);
}