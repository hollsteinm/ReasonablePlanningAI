// Radaway Software LLC. 2025. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"
#include "IPropertyTypeCustomization.h"

/**
 * Provides customizations from the RpaiType UStateQuery_Comparison when viewed in all details.
 * Primarily, it just inlines the properties to look nice.
 */
class REASONABLEPLANNINGAIEDITOR_API StateQueryComparisonCustom : public IDetailCustomization, public IPropertyTypeCustomization

{
public:
	StateQueryComparisonCustom(FName RHSFieldName);

	/** IDetailCustomization **/
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

	/** IPropertyTypeCustomization **/
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;

    static TSharedRef<IDetailCustomization> MakeInstance(FName RHSFieldName);
	static TSharedRef<IPropertyTypeCustomization> MakePropertyInstance(FName RHSFieldName);

private:
	FName RHSFieldName;
};
