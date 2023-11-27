// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class REASONABLEPLANNINGAIEDITOR_API FComposerBehaviorEditorToolkit : public FAssetEditorToolkit
{
public:
    void InitEditor(const TArray<UObject*>& InObjects);

    void RegisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;
    void UnregisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;

    FName GetToolkitFName() const override { return "RpaiComposerBehaviorEditor"; }
    FText GetBaseToolkitName() const override { return INVTEXT("RPAI Composer Behavior Editor"); }
    FString GetWorldCentricTabPrefix() const override { return "RPAI Behavior"; }
    FLinearColor GetWorldCentricTabColorScale() const override { return {}; }
    
private:
    URpaiComposerBehavior* Behavior;
};
