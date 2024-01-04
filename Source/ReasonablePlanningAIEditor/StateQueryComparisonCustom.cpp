// Troll Purse. All rights reserved.

#include "StateQueryComparisonCustom.h"
#include "PropertyEditing.h"
#include "PropertyCustomizationHelpers.h"

TSharedRef<IDetailCustomization> StateQueryComparisonCustom::MakeInstance(FName RHSFieldName)
{
    return MakeShareable(new StateQueryComparisonCustom(RHSFieldName));
}

TSharedRef<IPropertyTypeCustomization> StateQueryComparisonCustom::MakePropertyInstance(FName RHSFieldName)
{
	return MakeShareable(new StateQueryComparisonCustom(RHSFieldName));
}

StateQueryComparisonCustom::StateQueryComparisonCustom(FName RHSFieldName)
	: RHSFieldName(RHSFieldName)
{

}

void StateQueryComparisonCustom::CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
}

void StateQueryComparisonCustom::CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	TSharedPtr<IPropertyHandle> QueriedState = StructPropertyHandle->GetChildHandle(FName(TEXT("QueriedState")));
	TSharedPtr<IPropertyHandle> ComparisonOperation = StructPropertyHandle->GetChildHandle(FName(TEXT("ComparisonOperation")));
	StructBuilder.GetParentCategory().GetParentLayout().HideProperty(QueriedState);
	StructBuilder.GetParentCategory().GetParentLayout().HideProperty(ComparisonOperation);

	TSharedPtr<SHorizontalBox> Box;

	StructBuilder.AddCustomRow(FText::GetEmpty())
		.NameContent()
		[
			SNew(STextBlock)
				.Font(IDetailLayoutBuilder::GetDetailFont())
				.Text(NSLOCTEXT("Rpai", "ComparisonDisplayName", "Comparison"))
		]
		.ValueContent()
		.HAlign(EHorizontalAlignment::HAlign_Fill)
		[
			SAssignNew(Box, SHorizontalBox)
				+ SHorizontalBox::Slot()
				.HAlign(EHorizontalAlignment::HAlign_Fill)
				[
					StructBuilder.GenerateStructValueWidget(QueriedState.ToSharedRef())
				]
				+ SHorizontalBox::Slot()
				.HAlign(EHorizontalAlignment::HAlign_Fill)
				[
					ComparisonOperation->CreatePropertyValueWidget()
				]
		];

	if (RHSFieldName.IsValid())
	{
		TSharedPtr<IPropertyHandle> Rhs = StructPropertyHandle->GetChildHandle(RHSFieldName);
		check(Rhs.IsValid());
		StructBuilder.GetParentCategory().GetParentLayout().HideProperty(Rhs);
		Box->AddSlot()
			[
				Rhs->CreatePropertyValueWidget()
			];
	}
}

void StateQueryComparisonCustom::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	IDetailCategoryBuilder& RpaiCategory = DetailBuilder.EditCategory("Rpai", FText::GetEmpty(), ECategoryPriority::Important);
	TSharedPtr<IPropertyHandle> QueriedState = DetailBuilder.GetProperty(FName(TEXT("QueriedState")));
	TSharedPtr<IPropertyHandle> ComparisonOperation = DetailBuilder.GetProperty(FName(TEXT("ComparisonOperation")));

	check(QueriedState.IsValid());
	check(ComparisonOperation.IsValid());

	DetailBuilder.HideProperty(QueriedState);
	DetailBuilder.HideProperty(ComparisonOperation);

	TSharedPtr<SHorizontalBox> Box;
	
	RpaiCategory.AddCustomRow(FText::GetEmpty())
	.NameContent()
	[
		SNew(STextBlock)
		.Font(IDetailLayoutBuilder::GetDetailFont())
		.Text(NSLOCTEXT("Rpai", "ComparisonDisplayName", "Comparison"))
	]
	.ValueContent()
		[
			SAssignNew(Box, SHorizontalBox)
			+ SHorizontalBox::Slot()
			[
				QueriedState->CreatePropertyValueWidget()
			]
			+ SHorizontalBox::Slot()
			[
				ComparisonOperation->CreatePropertyValueWidget()
			]
		];

	if (RHSFieldName.IsValid())
	{
		TSharedPtr<IPropertyHandle> Rhs = DetailBuilder.GetProperty(RHSFieldName);
		check(Rhs.IsValid());
		DetailBuilder.HideProperty(Rhs);
		Box->AddSlot()
		[
			Rhs->CreatePropertyValueWidget()
		];
	}
}