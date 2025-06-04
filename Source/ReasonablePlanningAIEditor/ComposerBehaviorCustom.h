// Radaway Software LLC. 2025. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

/**
 * Provides customizations for composer behaviors. Makes coupled systems - such as state references and the state subclass
 * more integrated and enables validation of references.
 */
class REASONABLEPLANNINGAIEDITOR_API ComposerBehaviorCustom : public IDetailCustomization
{
public:

   virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

   static TSharedRef<IDetailCustomization> MakeInstance();

private:
   TArray<TSharedPtr<FText>> Warnings;

   // Simple state management
   FDelegateHandle OnSourcePropertyElementsChanged;
   FDelegateHandle OnTargetPropertyElementsChanged;

   /**
   * Just a validation workflow, do not prevent developers from doing what they want but let
   * them know that it may not work as intended.
   **/
   void OnBindingPropertyElementsChanged(TSharedPtr<IPropertyHandleArray> ChangedArray, TSharedPtr<IPropertyHandleArray> CoupledArray);
};
