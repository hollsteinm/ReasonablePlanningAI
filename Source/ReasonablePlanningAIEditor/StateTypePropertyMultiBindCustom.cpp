// Troll Purse. All rights reserved.

#include "StateTypePropertyMultiBindCustom.h"
#include "PropertyEditing.h"
#include "ClassViewerFilter.h"
#include "ClassViewerModule.h"
#include "StructViewerModule.h"
#include "StructViewerFilter.h"
#include "PropertyCustomizationHelpers.h"
#include "Core/RpaiTypes.h"

#define LOCTEXT_NAMESPACE "ReasonablePlanningAIEditor"

class FClassStructViewerFilter : public IClassViewerFilter, public IStructViewerFilter
{
public:
	static constexpr EClassFlags DisallowedClassFlags = CLASS_Deprecated | CLASS_Abstract;

	//~ Begin IClassViewerFilter
	virtual bool IsClassAllowed(const FClassViewerInitializationOptions&, const UClass* InClass, TSharedRef<FClassViewerFilterFuncs>) override
	{
		return InClass
			&& InClass->IsChildOf<AActor>()
			&& !InClass->HasAnyClassFlags(DisallowedClassFlags);
	}
	virtual bool IsUnloadedClassAllowed(const FClassViewerInitializationOptions&, const TSharedRef<const IUnloadedBlueprintData> InUnloadedClassData, TSharedRef<FClassViewerFilterFuncs>) override
	{
		return InUnloadedClassData->IsChildOf(AActor::StaticClass())
			&& !InUnloadedClassData->HasAnyClassFlags(DisallowedClassFlags);
	}
	//~ End IClassViewerFilter

	//~ Begin IStructViewerFilter
	virtual bool IsStructAllowed(const FStructViewerInitializationOptions& InInitOptions, const UScriptStruct* InStruct, TSharedRef<class FStructViewerFilterFuncs> InFilterFuncs) override
	{
		return true;
	}

	virtual bool IsUnloadedStructAllowed(const FStructViewerInitializationOptions& InInitOptions, const FSoftObjectPath& InStructPath, TSharedRef<class FStructViewerFilterFuncs> InFilterFuncs) override
	{
		return true;
	}
	//~ End IStructViewerFilter
};

TSharedRef<IPropertyTypeCustomization> StateTypePropertyMultiBindCustom::MakeInstance()
{
	return MakeShareable(new StateTypePropertyMultiBindCustom());
}

StateTypePropertyMultiBindCustom::StateTypePropertyMultiBindCustom()
{

}

void StateTypePropertyMultiBindCustom::CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	TSharedPtr<IPropertyHandle> TargetBindingClassHandle = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FRpaiStateTypePropertyMultiBind, TargetBindingClass));
	check(TargetBindingClassHandle.IsValid());

	FString ValueString;
	TargetBindingClassHandle->GetValueAsDisplayString(ValueString);

	FText ValueName = FText::FromString(ValueString);
	if (ValueName.EqualTo(FText::FromName(NAME_None)))
	{
		ValueName = LOCTEXT("Rpai_EmptyValue_Prompt_Class", "Select a Class or Struct");
	}

	HeaderRow
		.NameContent()
		[
			SNew(STextBlock)
				.Text(ValueName)
		]
		.ValueContent()
		[
			SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				[
					SAssignNew(ClassPickerComboButton, SComboButton)
						.ContentPadding(1)
						.OnGetMenuContent(this, &StateTypePropertyMultiBindCustom::GenerateClassPicker)
						.ButtonContent()
						[
							SNew(STextBlock)
								.Text(LOCTEXT("RpaiPickClass", "Actor"))
						]
				]
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Center)
				[
					SNew(STextBlock)
						.Margin(FMargin(8.f, 1.f))
						.Text(LOCTEXT("RpaiOrLower", "or"))
				]
				+ SHorizontalBox::Slot()
				[
					SAssignNew(StructPickerComboButton, SComboButton)
						.ContentPadding(1)
						.OnGetMenuContent(this, &StateTypePropertyMultiBindCustom::GenerateStructPicker)
						.ButtonContent()
						[
							SNew(STextBlock)
								.Text(LOCTEXT("RpaiPickStruct", "Struct"))
						]
				]
		];
}

void StateTypePropertyMultiBindCustom::CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	TSharedPtr<IPropertyHandle> BoundProps = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FRpaiStateTypePropertyMultiBind, BoundProperties));
	auto BoundPropertiesRow = StructBuilder.AddCustomRow(FText::GetEmpty())
		.NameContent()
		[
			BoundProps->CreatePropertyNameWidget()
		]
		.ValueContent()
		[
			BoundProps->CreatePropertyValueWidget()
		];
}

void StateTypePropertyMultiBindCustom::OnClassPicked(UClass* InClassPicked)
{
	ClassPickerComboButton->SetIsOpen(false);
	StructPickerComboButton->SetIsOpen(false);
}

void StateTypePropertyMultiBindCustom::OnStructPicked(const UScriptStruct* InStruct)
{
	ClassPickerComboButton->SetIsOpen(false);
	StructPickerComboButton->SetIsOpen(false);
}

TSharedRef<SWidget> StateTypePropertyMultiBindCustom::GenerateClassPicker()
{
	TSharedRef<FClassStructViewerFilter> ViewerFilter = MakeShared<FClassStructViewerFilter>();

	FClassViewerInitializationOptions ClassPickerOptions;
	ClassPickerOptions.ClassFilter = ViewerFilter;
	ClassPickerOptions.bShowBackgroundBorder = false;
	ClassPickerOptions.bShowUnloadedBlueprints = true;
	ClassPickerOptions.bShowNoneOption = true;

	FOnClassPicked OnClassPickedDelegate(FOnClassPicked::CreateSP(this, &StateTypePropertyMultiBindCustom::OnClassPicked));

	return FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer").CreateClassViewer(ClassPickerOptions, OnClassPickedDelegate);
}

TSharedRef<SWidget> StateTypePropertyMultiBindCustom::GenerateStructPicker()
{
	TSharedRef<FClassStructViewerFilter> ViewerFilter = MakeShared<FClassStructViewerFilter>();

	FStructViewerInitializationOptions StructPickerOptions;
	StructPickerOptions.StructFilter = ViewerFilter;
	StructPickerOptions.bShowUnloadedStructs = true;
	StructPickerOptions.bShowNoneOption = true;
	StructPickerOptions.bShowBackgroundBorder = false;

	FOnStructPicked OnStructPickedDelegate(FOnStructPicked::CreateSP(this, &StateTypePropertyMultiBindCustom::OnStructPicked));

	return FModuleManager::LoadModuleChecked<FStructViewerModule>("StructViewer").CreateStructViewer(StructPickerOptions, OnStructPickedDelegate);
}