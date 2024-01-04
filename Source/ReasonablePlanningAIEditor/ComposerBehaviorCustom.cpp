// Troll Purse. All rights reserved.

#include "ComposerBehaviorCustom.h"
#include "PropertyEditing.h"
#include "PropertyCustomizationHelpers.h"

TSharedRef<IDetailCustomization> ComposerBehaviorCustom::MakeInstance()
{
	return MakeShareable(new ComposerBehaviorCustom());
}

void ComposerBehaviorCustom::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	IDetailCategoryBuilder& RpaiCategory = DetailBuilder.EditCategory("Rpai", FText::GetEmpty(), ECategoryPriority::Important);
}