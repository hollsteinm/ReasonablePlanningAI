// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.

#include "Slate/SCachedPropertyPathStructPropertyPicker.h"
#include "ISinglePropertyView.h"
#include "PropertyPathHelpers.h"
#include "Editor.h"
#include "Widgets/SBoxPanel.h"
#include "SlateCore.h"
#include "EditorStyleSet.h"
#include "PropertyPath.h"

#define LOCTEXT_NAMESPACE "ReasonablePlanningAIEditor"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SCachedPropertyPathStructPropertyPicker::Construct(const FArguments& InArgs)
{
   PickerClass = InArgs._PickerClass;
   OnPropertyPathPicked = InArgs._OnPropertyPathPicked;
   SelectionString = InArgs._InitialPath;

   if (PickerClass.IsSet() || PickerClass.IsBound())
   {
      CachedPickerClass = InArgs._PickerClass.Get();
      ChildSlot
         [
            SAssignNew(ComboButton, SComboButton)
               .OnGetMenuContent(FOnGetContent::CreateSP(this, &SCachedPropertyPathStructPropertyPicker::GetPropertyPathDropdown))
               .ButtonContent()
               [
                  SNew(STextBlock)
                     .Text(TAttribute<FText>::Create(TAttribute<FText>::FGetter::CreateLambda([this]() -> FText {
                     if (SelectionString.IsEmpty())
                     {
                        return LOCTEXT("RpaiPickProperty", "Pick Property");
                     }
                     else
                     {
                        //TODO: verify exsists and get type for display
                        return FText::FromString(SelectionString);
                     }
                  })))
               ]
         ];
   }
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SCachedPropertyPathStructPropertyPicker::RecursiveMenuBuilder(FMenuBuilder& MenuBuilder, UStruct* GivenPickerClass, TArray<FProperty*> Parents)
{
   // 5 is a hardcoded value for the max recursion level of the box filters.
   const int32 MaxSubMenus = 5;

   for (TFieldIterator<FProperty> Piter(GivenPickerClass, EFieldIterationFlags::IncludeDeprecated | EFieldIterationFlags::IncludeSuper); Piter; ++Piter)
   {
      FProperty* Prop = *Piter;
      if (Piter->IsA<FBoolProperty>())
      {
         MenuBuilder.AddMenuEntry(
            Piter->GetDisplayNameText(),
            Piter->GetToolTipText(),
            FSlateIcon(FEditorStyle::GetStyleSetName(), TEXT("Kismet.Tabs.Variables")),
            FUIAction(FExecuteAction::CreateSP(this, &SCachedPropertyPathStructPropertyPicker::OnSelection, Prop, Parents))
         );
      }
      else if (Piter->IsA<FClassProperty>())
      {
         MenuBuilder.AddMenuEntry(
            Piter->GetDisplayNameText(),
            Piter->GetToolTipText(),
            FSlateIcon(FEditorStyle::GetStyleSetName(), TEXT("Kismet.Tabs.Variables")),
            FUIAction(FExecuteAction::CreateSP(this, &SCachedPropertyPathStructPropertyPicker::OnSelection, Prop, Parents))
         );
      }
      else if (Piter->IsA<FEnumProperty>())
      {
         MenuBuilder.AddMenuEntry(
            Piter->GetDisplayNameText(),
            Piter->GetToolTipText(),
            FSlateIcon(FEditorStyle::GetStyleSetName(), TEXT("Kismet.Tabs.Variables")),
            FUIAction(FExecuteAction::CreateSP(this, &SCachedPropertyPathStructPropertyPicker::OnSelection, Prop, Parents))
         );
      }
      else if (Piter->IsA<FFloatProperty>())
      {
         MenuBuilder.AddMenuEntry(
            Piter->GetDisplayNameText(),
            Piter->GetToolTipText(),
            FSlateIcon(FEditorStyle::GetStyleSetName(), TEXT("Kismet.Tabs.Variables")),
            FUIAction(FExecuteAction::CreateSP(this, &SCachedPropertyPathStructPropertyPicker::OnSelection, Prop, Parents))
         );
      }
      else if (Piter->IsA<FDoubleProperty>())
      {
         MenuBuilder.AddMenuEntry(
            Piter->GetDisplayNameText(),
            Piter->GetToolTipText(),
            FSlateIcon(FEditorStyle::GetStyleSetName(), TEXT("Kismet.Tabs.Variables")),
            FUIAction(FExecuteAction::CreateSP(this, &SCachedPropertyPathStructPropertyPicker::OnSelection, Prop, Parents))
         );
      }
      else if (Piter->IsA<FIntProperty>())
      {
         MenuBuilder.AddMenuEntry(
            Piter->GetDisplayNameText(),
            Piter->GetToolTipText(),
            FSlateIcon(FEditorStyle::GetStyleSetName(), TEXT("Kismet.Tabs.Variables")),
            FUIAction(FExecuteAction::CreateSP(this, &SCachedPropertyPathStructPropertyPicker::OnSelection, Prop, Parents))
         );
      }
      else if (Piter->IsA<FNameProperty>())
      {
         MenuBuilder.AddMenuEntry(
            Piter->GetDisplayNameText(),
            Piter->GetToolTipText(),
            FSlateIcon(FEditorStyle::GetStyleSetName(), TEXT("Kismet.Tabs.Variables")),
            FUIAction(FExecuteAction::CreateSP(this, &SCachedPropertyPathStructPropertyPicker::OnSelection, Prop, Parents))
         );
      }
      else if (Piter->IsA<FStrProperty>())
      {
         MenuBuilder.AddMenuEntry(
            Piter->GetDisplayNameText(),
            Piter->GetToolTipText(),
            FSlateIcon(FEditorStyle::GetStyleSetName(), TEXT("Kismet.Tabs.Variables")),
            FUIAction(FExecuteAction::CreateSP(this, &SCachedPropertyPathStructPropertyPicker::OnSelection, Prop, Parents))
         );
      }
      else if (Piter->IsA<FObjectProperty>() && Parents.Num() < MaxSubMenus)
      {
         TArray<FProperty*> NewParentBranch = Parents;
         NewParentBranch.Add(Prop);
         UStruct* NewType = CastField<FObjectProperty>(*Piter)->PropertyClass;
         MenuBuilder.AddSubMenu(
            Prop->GetDisplayNameText(),
            Prop->GetToolTipText(),
            FNewMenuDelegate::CreateSP(this, &SCachedPropertyPathStructPropertyPicker::RecursiveMenuBuilder, NewType, NewParentBranch)
         );
      }
      else if (Piter->IsA<FStructProperty>())
      {
         if (auto StructProperty = CastField<FStructProperty>(*Piter))
         {
            if (StructProperty->Struct == TBaseStructure<FVector>::Get())
            {
               MenuBuilder.AddMenuEntry(
                  Piter->GetDisplayNameText(),
                  Piter->GetToolTipText(),
                  FSlateIcon(FEditorStyle::GetStyleSetName(), TEXT("Kismet.Tabs.Variables")),
                  FUIAction(FExecuteAction::CreateSP(this, &SCachedPropertyPathStructPropertyPicker::OnSelection, Prop, Parents))
               );
            }
            else if (StructProperty->Struct == TBaseStructure<FRotator>::Get())
            {
               MenuBuilder.AddMenuEntry(
                  Piter->GetDisplayNameText(),
                  Piter->GetToolTipText(),
                  FSlateIcon(FEditorStyle::GetStyleSetName(), TEXT("Kismet.Tabs.Variables")),
                  FUIAction(FExecuteAction::CreateSP(this, &SCachedPropertyPathStructPropertyPicker::OnSelection, Prop, Parents))
               );
            }
            else if (Parents.Num() < MaxSubMenus)
            {
               TArray<FProperty*> NewParentBranch = Parents;
               NewParentBranch.Add(Prop);
               UStruct* NewType = StructProperty->Struct.Get();
               MenuBuilder.AddSubMenu(
                  Prop->GetDisplayNameText(),
                  Prop->GetToolTipText(),
                  FNewMenuDelegate::CreateSP(this, &SCachedPropertyPathStructPropertyPicker::RecursiveMenuBuilder, NewType, NewParentBranch)
               );
            }
         }
      }
   }
}

void SCachedPropertyPathStructPropertyPicker::OnSelection(FProperty* Leaf, TArray<FProperty*> Parents)
{
   FString NewDisplayName;
   bool FirstAddition = true;
   for (int PropertyIndex = 0; PropertyIndex < Parents.Num(); PropertyIndex++)
   {
      const FProperty* Property = Parents[PropertyIndex];
      if (!FirstAddition)
      {
         NewDisplayName += TEXT(".");
      }

      NewDisplayName += Property->GetFName().ToString();
      FirstAddition = false;
   }

   if (Parents.Num() > 0)
   {
      NewDisplayName += TEXT(".");
   }
   NewDisplayName += Leaf->GetFName().ToString();

   SelectionString = NewDisplayName;
   OnPropertyPathPicked.ExecuteIfBound(SelectionString);
}

TSharedRef<SWidget> SCachedPropertyPathStructPropertyPicker::GetPropertyPathDropdown()
{
   FMenuBuilder MenuBuilder(true, nullptr, TSharedPtr<FExtender>(), false, &FCoreStyle::Get(), true, NAME_None, false);
   if (PickerClass.IsSet() || PickerClass.IsBound())
   {
      UStruct* CurrentPickerClass = PickerClass.Get();
      if (CurrentPickerClass != CachedPickerClass || !CachedPropertyPathDropdown.IsValid())
      {
         TArray<FProperty*> Parents;
         RecursiveMenuBuilder(MenuBuilder, CurrentPickerClass, Parents);
         CachedPropertyPathDropdown = MenuBuilder.MakeWidget();
         CachedPickerClass = CurrentPickerClass;
      }
   }
   else
   {
      CachedPropertyPathDropdown = SNullWidget::NullWidget;
   }
   return CachedPropertyPathDropdown.ToSharedRef();
}