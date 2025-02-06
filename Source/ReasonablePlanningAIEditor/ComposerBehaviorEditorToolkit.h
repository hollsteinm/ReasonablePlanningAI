// Troll Purse. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Slate/SComposerBehaviorWidget.h"

/**
 * Toolkit that manages the custom asset editor for RpaiComposerBehavior. Provides editing data properties and
 * running experiments.
 */
class REASONABLEPLANNINGAIEDITOR_API FComposerBehaviorEditorToolkit : public FAssetEditorToolkit, FNotifyHook
{
public:
    void InitEditor(const TArray<UObject*>& InObjects);

    void RegisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;
    void UnregisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;

    FName GetToolkitFName() const override { return "RpaiComposerBehaviorEditor"; }
    FText GetBaseToolkitName() const override { return INVTEXT("RPAI Composer Behavior Editor"); }
    FString GetWorldCentricTabPrefix() const override { return "RPAI Behavior"; }
    FLinearColor GetWorldCentricTabColorScale() const override { return {}; }
    
    /**
     Notify Hook Interface methods
     */
    virtual void NotifyPostChange(const FPropertyChangedEvent& PropertyChangedEvent, FProperty* PropertyThatChanged) override;
    virtual void NotifyPostChange(const FPropertyChangedEvent& PropertyChangedEvent, FEditPropertyChain* PropertyThatChanged) override;
    virtual void NotifyPreChange(FProperty* PropertyChangedEvent) override;
    virtual void NotifyPreChange(FEditPropertyChain* PropertyThatChanged) override;
    
private:
    URpaiComposerBehavior* Behavior;
    TSharedPtr<SComposerBehaviorWidget> ExperimentWidget;
};
