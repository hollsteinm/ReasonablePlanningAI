// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"

/**
 * Basic asset action registry
 */
class REASONABLEPLANNINGAIEDITOR_API FComposerBehaviorAssetTypeActions : public FAssetTypeActions_Base
{
public:
	FComposerBehaviorAssetTypeActions(EAssetTypeCategories::Type ModuleAssetCategory);
	~FComposerBehaviorAssetTypeActions();

	virtual UClass* GetSupportedClass() const override;
	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual uint32 GetCategories() override;

private:
	EAssetTypeCategories::Type AssetCategory;
};
