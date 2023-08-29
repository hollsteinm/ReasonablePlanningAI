// Troll Purse. All rights reserved.


#include "ComposerBehaviorAssetTypeActions.h"
#include "Composer/RpaiComposerBehavior.h"
#include "Modules/ModuleManager.h"
#include "AIModule.h"

FComposerBehaviorAssetTypeActions::FComposerBehaviorAssetTypeActions(EAssetTypeCategories::Type ModuleAssetCategory)
	: AssetCategory(ModuleAssetCategory)
{
}

FComposerBehaviorAssetTypeActions::~FComposerBehaviorAssetTypeActions()
{
}

UClass* FComposerBehaviorAssetTypeActions::GetSupportedClass() const
{
	return URpaiComposerBehavior::StaticClass();
}

FText FComposerBehaviorAssetTypeActions::GetName() const
{
	return INVTEXT("Rpai Behavior");
}

FColor FComposerBehaviorAssetTypeActions::GetTypeColor() const
{
	return FColor::Purple;
}

uint32 FComposerBehaviorAssetTypeActions::GetCategories()
{
	return AssetCategory;
}