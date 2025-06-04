#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Core/RpaiGoalBase.h"

class SStateTypePropertyMultibind : public SCompoundWidget
{
public:
   SLATE_BEGIN_ARGS(SStateTypePropertyMultibind)
      : _AllowedClasses()
      , _StructPropertyHandle()
      {}
      SLATE_ARGUMENT(TArray<TObjectPtr<UClass>>, AllowedClasses)
      SLATE_ARGUMENT(TSharedPtr<IPropertyHandle>, StructPropertyHandle)
   SLATE_END_ARGS()

   virtual void Construct(const FArguments& InArgs);

private:
   // Slate bound
   TArray<TObjectPtr<UClass>> AllowedClasses;
   TSharedPtr<IPropertyHandle> StructPropertyHandle;

   TSharedRef<SWidget> GenerateClassPicker(TSharedPtr<IPropertyHandle> Property);

   void OnClassPicked(UClass* InClassPicked,  TSharedPtr<IPropertyHandle> Property);

   TSharedPtr<SComboButton> ClassPickerComboButton;

   void AddBindingButton_OnClick(TSharedPtr<IPropertyHandleArray> Property);
   void ClearBindingButton_OnClick(TSharedPtr<IPropertyHandleArray> Property);
   FText GetAddNewBindingTooltip() const;
   bool CanAddNewBinding() const;
   FText GetClearBindingTooltip() const;
};
