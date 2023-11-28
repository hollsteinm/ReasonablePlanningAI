#pragma once
 
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Composer/RpaiComposerBehavior.h"

class SComposerBehaviorWidget : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SComposerBehaviorWidget)
            : _ComposerBehavior(nullptr)
            {}
            SLATE_ATTRIBUTE(URpaiComposerBehavior*, ComposerBehavior)
    SLATE_END_ARGS()
    
    virtual void Construct(const FArguments& InArgs);
    virtual void Tick(const FGeometry & AllottedGeometry, const double InCurrentTime, const float InDeltaTime);
    virtual void AddReferencedObjects( FReferenceCollector& Collector );
    
    void NotifyStateTypePropertyChanged();
        
private:
    TAttribute<URpaiComposerBehavior*> ComposerBehavior;
    
    URpaiState* TestStartingState;
        
    TSharedPtr<IDetailsView> TestStartingStateDetailView;
    
    bool bIsExperimenting;
    ERpaiPlannerResult LastPlanResult;
    FRpaiMemoryStruct CurrentPlannerMemory;
    FRpaiMemory ComponentActionMemory;
    URpaiGoalBase* CurrentGoal;
            
    EVisibility GetSetStateMessageVisibility() const;
    bool IsEvaluateButtonEnabled() const;
    FReply OnEvaluateGoalAndPlan();
};
