// Copyright Epic Games, Inc. All Rights Reserved.


#include "K2Node_RpaiWriteMemory.h"

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

FName UK2Node_RpaiWriteMemory::MemoryStructInputPin = FName(TEXT("MemoryStruct"));

FName UK2Node_RpaiWriteMemory::WriteStructInputPin = FName(TEXT("StructToWrite"));

void UK2Node_RpaiWriteMemory::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
   bool bIsDirty = false;
   FName PropertyName = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
   if (PropertyName == GET_MEMBER_NAME_CHECKED(UK2Node_RpaiWriteMemory, StructType))
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

void UK2Node_RpaiWriteMemory::PreloadRequiredAssets()
{
   Super::PreloadRequiredAssets();
   PreloadObject(StructType);
}

UEdGraphPin* UK2Node_RpaiWriteMemory::GetMemoryStructInputPin() const
{
   return FindPin(MemoryStructInputPin, EGPD_Input);
}

UEdGraphPin* UK2Node_RpaiWriteMemory::GetThenOutputPin() const
{
   return FindPin(UEdGraphSchema_K2::PN_Then, EGPD_Output);
}

UEdGraphPin* UK2Node_RpaiWriteMemory::GetInvalidOutputPin() const
{
   return FindPin(UEdGraphSchema_K2::PN_Else, EGPD_Output);
}

FText UK2Node_RpaiWriteMemory::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
   if (StructType == nullptr)
   {
      return NSLOCTEXT("Rpai", "WriteMemoryNullStructTitle", "Write Memory as <unknown struct>");
   }
   else if (CachedTitle.IsOutOfDate(this))
   {
      FFormatNamedArguments Args;
      Args.Add(TEXT("StructName"), FText::FromName(StructType->GetFName()));
      CachedTitle.SetCachedText(FText::Format(NSLOCTEXT("Rpai", "WriteMemoryAsStruct", "Write Memory As {StructName}"), Args), this);
   }
   return CachedTitle;
}

void UK2Node_RpaiWriteMemory::AllocateDefaultPins()
{
   CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
   CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, FRpaiMemoryStruct::StaticStruct(), MemoryStructInputPin);
   if (StructType != nullptr)
   {
      CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, StructType, WriteStructInputPin);
      auto Then = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);
      Then->PinFriendlyName = NSLOCTEXT("Rpai", "ThenPin", "Valid");
      auto Else = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Else);
      Else->PinFriendlyName = NSLOCTEXT("Rpai", "InvalidPin", "Invalid");
   }
}

void UK2Node_RpaiWriteMemory::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
   Super::ExpandNode(CompilerContext, SourceGraph);
   auto InputPin = GetMemoryStructInputPin();
   if (StructType && InputPin != nullptr)
   {
      const UEdGraphSchema_K2* Schema = CompilerContext.GetSchema();

      const FName WriteMemoryFunctionName = GET_FUNCTION_NAME_CHECKED(URpaiBPLibrary, WriteMemory);
      const UFunction* WriteMemoryFunction = URpaiBPLibrary::StaticClass()->FindFunctionByName(WriteMemoryFunctionName);
      check(WriteMemoryFunction != nullptr);

      UK2Node_CallFunction* WriteMemory = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
      WriteMemory->SetFromFunction(WriteMemoryFunction);
      WriteMemory->AllocateDefaultPins();

      UEdGraphPin* OriginalInStructPin = FindPinChecked(WriteStructInputPin, EGPD_Input);
      UEdGraphPin* WriteMemoryInParamStruct = WriteMemory->FindPinChecked(TEXT("InStruct"));
      WriteMemoryInParamStruct->PinType = OriginalInStructPin->PinType;
      WriteMemoryInParamStruct->PinType.PinSubCategoryObject = OriginalInStructPin->PinType.PinSubCategoryObject;
      CompilerContext.MovePinLinksToIntermediate(*OriginalInStructPin, *WriteMemoryInParamStruct);

      UEdGraphPin* WriteMemoryMemoryInputPin = WriteMemory->FindPinChecked(TEXT("Memory"));

      CompilerContext.MovePinLinksToIntermediate(*InputPin, *WriteMemoryMemoryInputPin);
      CompilerContext.MovePinLinksToIntermediate(*GetExecPin(), *WriteMemory->GetExecPin());

      UK2Node_IfThenElse* IfThenElse = CompilerContext.SpawnIntermediateNode<UK2Node_IfThenElse>(this, SourceGraph);
      IfThenElse->AllocateDefaultPins();

      bool bConnected = Schema->TryCreateConnection(WriteMemory->GetReturnValuePin(), IfThenElse->GetConditionPin());
      check(bConnected);

      bConnected = Schema->TryCreateConnection(WriteMemory->GetThenPin(), IfThenElse->GetExecPin());
      check(bConnected);

      CompilerContext.MovePinLinksToIntermediate(*GetThenOutputPin(), *IfThenElse->GetThenPin());
      CompilerContext.MovePinLinksToIntermediate(*GetInvalidOutputPin(), *IfThenElse->GetElsePin());

      BreakAllNodeLinks();
   }
}

void UK2Node_RpaiWriteMemory::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
   struct GetMenuActions_Utils
   {
      static void SetNodeStructType(UEdGraphNode* NewNode, FFieldVariant /*StructField*/, TWeakObjectPtr<UScriptStruct> NonConstStructPtr)
      {
         UK2Node_RpaiWriteMemory* StructNode = CastChecked<UK2Node_RpaiWriteMemory>(NewNode);
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
