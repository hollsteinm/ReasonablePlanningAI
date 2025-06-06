// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Core/RpaiGoalBase.h"

DECLARE_DELEGATE_OneParam(FOnPropertyPathPicked, FString)

class SCachedPropertyPathStructPropertyPicker : public SCompoundWidget
{
public:
   SLATE_BEGIN_ARGS(SCachedPropertyPathStructPropertyPicker)
      : _PickerClass(nullptr)
      , _OnPropertyPathPicked()
      , _InitialPath()
      , _bShowComboButton(true)
      {}
      SLATE_ATTRIBUTE(UStruct*, PickerClass)
      SLATE_EVENT(FOnPropertyPathPicked, OnPropertyPathPicked)
      SLATE_ARGUMENT(FString, InitialPath)
      SLATE_ARGUMENT(bool, bShowComboButton)
   SLATE_END_ARGS()

   virtual void Construct(const FArguments& InArgs);

private:
   // Slate bound
   TAttribute<UStruct*> PickerClass;
   FOnPropertyPathPicked OnPropertyPathPicked;

   TSharedPtr<SComboButton> ComboButton;
   TSharedRef<SWidget> GetPropertyPathDropdown();
   TSharedPtr<SWidget> CachedPropertyPathDropdown;

   // State Management
   UStruct* CachedPickerClass;
   FString SelectionString;

   void OnSelection(FProperty* Leaf, TArray<FProperty*> Parents);
   void RecursiveMenuBuilder(FMenuBuilder& MenuBuilder, UStruct* GivenPickerClass, TArray<FProperty*> Parents);
};
