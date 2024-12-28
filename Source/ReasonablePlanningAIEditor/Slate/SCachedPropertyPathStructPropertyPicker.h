#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Core/RpaiGoalBase.h"

DECLARE_DELEGATE_OneParam(FOnPropertyPathPicked, FString)

class SCachedPropertyPathStructPropertyPicker : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SCachedPropertyPathStructPropertyPicker)
		: _PickerClass(nullptr)
		, _OnPropertyPathPicked()
		{}
		SLATE_ATTRIBUTE(UStruct*, PickerClass)
		SLATE_EVENT(FOnPropertyPathPicked, OnPropertyPathPicked)
	SLATE_END_ARGS()

	virtual void Construct(const FArguments& InArgs);

private:
	// Slate bound
	TAttribute<UStruct*> PickerClass;
	FOnPropertyPathPicked OnPropertyPathPicked;

	TSharedPtr<SListView<TSharedPtr<FString>>> PropertyListView;
	TSharedRef<SWidget> GetPropertyPathDropdown();

	UStruct* CachedPickerClass;
	TArray<TSharedPtr<FString>> CachedPropertyPaths;
};
