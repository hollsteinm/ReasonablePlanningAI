// Troll Purse. All rights reserved.


#include "ComposerStateQueryCustom.h"
#include "PropertyEditing.h"
#include "PropertyCustomizationHelpers.h"
#include "ISinglePropertyView.h"
#include "IPropertyUtilities.h"

TMap<FName, TSharedRef<IPropertyTypeCustomization>> ComposerStateQueryCustom::Customizations;

TSharedRef<IPropertyTypeCustomization> ComposerStateQueryCustom::MakeInstance()
{
	return MakeShareable(new ComposerStateQueryCustom());
}

void ComposerStateQueryCustom::AddChildCustomization(const FName Name, TSharedRef<IPropertyTypeCustomization> Customization)
{
    Customizations.Add(Name, Customization);
}

void ComposerStateQueryCustom::CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
    UObject* Value = nullptr;
    bool bFallback = false;
    if (StructPropertyHandle->GetValue(Value) == FPropertyAccess::Success)
    {
        if (Value == nullptr)
        {
            return;
        }

        FName ActualClassName = Value->GetClass()->GetFName();
        TSharedRef<IPropertyTypeCustomization>* Customization = Customizations.Find(ActualClassName);
        if (Customization)
        {
            (*Customization)->CustomizeChildren(StructPropertyHandle, StructBuilder, StructCustomizationUtils);
        }
        else
        {
            FAddPropertyParams Params;
            Params.AllowChildren(true)
                .HideRootObjectNode(true)
                .UniqueId(StructPropertyHandle->GetProperty()->GetID())
                ;
            StructBuilder.AddExternalObjects(TArray<UObject*> { Value }, Params);
        }
    }

}

void ComposerStateQueryCustom::CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
    UObject* Value = nullptr;
    bool bFallback = false;
    if (StructPropertyHandle->GetValue(Value) == FPropertyAccess::Success)
    {
        if (Value == nullptr)
        {
            goto fallback;
        }

        FName ActualClassName = Value->GetClass()->GetFName();
        TSharedRef<IPropertyTypeCustomization>* Customization = Customizations.Find(ActualClassName);
        if (Customization)
        {
            (*Customization)->CustomizeHeader(StructPropertyHandle, HeaderRow, StructCustomizationUtils);
        }
        else
        {
            goto fallback;
        }
    }
    else
    {
        goto fallback;
    }

    fallback:
        HeaderRow
            .NameContent()
            [
                StructPropertyHandle->CreatePropertyNameWidget()
            ]
            .ValueContent()
            [
                StructPropertyHandle->CreatePropertyValueWidget()
            ];
}

