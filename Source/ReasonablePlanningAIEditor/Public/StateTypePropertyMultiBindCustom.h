// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.

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
};
