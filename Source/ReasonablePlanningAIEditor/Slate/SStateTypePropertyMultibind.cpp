#include "SStateTypePropertyMultibind.h"
#include "PropertyEditing.h"
#include "ClassViewerFilter.h"
#include "ClassViewerModule.h"
#include "StructViewerModule.h"
#include "StructViewerFilter.h"
#include "PropertyCustomizationHelpers.h"
#include "Core/RpaiTypes.h"
#include "SlateCore.h"

#define LOCTEXT_NAMESPACE "ReasonablePlanningAIEditor"

class FClassStructViewerFilter : public IClassViewerFilter, public IStructViewerFilter
{
private:
	TArray<UClass*> AllowedClasses;

public:
	static constexpr EClassFlags DisallowedClassFlags = CLASS_Deprecated | CLASS_Abstract;

	FClassStructViewerFilter()
		: AllowedClasses()
	{

	}

	FClassStructViewerFilter(const TArray<UClass*>& InAllowedClasses)
		: AllowedClasses(InAllowedClasses)
	{

	}

	//~ Begin IClassViewerFilter
	virtual bool IsClassAllowed(const FClassViewerInitializationOptions&, const UClass* InClass, TSharedRef<FClassViewerFilterFuncs>) override
	{
		if (InClass && !InClass->HasAnyClassFlags(DisallowedClassFlags))
		{
			for (const UClass* C : AllowedClasses)
			{
				if (InClass->IsChildOf(C))
				{
					return true;
				}
			}
			return false;
		}
		return false;
	}
	virtual bool IsUnloadedClassAllowed(const FClassViewerInitializationOptions&, const TSharedRef<const IUnloadedBlueprintData> InUnloadedClassData, TSharedRef<FClassViewerFilterFuncs>) override
	{
		if (!InUnloadedClassData->HasAnyClassFlags(DisallowedClassFlags))
		{
			for (const UClass* C : AllowedClasses)
			{
				if (InUnloadedClassData->IsChildOf(C))
				{
					return true;
				}
			}
			return false;
		}
		return false;
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

static FString GetBoundPropertyName(TSharedPtr<IPropertyHandle> Element)
{
	void* OutValue = nullptr;
	if (Element->GetValueData(OutValue) == FPropertyAccess::Success)
	{
		FCachedPropertyPath* PropertyPath = static_cast<FCachedPropertyPath*>(OutValue);
		return PropertyPath->ToString();
	}
	return FString();
}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SStateTypePropertyMultibind::Construct(const FArguments& InArgs)
{
	AllowedClasses = InArgs._AllowedClasses;
	StructPropertyHandle = InArgs._StructPropertyHandle;

	if (StructPropertyHandle.IsValid())
	{
		TSharedPtr<IPropertyHandle> BoundProps = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FRpaiStateTypePropertyMultiBind, BoundProperties));
		TSharedPtr<IPropertyHandle> TargetBindingClassHandle = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FRpaiStateTypePropertyMultiBind, TargetBindingClass));

		check(TargetBindingClassHandle.IsValid());
		check(BoundProps.IsValid());

		TSharedPtr<IPropertyHandleArray> BoundPropsArray = BoundProps->AsArray();
		check(BoundPropsArray.IsValid());

		TSharedRef<SWidget> AddButton = PropertyCustomizationHelpers::MakeAddButton(
			FSimpleDelegate::CreateSP(this, &SStateTypePropertyMultibind::AddBindingButton_OnClick, BoundPropsArray),
			TAttribute<FText>(this, &SStateTypePropertyMultibind::GetAddNewBindingTooltip),
			TAttribute<bool>(this, &SStateTypePropertyMultibind::CanAddNewBinding)
		);
		TSharedRef<SWidget> ClearButton = PropertyCustomizationHelpers::MakeClearButton(
			FSimpleDelegate::CreateSP(this, &SStateTypePropertyMultibind::ClearBindingButton_OnClick, BoundPropsArray),
			TAttribute<FText>(this, &SStateTypePropertyMultibind::GetClearBindingTooltip)
		);

		FUIAction CopyAction;
		FUIAction PasteAction;
		BoundProps->CreateDefaultPropertyCopyPasteActions(CopyAction, PasteAction);

		ChildSlot
			[
				SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					[
						SAssignNew(ClassPickerComboButton, SComboButton)
							.ContentPadding(1)
							.OnGetMenuContent(this, &SStateTypePropertyMultibind::GenerateClassPicker, TargetBindingClassHandle)
							.ButtonContent()
							[
								SNew(STextBlock)
									.Text(LOCTEXT("RpaiPickClass", "Actor"))
							]
					]
					+ SHorizontalBox::Slot()
					.Padding(2.0f)
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					.AutoWidth()
					[
						AddButton
					]
					+ SHorizontalBox::Slot()
					.Padding(2.0f)
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					.AutoWidth()
					[
						ClearButton
					]
			];
	}
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SStateTypePropertyMultibind::OnClassPicked(UClass* InClassPicked, TSharedPtr<IPropertyHandle> Property)
{
	// SetValue does not support UClass -> UStruct conversions even though they are correct C++ side.
	// Raw Ptr nonsense it is, don't break!
	void* ContainerAddress;
	if (StructPropertyHandle->GetValueData(ContainerAddress) == FPropertyAccess::Success && Property->IsValidHandle())
	{
		FRpaiStateTypePropertyMultiBind* Container = static_cast<FRpaiStateTypePropertyMultiBind*>(ContainerAddress);
		Property->NotifyPreChange();
		Container->TargetBindingClass = InClassPicked;
		Property->NotifyPostChange(EPropertyChangeType::ValueSet);
		StructPropertyHandle->RequestRebuildChildren();
	}
	ClassPickerComboButton->SetIsOpen(false);
}

TSharedRef<SWidget> SStateTypePropertyMultibind::GenerateClassPicker( TSharedPtr<IPropertyHandle> Property)
{
	TSharedRef<FClassStructViewerFilter> ViewerFilter = MakeShared<FClassStructViewerFilter>(AllowedClasses);

	FClassViewerInitializationOptions ClassPickerOptions;
	ClassPickerOptions.ClassFilter = ViewerFilter;
	ClassPickerOptions.PropertyHandle = Property;
	ClassPickerOptions.bShowBackgroundBorder = false;
	ClassPickerOptions.bShowUnloadedBlueprints = true;
	ClassPickerOptions.bShowNoneOption = true;

	FOnClassPicked OnClassPickedDelegate(FOnClassPicked::CreateSP(this, &SStateTypePropertyMultibind::OnClassPicked, Property));

	return FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer").CreateClassViewer(ClassPickerOptions, OnClassPickedDelegate);
}

void SStateTypePropertyMultibind::AddBindingButton_OnClick(TSharedPtr<IPropertyHandleArray> Property)
{
	FPropertyAccess::Result Result = Property->AddItem();
	if (Result == FPropertyAccess::Success)
	{
		StructPropertyHandle->RequestRebuildChildren();
	}
}

void SStateTypePropertyMultibind::ClearBindingButton_OnClick(TSharedPtr<IPropertyHandleArray> Property)
{
	FPropertyAccess::Result Result = Property->EmptyArray();
	if (Result == FPropertyAccess::Success)
	{
		StructPropertyHandle->RequestRebuildChildren();
	}
}

FText SStateTypePropertyMultibind::GetAddNewBindingTooltip() const
{
	return LOCTEXT("RpaiAddNewBindingButton", "Add New Binding");
}

bool SStateTypePropertyMultibind::CanAddNewBinding() const
{
	return true;
}

FText SStateTypePropertyMultibind::GetClearBindingTooltip() const
{
	return LOCTEXT("RpaiClearBindingsButton", "Clear Bindings");
}