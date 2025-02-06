// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * When using an abstract base class within a collection, resolve the most child class declaration of the implementation
 * and use that property customization instead.
 */
class REASONABLEPLANNINGAIEDITOR_API ComposerStateQueryCustom : public IPropertyTypeCustomization
{
public:

	/** IPropertyTypeCustomization **/
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;

	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	/**
	* Cannot utilize property module becuase the necessary methods are not exported.
	**/
	static void AddChildCustomization(const FName Name, TSharedRef<IPropertyTypeCustomization> Customization);

private:
	static TMap<FName, TSharedRef<IPropertyTypeCustomization>> Customizations;
};
