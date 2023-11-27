#pragma once
 
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Composer/RpaiComposerBehavior.h"
 
DECLARE_DELEGATE_TwoParams(FOnActionSelected, URpaiComposerBehavior*, URpaiActionBase*);
DECLARE_DELEGATE_TwoParams(FOnGoalSelected, URpaiComposerBehavior*, URpaiGoalBase*);
DECLARE_DELEGATE_TwoParams(FOnActionRemoved, URpaiComposerBehavior*, URpaiActionBase*);
DECLARE_DELEGATE_TwoParams(FOnGoalRemoved, URpaiComposerBehavior*, URpaiGoalBase*);
DECLARE_DELEGATE_TwoParams(FOnActionAdded, URpaiComposerBehavior*, URpaiActionBase*);
DECLARE_DELEGATE_TwoParams(FOnGoalAdded, URpaiComposerBehavior*, URpaiGoalBase*);
DECLARE_DELEGATE_TwoParams(FOnConstructedClassTypeChanged, URpaiComposerBehavior*, TSubclassOf<URpaiState>);

class SComposerBehaviorWidget : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SComposerBehaviorWidget)
            : _ComposerBehavior(nullptr)
            {}
            SLATE_ATTRIBUTE(URpaiComposerBehavior*, ComposerBehavior)
            SLATE_EVENT(FOnConstructedClassTypeChanged, OnStateTypeChanged)
    SLATE_END_ARGS()
    
    virtual void Construct(const FArguments& InArgs);
    virtual void AddReferencedObjects( FReferenceCollector& Collector );
        
private:
    TAttribute<URpaiComposerBehavior*> ComposerBehavior;
    
    URpaiState* TestStartingState;
    URpaiState* TestDesiredState;
    
    TSharedPtr<IPropertyHandle> ConstructedStateTypePropertyHandle;
    
    TSharedPtr<IDetailsView> TestStartingStateDetailView;
    TSharedPtr<IDetailsView> TestDesiredStateDetailView;
        
    void OnStateTypePropertyChanged();
};
