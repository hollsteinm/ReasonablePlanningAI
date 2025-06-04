// Radaway Software LLC. 2025. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"

/**
 * Provides customizations from the RpaiType FStateKeyValueReference when viewed in all details.
 * Primarily, it just inlines the properties to look nice.
 */
class REASONABLEPLANNINGAIEDITOR_API StateKeyValueReferenceCustom : public IPropertyTypeCustomization
{
public:
   virtual void CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;
   virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;

    static TSharedRef<IPropertyTypeCustomization> MakeInstance();
};
