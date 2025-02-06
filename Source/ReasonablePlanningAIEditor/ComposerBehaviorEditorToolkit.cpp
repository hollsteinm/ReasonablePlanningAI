// Troll Purse. All rights reserved.


#include "ComposerBehaviorEditorToolkit.h"
#include "Widgets/Docking/SDockTab.h"
#include "Modules/ModuleManager.h"

void FComposerBehaviorEditorToolkit::InitEditor(const TArray<UObject*>& InObjects)
{
    if(InObjects.Num() <= 0)
    {
        return;
    }
    
    Behavior = Cast<URpaiComposerBehavior>(InObjects[0]);
 
    const TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout("RpaiComposerBehaviorEditorLayout")
    ->AddArea
    (
        FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
        ->Split
        (
            FTabManager::NewSplitter()
            ->SetSizeCoefficient(0.6f)
            ->SetOrientation(Orient_Horizontal)
            ->Split
            (
                FTabManager::NewStack()
                ->SetSizeCoefficient(0.8f)
                ->AddTab("RpaiComposerBehaviorExperimentTab", ETabState::OpenedTab)
            )
            ->Split
            (
                FTabManager::NewStack()
                ->SetSizeCoefficient(0.2f)
                ->AddTab("RpaiComposerBehaviorDetailsTab", ETabState::OpenedTab)
            )
        )
    );
    FAssetEditorToolkit::InitAssetEditor(EToolkitMode::Standalone, {}, "RpaiComposerBehaviorEditor", Layout, true, true, InObjects);
}

void FComposerBehaviorEditorToolkit::RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
    FAssetEditorToolkit::RegisterTabSpawners(InTabManager);
 
    WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(INVTEXT("RPAI Composer Behavior Editor"));
 
    InTabManager->RegisterTabSpawner("RpaiComposerBehaviorExperimentTab", FOnSpawnTab::CreateLambda([this](const FSpawnTabArgs&)
    {
        return SNew(SDockTab)
        [
            SAssignNew(ExperimentWidget, SComposerBehaviorWidget)
            .ComposerBehavior(Behavior)
        ];
    }))
    .SetDisplayName(INVTEXT("Experiment"))
    .SetGroup(WorkspaceMenuCategory.ToSharedRef());
 
    FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
    FDetailsViewArgs DetailsViewArgs;
    DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
    DetailsViewArgs.NotifyHook = this;
    TSharedRef<IDetailsView> DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
    DetailsView->SetObjects(TArray<UObject*>{ Behavior });
    InTabManager->RegisterTabSpawner("RpaiComposerBehaviorDetailsTab", FOnSpawnTab::CreateLambda([=](const FSpawnTabArgs&)
    {
        return SNew(SDockTab)
        [
            DetailsView
        ];
    }))
    .SetDisplayName(INVTEXT("Details"))
    .SetGroup(WorkspaceMenuCategory.ToSharedRef());
}

void FComposerBehaviorEditorToolkit::UnregisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
    FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
    InTabManager->UnregisterTabSpawner("RpaiComposerBehaviorExperimentTab");
    InTabManager->UnregisterTabSpawner("RpaiComposerBehaviorDetailsTab");
}

void FComposerBehaviorEditorToolkit::NotifyPostChange(const FPropertyChangedEvent& PropertyChangedEvent, FProperty* PropertyThatChanged)
{
    if(PropertyChangedEvent.GetPropertyName().IsEqual("ConstructedStateType"))
    {
        if(ExperimentWidget)
        {
            ExperimentWidget->NotifyStateTypePropertyChanged();
        }
    }
    else if(PropertyChangedEvent.GetPropertyName().IsEqual("Goals"))
    {
        if(ExperimentWidget)
        {
            ExperimentWidget->NotifyGoalsPropertyChanged();
        }
    }
}

void FComposerBehaviorEditorToolkit::NotifyPostChange(const FPropertyChangedEvent& PropertyChangedEvent, FEditPropertyChain* PropertyThatChanged)
{
    if(PropertyChangedEvent.GetPropertyName().IsEqual("ConstructedStateType"))
    {
        if(ExperimentWidget)
        {
            ExperimentWidget->NotifyStateTypePropertyChanged();
        }
    }
    else if(PropertyChangedEvent.GetPropertyName().IsEqual("Goals"))
    {
        if(ExperimentWidget)
        {
            ExperimentWidget->NotifyGoalsPropertyChanged();
        }
    }
}

void FComposerBehaviorEditorToolkit::NotifyPreChange(FProperty* PropertyChangedEvent)
{
}

void FComposerBehaviorEditorToolkit::NotifyPreChange(FEditPropertyChain* PropertyThatChanged)
{
}
