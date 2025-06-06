// Copyright (C) 2025 Radaway Software LLC. All Rights Reserved.

#include "StateKeyValueReferenceCustom.h"
#include "Core/RpaiTypes.h"
#include "PropertyEditing.h"
#include "PropertyCustomizationHelpers.h"

TSharedRef<IPropertyTypeCustomization> StateKeyValueReferenceCustom::MakeInstance()
{
    return MakeShareable(new StateKeyValueReferenceCustom);
}

void StateKeyValueReferenceCustom::CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
   TSharedPtr<IPropertyHandle> StateNamePropertyHandle = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FStateKeyValueReference, StateKeyName));
   TSharedPtr<IPropertyHandle> StateTypePropertyHandle = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FStateKeyValueReference, ExpectedValueType));
   check(StateNamePropertyHandle.IsValid());
   check(StateTypePropertyHandle.IsValid());

   HeaderRow
   .NameContent()
      [
         StructPropertyHandle->CreatePropertyNameWidget()
      ]
   .ValueContent()
   .HAlign(EHorizontalAlignment::HAlign_Fill)
      [
         SNew(SHorizontalBox)
         + SHorizontalBox::Slot()
         .FillWidth(1)
         .MaxWidth(64.0f)
         [
            StateNamePropertyHandle->CreatePropertyValueWidget()
         ]
         + SHorizontalBox::Slot()
         .AutoWidth()
         .Padding(4.0f, 0.0f)
         [
            StateTypePropertyHandle->CreatePropertyValueWidget()
         ]
      ];
}

void StateKeyValueReferenceCustom::CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
   //Noop
}