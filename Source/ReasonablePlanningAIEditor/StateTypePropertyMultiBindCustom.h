// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"
#include "IPropertyTypeCustomization.h"

/**
 * Provides customizations from the RpaiType UStateQuery_Comparison when viewed in all details.
 * Primarily, it just inlines the properties to look nice.
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
	TSharedRef<SWidget> GenerateClassPicker();
	TSharedRef<SWidget> GenerateStructPicker();

	void OnClassPicked(UClass* InClassPicked);
	void OnStructPicked(const UScriptStruct* InStruct);

	TSharedPtr<SComboButton> ClassPickerComboButton;
	TSharedPtr<SComboButton> StructPickerComboButton;
};
