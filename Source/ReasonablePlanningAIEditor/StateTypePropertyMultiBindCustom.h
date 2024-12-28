// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"
#include "IPropertyTypeCustomization.h"

/**
 * Provides customizations from the StateTypePropertyMultiBind when viewed in all details.
 * Features include a struct or class picker, better formatting, and strongly referenced
 * property path pickers.
 */
class REASONABLEPLANNINGAIEDITOR_API StateTypePropertyMultiBindCustom : public IPropertyTypeCustomization
{
public:
	StateTypePropertyMultiBindCustom();

	/** IPropertyTypeCustomization **/
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;

	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

private:
	TSharedRef<SWidget> GenerateClassPicker(TSharedRef<IPropertyHandle> Parent, TSharedPtr<IPropertyHandle> Property);
	TSharedRef<SWidget> GenerateStructPicker(TSharedRef<IPropertyHandle> Parent, TSharedPtr<IPropertyHandle> Property);

	void OnClassPicked(UClass* InClassPicked, TSharedRef<IPropertyHandle> Parent, TSharedPtr<IPropertyHandle> Property);
	void OnStructPicked(const UScriptStruct* InStructPicked, TSharedRef<IPropertyHandle> Parent, TSharedPtr<IPropertyHandle> Property);

	TSharedPtr<SComboButton> ClassPickerComboButton;
	TSharedPtr<SComboButton> StructPickerComboButton;

	void AddBindingButton_OnClick(TSharedRef<IPropertyHandle> Parent, TSharedPtr<IPropertyHandleArray> Property);
	void ClearBindingButton_OnClick(TSharedRef<IPropertyHandle> Parent, TSharedPtr<IPropertyHandleArray> Property);
	FText GetAddNewBindingTooltip() const;
	bool CanAddNewBinding() const;
	FText GetClearBindingTooltip() const;

	FText GetBoundPropertyName(TSharedPtr<IPropertyHandle> Element);
};
