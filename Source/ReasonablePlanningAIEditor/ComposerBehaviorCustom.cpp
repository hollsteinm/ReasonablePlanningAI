// Radaway Software LLC. 2025. All rights reserved.

#include "ComposerBehaviorCustom.h"
#include "PropertyEditing.h"
#include "Core/RpaiTypes.h"
#include "PropertyCustomizationHelpers.h"
#include "Slate/SStateTypePropertyMultibind.h"
#include "AIController.h"

TSharedRef<IDetailCustomization> ComposerBehaviorCustom::MakeInstance()
{
   return MakeShareable(new ComposerBehaviorCustom());
}

void ComposerBehaviorCustom::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
   IDetailCategoryBuilder& RpaiCategory = DetailBuilder.EditCategory("Rpai", FText::GetEmpty(), ECategoryPriority::Important);

   auto BindingsProperty = DetailBuilder.GetProperty("Bindings");
   if (BindingsProperty->IsValidHandle())
   {
      DetailBuilder.HideProperty(BindingsProperty);
      auto SourceProperties = BindingsProperty->GetChildHandle(GET_MEMBER_NAME_CHECKED(FRpaiStateBinding, SourceBindings));
      auto TargetProperties = BindingsProperty->GetChildHandle(GET_MEMBER_NAME_CHECKED(FRpaiStateBinding, TargetBindings));

      check(SourceProperties.IsValid() && TargetProperties.IsValid());
      check(SourceProperties->IsValidHandle() && TargetProperties->IsValidHandle());
      
      auto SourcePropertiesArray = SourceProperties->AsArray();
      auto TargetPropertiesArray = TargetProperties->AsArray();

      check(SourcePropertiesArray.IsValid() && TargetPropertiesArray.IsValid());

      IDetailGroup& BindingGroup = RpaiCategory.AddGroup("Rpai.Bindings", NSLOCTEXT("Rpai", "RpaiBehaviorCustomBindings", "Bindings"));
      BindingGroup.AddWidgetRow()
         .WholeRowWidget
         [
            SNew(SListView<TSharedPtr<FText>>)
               .ListItemsSource(&Warnings)
               .OnGenerateRow(SListView<TSharedPtr<FText>>::FOnGenerateRow::CreateLambda([](TSharedPtr<FText> Item, const TSharedRef<STableViewBase>& Table) -> TSharedRef<ITableRow> {
               return SNew(STableRow<TSharedPtr<FText>>, Table)
                  [
                     SNew(STextBlock)
                        .Text(*Item)
                        .ColorAndOpacity(FSlateColor(EStyleColor::AccentYellow))
                  ];
               }))
         ];

      BindingGroup.AddPropertyRow(SourceProperties.ToSharedRef());
      BindingGroup.AddPropertyRow(TargetProperties.ToSharedRef());

      OnSourcePropertyElementsChanged = SourcePropertiesArray->SetOnNumElementsChanged(FSimpleDelegate::CreateSP(this, &ComposerBehaviorCustom::OnBindingPropertyElementsChanged, SourcePropertiesArray, TargetPropertiesArray));
      OnTargetPropertyElementsChanged = SourcePropertiesArray->SetOnNumElementsChanged(FSimpleDelegate::CreateSP(this, &ComposerBehaviorCustom::OnBindingPropertyElementsChanged, TargetPropertiesArray, SourcePropertiesArray));

      // The number of elements in each need to be equal as the binding is N (Source) M (Properties) mapped to N (Target) M (Properties).
      uint32 SourceElementCount = 0U;
      uint32 TargetElementCount = 0U;
      if (SourcePropertiesArray->GetNumElements(SourceElementCount) == FPropertyAccess::Success && TargetPropertiesArray->GetNumElements(TargetElementCount) == FPropertyAccess::Success)
      {
         if (SourceElementCount != TargetElementCount)
         {
            TSharedPtr<FText> Msg(new FText(NSLOCTEXT("Rpai", "CountWarning", "Source and Target Bindings should be have same number of elements.")));
            Warnings.Add(Msg);
         }

         for (uint32 Idx = 0U; Idx < SourceElementCount; ++Idx)
         {
            auto Property = SourcePropertiesArray->GetElement(Idx);
            // TODO: Set filter classes
         }
      }
   }
}

void ComposerBehaviorCustom::OnBindingPropertyElementsChanged(TSharedPtr<IPropertyHandleArray> ChangedArray, TSharedPtr<IPropertyHandleArray> CoupledArray)
{
   uint32 SourceElementCount = 0U;
   uint32 TargetElementCount = 0U;
   if (ChangedArray->GetNumElements(SourceElementCount) == FPropertyAccess::Success && CoupledArray->GetNumElements(TargetElementCount) == FPropertyAccess::Success)
   {
      if (SourceElementCount != TargetElementCount)
      {
         TSharedPtr<FText> Msg(new FText(NSLOCTEXT("Rpai", "CountWarning", "Source and Target Bindings should be have same number of elements.")));
         Warnings.Add(Msg);
      }
   }
}