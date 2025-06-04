// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.
#pragma once

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
#include "Slate/SStateTypePropertyMultibind.h"
#include "AIController.h"
#define LOCTEXT_NAMESPACE "ReasonablePlanningAIEditor"

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
      ValueName = LOCTEXT("Rpai_EmptyValue_Prompt_Class", "Select a Class");
   }

   HeaderRow
      .NameContent()
      [
         SNew(STextBlock)
            .Text(ValueName)
      ]
      .ValueContent()
      [
         SNew(SStateTypePropertyMultibind)
            .StructPropertyHandle(StructPropertyHandle)
            .AllowedClasses({ URpaiState::StaticClass(), AAIController::StaticClass(), APawn::StaticClass() })
      ];
}

static FString GetBoundPropertyName (TSharedPtr<IPropertyHandle> Element)
{
   void* OutValue = nullptr;
   if (Element->GetValueData (OutValue) == FPropertyAccess::Success)
   {
      FCachedPropertyPath* PropertyPath = static_cast<FCachedPropertyPath*>(OutValue);
      return PropertyPath->ToString ();
   }
   return FString ();
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
                        .InitialPath(GetBoundPropertyName(Element))
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
