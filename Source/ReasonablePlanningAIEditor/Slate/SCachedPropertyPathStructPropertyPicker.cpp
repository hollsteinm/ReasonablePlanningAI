#include "SCachedPropertyPathStructPropertyPicker.h"
#include "ISinglePropertyView.h"
#include "PropertyPathHelpers.h"
#include "Editor.h"
#include "Widgets/SBoxPanel.h"
#include "SlateCore.h"

#define LOCTEXT_NAMESPACE "ReasonablePlanningAIEditor"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SCachedPropertyPathStructPropertyPicker::Construct(const FArguments& InArgs)
{
	PickerClass = InArgs._PickerClass;
	OnPropertyPathPicked = InArgs._OnPropertyPathPicked;

	if (PickerClass.IsSet() || PickerClass.IsBound())
	{
		LazyBuildPropertyValueWidget();
		LazyInitializePropertyListValues();
		
		const FString& InitialValue = InArgs._InitialValue;
		if (!InitialValue.IsEmpty())
		{
			int32 FoundIndex = INDEX_NONE;
			int32 Count = CachedPropertyPaths.Num();
			for (int32 Idx = 0; Idx < Count; ++Idx)
			{
				const auto& Value = CachedPropertyPaths[Idx];
				if (Value.IsValid() && Value->Equals(InitialValue))
				{
					FoundIndex = Idx;
					break;
				}
			}

			if (FoundIndex != INDEX_NONE)
			{
				PropertyListView->SetSelection(CachedPropertyPaths[FoundIndex]);
			}
		}
	}
	
	CachedPickerClass = nullptr;
	ChildSlot
		[
			SAssignNew(ComboButton, SComboButton)
				.OnGetMenuContent(FOnGetContent::CreateSP(this, &SCachedPropertyPathStructPropertyPicker::GetPropertyPathDropdown))
				.ButtonContent()
				[
					SNew(STextBlock)
						.Text(TAttribute<FText>::Create(TAttribute<FText>::FGetter::CreateLambda([this]() -> FText {
							if (PropertyListView.IsValid() && PropertyListView->GetNumItemsSelected() > 0)
							{
								TArray<TSharedPtr<FString>> Selections = PropertyListView->GetSelectedItems();
								return FText::FromString(*(Selections[0].Get()));
							}
							else
							{
								return LOCTEXT("RpaiPickProperty", "Pick Property");
							}
					})))
				]
		];



	
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

static bool GetPropertyPaths(TFieldIterator<FProperty> Piter, TArray<FString>& OutPaths)
{
	if (Piter->IsA<FBoolProperty>())
	{
		OutPaths.Add(Piter->GetName());
	}
	else if (Piter->IsA<FClassProperty>())
	{
		OutPaths.Add(Piter->GetName());
	}
	else if (Piter->IsA<FEnumProperty>())
	{
		OutPaths.Add(Piter->GetName());
	}
	else if (Piter->IsA<FFloatProperty>())
	{
		OutPaths.Add(Piter->GetName());
	}
	else if (Piter->IsA<FDoubleProperty>())
	{
		OutPaths.Add(Piter->GetName());
	}
	else if (Piter->IsA<FIntProperty>())
	{
		OutPaths.Add(Piter->GetName());
	}
	else if (Piter->IsA<FNameProperty>())
	{
		OutPaths.Add(Piter->GetName());
	}
	else if (Piter->IsA<FStrProperty>())
	{
		OutPaths.Add(Piter->GetName());
	}
	else if (Piter->IsA<FStructProperty>())
	{
		if (auto StructProperty = CastField<FStructProperty>(*Piter))
		{
			if (StructProperty->Struct == TBaseStructure<FVector>::Get())
			{
				OutPaths.Add(Piter->GetName());
			}
			else if (StructProperty->Struct == TBaseStructure<FRotator>::Get())
			{
				OutPaths.Add(Piter->GetName());
			}
		}
	}
	return !OutPaths.IsEmpty();
}

void SCachedPropertyPathStructPropertyPicker::LazyBuildPropertyValueWidget()
{
	// Construct the List view if neccesary
	if (!PropertyListView.IsValid())
	{
		SAssignNew(PropertyListView, SListView<TSharedPtr<FString>>)
			.ListItemsSource(&CachedPropertyPaths)
			.SelectionMode(ESelectionMode::Single)
			.ReturnFocusToSelection(true)
			.OnSelectionChanged_Lambda([this](TSharedPtr<FString> Selection, ESelectInfo::Type SelectionType) -> void
				{
					if (Selection.IsValid() && SelectionType != ESelectInfo::OnNavigation && SelectionType != ESelectInfo::Direct)
					{
						OnPropertyPathPicked.ExecuteIfBound(*Selection.Get());
					}
				})
			.OnGenerateRow(SListView<TSharedPtr<FString>>::FOnGenerateRow::CreateLambda([this](TSharedPtr<FString> Value, const TSharedRef<STableViewBase> OwnerTable) -> TSharedRef<ITableRow>
				{
					return SNew(STableRow<TSharedPtr<FString>>, OwnerTable)
						[
							SNew(STextBlock)
								.Text(FText::FromString(*Value.Get()))
						];
				}));
	}
}

void SCachedPropertyPathStructPropertyPicker::LazyInitializePropertyListValues()
{
	UStruct* CurrentPickerClass = PickerClass.Get();
	if (CachedPickerClass == nullptr || CachedPickerClass != CurrentPickerClass || CachedPropertyPaths.IsEmpty())
	{

		// Rebuild cached content
		CachedPropertyPaths.Empty();

		// Regenerate the list content
		TArray<FString> Properties;
		for (TFieldIterator<FProperty> Piter(CurrentPickerClass, EFieldIterationFlags::IncludeDeprecated | EFieldIterationFlags::IncludeSuper); Piter; ++Piter)
		{
			if (GetPropertyPaths(Piter, Properties))
			{
				for (const auto& Property : Properties)
				{
					TSharedPtr<FString> Str = MakeShared<FString>(Property);
					CachedPropertyPaths.Add(Str);
				}
				Properties.Empty();
			}
		}

		PropertyListView->RebuildList();
	}
}

TSharedRef<SWidget> SCachedPropertyPathStructPropertyPicker::GetPropertyPathDropdown()
{
	if (PickerClass.IsSet() || PickerClass.IsBound())
	{
		LazyBuildPropertyValueWidget();
		LazyInitializePropertyListValues();
		return PropertyListView.ToSharedRef();
	}
	else
	{
		return SNullWidget::NullWidget;
	}
}