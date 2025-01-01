// Troll Purse. All rights reserved.

#include "StateTypePropertyMultiBindCustom.h"
#include "PropertyEditing.h"
#include "ClassViewerFilter.h"
#include "ClassViewerModule.h"
#include "StructViewerModule.h"
#include "StructViewerFilter.h"
#include "PropertyCustomizationHelpers.h"
#include "Core/RpaiTypes.h"
#include "Core/RpaiState.h"
#include "Slate/SCachedPropertyPathStructPropertyPicker.h"
#include "AIController.h"
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

TSharedRef<IPropertyTypeCustomization> StateTypePropertyMultiBindCustom::MakeInstance()
{
	return MakeShareable(new StateTypePropertyMultiBindCustom());
}

StateTypePropertyMultiBindCustom::StateTypePropertyMultiBindCustom()
{

}

void StateTypePropertyMultiBindCustom::CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	TSharedPtr<IPropertyHandle> BoundProps = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FRpaiStateTypePropertyMultiBind, BoundProperties));
	TSharedPtr<IPropertyHandle> TargetBindingClassHandle = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FRpaiStateTypePropertyMultiBind, TargetBindingClass));
	check(TargetBindingClassHandle.IsValid());
	check(BoundProps.IsValid());

	FString ValueString;
	TargetBindingClassHandle->GetValueAsDisplayString(ValueString);

	FText ValueName = FText::FromString(ValueString);
	if (ValueName.EqualTo(FText::FromName(NAME_None)))
	{
		ValueName = LOCTEXT("Rpai_EmptyValue_Prompt_Class", "Select a Class or Struct");
	}

	TSharedPtr<IPropertyHandleArray> BoundPropsArray = BoundProps->AsArray();
	check(BoundPropsArray.IsValid());

	TSharedRef<SWidget> AddButton = PropertyCustomizationHelpers::MakeAddButton(
		FSimpleDelegate::CreateSP(this, &StateTypePropertyMultiBindCustom::AddBindingButton_OnClick, StructPropertyHandle, BoundPropsArray),
		TAttribute<FText>(this, &StateTypePropertyMultiBindCustom::GetAddNewBindingTooltip),
		TAttribute<bool>(this, &StateTypePropertyMultiBindCustom::CanAddNewBinding)
	);
	TSharedRef<SWidget> ClearButton = PropertyCustomizationHelpers::MakeClearButton(
		FSimpleDelegate::CreateSP(this, &StateTypePropertyMultiBindCustom::ClearBindingButton_OnClick, StructPropertyHandle, BoundPropsArray),
		TAttribute<FText>(this, &StateTypePropertyMultiBindCustom::GetClearBindingTooltip)
	);

	FUIAction CopyAction;
	FUIAction PasteAction;
	BoundProps->CreateDefaultPropertyCopyPasteActions(CopyAction, PasteAction);

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
						.OnGetMenuContent(this, &StateTypePropertyMultiBindCustom::GenerateClassPicker, StructPropertyHandle, TargetBindingClassHandle)
						.ButtonContent()
						[
							SNew(STextBlock)
								.Text(LOCTEXT("RpaiPickClass", "Actor"))
						]
				]
				//+ SHorizontalBox::Slot()
				//.HAlign(HAlign_Center)
				//[
				//	SNew(STextBlock)
				//		.Margin(FMargin(8.f, 1.f))
				//		.Text(LOCTEXT("RpaiOrLower", "or"))
				//]
				//+ SHorizontalBox::Slot()
				//[
				//	SAssignNew(StructPickerComboButton, SComboButton)
				//		.ContentPadding(1)
				//		.OnGetMenuContent(this, &StateTypePropertyMultiBindCustom::GenerateStructPicker, StructPropertyHandle, TargetBindingClassHandle)
				//		.ButtonContent()
				//		[
				//			SNew(STextBlock)
				//				.Text(LOCTEXT("RpaiPickStruct", "Struct"))
				//		]
				//]
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

void StateTypePropertyMultiBindCustom::CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	TSharedPtr<IPropertyHandle> BoundProps = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FRpaiStateTypePropertyMultiBind, BoundProperties));
	TSharedPtr<IPropertyHandle> PickedClassOrStruct = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FRpaiStateTypePropertyMultiBind, TargetBindingClass));
	void* ContainerAddress = nullptr;
	if (BoundProps->IsValidHandle() && PickedClassOrStruct->IsValidHandle() && PickedClassOrStruct->GetValueData(ContainerAddress) == FPropertyAccess::Success)
	{
		if (ContainerAddress != nullptr)
		{
			TSharedPtr<IPropertyHandleArray> BoundPropsArray = BoundProps->AsArray();
			check(BoundPropsArray.IsValid());

			uint32 TotalElements = 0U;
			if (BoundPropsArray->GetNumElements(TotalElements) == FPropertyAccess::Success)
			{
				for (uint32 Idx = 0U; Idx < TotalElements; ++Idx)
				{
					TSharedPtr<IPropertyHandle> Element = BoundPropsArray->GetElement(Idx);
					StructBuilder.AddProperty(Element.ToSharedRef())
						.CustomWidget()
						[
							SNew(SCachedPropertyPathStructPropertyPicker)
								.PickerClass((*static_cast<TObjectPtr<UStruct>*>(ContainerAddress)).Get())
								.InitialValue(GetBoundPropertyName(Element))
								.OnPropertyPathPicked_Lambda([Element](const FString& PropertyPath) -> void {
									if (Element.IsValid())
									{
										void* OutValue = nullptr;
										if (Element->GetValueData(OutValue) == FPropertyAccess::Success)
										{
											FCachedPropertyPath* PathObj = static_cast<FCachedPropertyPath*>(OutValue);
											PathObj->RemoveFromStart(PathObj->GetNumSegments());
											PathObj->MakeFromString(PropertyPath);
										}
									}
								})
						];
				}
			}
		}
	}
}

void StateTypePropertyMultiBindCustom::OnClassPicked(UClass* InClassPicked, TSharedRef<IPropertyHandle> Parent, TSharedPtr<IPropertyHandle> Property)
{
	// SetValue does not support UClass -> UStruct conversions even though they are correct C++ side.
	// Raw Ptr nonsense it is, don't break!
	void* ContainerAddress;
	if (Parent->GetValueData(ContainerAddress) == FPropertyAccess::Success && Property->IsValidHandle())
	{
		FRpaiStateTypePropertyMultiBind* Container = static_cast<FRpaiStateTypePropertyMultiBind*>(ContainerAddress);
		Property->NotifyPreChange();
		Container->TargetBindingClass = InClassPicked;
		Property->NotifyPostChange(EPropertyChangeType::ValueSet);
		Parent->RequestRebuildChildren();
	}
	ClassPickerComboButton->SetIsOpen(false);
	StructPickerComboButton->SetIsOpen(false);
}

void StateTypePropertyMultiBindCustom::OnStructPicked(const UScriptStruct* InStructPicked, TSharedRef<IPropertyHandle> Parent, TSharedPtr<IPropertyHandle> Property)
{
	if (Property->SetValue(InStructPicked) == FPropertyAccess::Success)
	{
		Parent->RequestRebuildChildren();
	}
	ClassPickerComboButton->SetIsOpen(false);
	StructPickerComboButton->SetIsOpen(false);
}

TSharedRef<SWidget> StateTypePropertyMultiBindCustom::GenerateClassPicker(TSharedRef<IPropertyHandle> Parent, TSharedPtr<IPropertyHandle> Property)
{
	static TArray<UClass*> AllowedClasses({ AAIController::StaticClass(), APawn::StaticClass(), URpaiState::StaticClass() });
	TSharedRef<FClassStructViewerFilter> ViewerFilter = MakeShared<FClassStructViewerFilter>(AllowedClasses);

	FClassViewerInitializationOptions ClassPickerOptions;
	ClassPickerOptions.ClassFilter = ViewerFilter;
	ClassPickerOptions.PropertyHandle = Property;
	ClassPickerOptions.bShowBackgroundBorder = false;
	ClassPickerOptions.bShowUnloadedBlueprints = true;
	ClassPickerOptions.bShowNoneOption = true;

	FOnClassPicked OnClassPickedDelegate(FOnClassPicked::CreateSP(this, &StateTypePropertyMultiBindCustom::OnClassPicked, Parent, Property));

	return FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer").CreateClassViewer(ClassPickerOptions, OnClassPickedDelegate);
}

TSharedRef<SWidget> StateTypePropertyMultiBindCustom::GenerateStructPicker(TSharedRef<IPropertyHandle> Parent, TSharedPtr<IPropertyHandle> Property)
{
	TSharedRef<FClassStructViewerFilter> ViewerFilter = MakeShared<FClassStructViewerFilter>();

	FStructViewerInitializationOptions StructPickerOptions;
	StructPickerOptions.StructFilter = ViewerFilter;
	StructPickerOptions.PropertyHandle = Property;
	StructPickerOptions.bShowUnloadedStructs = true;
	StructPickerOptions.bShowNoneOption = true;
	StructPickerOptions.bShowBackgroundBorder = false;

	FOnStructPicked OnStructPickedDelegate(FOnStructPicked::CreateSP(this, &StateTypePropertyMultiBindCustom::OnStructPicked, Parent, Property));

	return FModuleManager::LoadModuleChecked<FStructViewerModule>("StructViewer").CreateStructViewer(StructPickerOptions, OnStructPickedDelegate);
}

void StateTypePropertyMultiBindCustom::AddBindingButton_OnClick(TSharedRef<IPropertyHandle> Parent, TSharedPtr<IPropertyHandleArray> Property)
{
	FPropertyAccess::Result Result = Property->AddItem();
	if (Result == FPropertyAccess::Success)
	{
		Parent->RequestRebuildChildren();
	}
}

void StateTypePropertyMultiBindCustom::ClearBindingButton_OnClick(TSharedRef<IPropertyHandle> Parent, TSharedPtr<IPropertyHandleArray> Property)
{
	FPropertyAccess::Result Result = Property->EmptyArray();
	if (Result == FPropertyAccess::Success)
	{
		Parent->RequestRebuildChildren();
	}
}

FText StateTypePropertyMultiBindCustom::GetAddNewBindingTooltip() const
{
	return LOCTEXT("RpaiAddNewBindingButton", "Add New Binding");
}

bool StateTypePropertyMultiBindCustom::CanAddNewBinding() const
{
	return true;
}

FText StateTypePropertyMultiBindCustom::GetClearBindingTooltip() const
{
	return LOCTEXT("RpaiClearBindingsButton", "Clear Bindings");
}
