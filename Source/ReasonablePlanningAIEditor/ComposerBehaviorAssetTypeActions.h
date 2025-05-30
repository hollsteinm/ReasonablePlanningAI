// Radaway Software LLC. 2025. All rights reserved.

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
    
    void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor) override;

private:
	EAssetTypeCategories::Type AssetCategory;
};
