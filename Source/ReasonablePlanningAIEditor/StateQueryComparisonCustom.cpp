// Radaway Software LLC. 2025. All rights reserved.

#include "StateQueryComparisonCustom.h"
#include "PropertyEditing.h"
#include "PropertyCustomizationHelpers.h"
#include "Core/RpaiTypes.h"

TSharedRef<IDetailCustomization> StateQueryComparisonCustom::MakeInstance(FName RHSFieldName)
{
    return MakeShareable<IDetailCustomization>(new StateQueryComparisonCustom(RHSFieldName));
}

TSharedRef<IPropertyTypeCustomization> StateQueryComparisonCustom::MakePropertyInstance(FName RHSFieldName)
{
   return MakeShareable<IPropertyTypeCustomization> (new StateQueryComparisonCustom(RHSFieldName));
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
   
   /**
   IDetailLayoutBuilder::GetProperty() will only return the immediate class properties found on scope. Which means parent class properties will
   not be found. Therefore we must search for them after getting the appropriate category's default properties.
   **/
   TArray<TSharedRef<IPropertyHandle>> DefaultProperties;
   RpaiCategory.GetDefaultProperties(DefaultProperties);

   const auto FindQueriedState = [](const TSharedRef<IPropertyHandle>& Handle) -> bool { return Handle->GetProperty()->GetFName().IsEqual("QueriedState"); };
   const auto FindComparison = [](const TSharedRef<IPropertyHandle>& Handle) -> bool { return Handle->GetProperty()->GetFName().IsEqual("ComparisonOperation"); };

   auto QueriedStateResult = DefaultProperties.FindByPredicate(FindQueriedState);
   auto ComparisonOperationResult = DefaultProperties.FindByPredicate(FindComparison);

   check(QueriedStateResult != nullptr);
   check(ComparisonOperationResult != nullptr);

   TSharedRef<IPropertyHandle> QueriedState = *QueriedStateResult;
   TSharedRef<IPropertyHandle> ComparisonOperation = *ComparisonOperationResult;

   QueriedState->MarkHiddenByCustomization();
   ComparisonOperation->MarkHiddenByCustomization();

   TSharedPtr<SHorizontalBox> Box;
   
   FDetailWidgetRow& Row = RpaiCategory.AddCustomRow(FText::GetEmpty())
      .NameContent()
      [
         SNew(STextBlock)
            .Font(IDetailLayoutBuilder::GetDetailFont())
            .Text(NSLOCTEXT("Rpai", "ComparisonDisplayName", "Query Definition"))
      ]
      .ValueContent()
      .HAlign(HAlign_Fill)
      [
         SAssignNew(Box, SHorizontalBox)
            + SHorizontalBox::Slot()
            .AutoWidth()
            [
               SNew(STextBlock)
                  .Font(IDetailLayoutBuilder::GetDetailFont())
                  .Text_Lambda([QueriedState]() -> FText
                     {
                        TSharedRef<IPropertyHandle> StateNamePropertyHandle = QueriedState->GetChildHandle(GET_MEMBER_NAME_CHECKED(FStateKeyValueReference, StateKeyName)).ToSharedRef();
                        FText StateNameText;
                        StateNamePropertyHandle->GetValueAsDisplayText(StateNameText);
                        return StateNameText;
                     })
            ]
            + SHorizontalBox::Slot()
            .AutoWidth()
            [
               SNew(STextBlock)
                  .Font(IDetailLayoutBuilder::GetDetailFont())
                  .Margin(FMargin(8., 0., 0., 0.))
                  .Text(NSLOCTEXT("Rpai", "AsUpper", "AS"))
            ]
            + SHorizontalBox::Slot()
            .AutoWidth()
            [
               SNew(STextBlock)
                  .Font(IDetailLayoutBuilder::GetDetailFont())
                  .Margin(FMargin(8., 0., 0., 0.))
                  .Text_Lambda([QueriedState]() -> FText
                     {
                        TSharedRef<IPropertyHandle> StateTypePropertyHandle = QueriedState->GetChildHandle(GET_MEMBER_NAME_CHECKED(FStateKeyValueReference, ExpectedValueType)).ToSharedRef();
                        FText StateTypeText;
                        StateTypePropertyHandle->GetValueAsDisplayText(StateTypeText);
                        return StateTypeText;
                     })
            ]
            + SHorizontalBox::Slot()
            .AutoWidth()
            [
               SNew(STextBlock)
                  .Font(IDetailLayoutBuilder::GetDetailFont())
                  .Margin(FMargin(8., 0., 0., 0.))
                  .Text_Lambda([ComparisonOperation]() -> FText
                     {
                        FText OperationText;
                        ComparisonOperation->GetValueAsDisplayText(OperationText);
                        return OperationText;
                     })
            ]
      ];

   RpaiCategory.AddProperty(QueriedState.ToSharedPtr());
   RpaiCategory.AddProperty(ComparisonOperation.ToSharedPtr());

   if (RHSFieldName.IsValid())
   {
      TSharedPtr<IPropertyHandle> Rhs = DetailBuilder.GetProperty(RHSFieldName);
      check(Rhs.IsValid());
      Rhs->MarkHiddenByCustomization();

      Box->AddSlot()
      .AutoWidth()
      [
         SNew(STextBlock)
            .Font(IDetailLayoutBuilder::GetDetailFont())
            .Margin(FMargin(8., 0., 0., 0.))
            .Text_Lambda([Value = Rhs.ToSharedRef()]() -> FText
               {
                  FText FieldNameText;
                  Value->GetValueAsDisplayText(FieldNameText);
                  return FieldNameText;
               })
      ];

      RpaiCategory.AddProperty(Rhs);
   }
}