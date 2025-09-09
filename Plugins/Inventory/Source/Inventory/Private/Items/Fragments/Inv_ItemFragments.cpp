// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Fragments/Inv_ItemFragments.h"

#include "Widgets/Composite/Inv_CompositeBase.h"
#include "Widgets/Composite/Inv_Leaf_Image.h"
#include "Widgets/Composite/Inv_Leaf_LabeledValue.h"
#include "Widgets/Composite/Inv_Leaf_Text.h"

void FInv_InventoryItemFragment::Assimilate(UInv_CompositeBase* Composite) const
{
	if (!MatchesWidgetTag(Composite)) return;
	Composite->Expand();
}

bool FInv_InventoryItemFragment::MatchesWidgetTag(UInv_CompositeBase* Composite) const
{
	return Composite->GetFragmentTag() == GetFragmentTag();
}

void FInv_LabeledNumberFragment::Assimilate(UInv_CompositeBase* Composite) const
{
	FInv_InventoryItemFragment::Assimilate(Composite);
	if (!MatchesWidgetTag(Composite)) return;

	UInv_Leaf_LabeledValue* LabeledValue = Cast<UInv_Leaf_LabeledValue>(Composite);
	if (!IsValid(LabeledValue)) return;

	LabeledValue->SetText_Label(Text_Label, bCollapseLabel);
	FNumberFormattingOptions Options;
	Options.MinimumFractionalDigits = MinFractionalDigits;
	Options.MaximumFractionalDigits = MaxFractionalDigits;
	LabeledValue->SetText_Value(FText::AsNumber(Value, &Options), bCollapseValue);
}

void FInv_LabeledNumberFragment::Manifest()
{
	FInv_InventoryItemFragment::Manifest();
	if (bRandomizeOnManifest)
	{
		Value = FMath::RandRange(Min, Max);
	}
	bRandomizeOnManifest = false;
}


void FInv_ConsumableFragment::OnConsume(APlayerController* PC)
{
	for (auto& Modifier : ConsumeModifiers)
	{
		auto& ModRef = Modifier.GetMutable();
		ModRef.OnConsume(PC);
	}
}

void FInv_ConsumableFragment::Assimilate(UInv_CompositeBase* Composite) const
{
	FInv_InventoryItemFragment::Assimilate(Composite);
	for (const auto& Modifier : ConsumeModifiers)
	{
		const auto& ModRef = Modifier.Get();
		ModRef.Assimilate(Composite);
	}
}

void FInv_ConsumableFragment::Manifest()
{
	FInv_InventoryItemFragment::Manifest();
	for (auto& Modifier : ConsumeModifiers)
	{
		auto& ModRef = Modifier.GetMutable();
		ModRef.Manifest();
	}
}

void FInv_HealthPotionFragment::OnConsume(APlayerController* PC)
{
	GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, FString::Printf(TEXT("Healing %f"), GetValue()));
}

void FInv_ManaPotionFragment::OnConsume(APlayerController* PC)
{
	GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Blue, FString::Printf(TEXT("Healing %f"), GetValue()));
}

void FInv_EquipmentFragment::Equip(APlayerController* PC)
{
	//GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Blue,  TEXT("Cloak Equipped!"));
	if (bEquipped) return;
	bEquipped = true;
	
	for (auto& Modifier: EquipModifiers)
	{
		auto& EquipModifier = Modifier.GetMutable();
		EquipModifier.Equip(PC);
	}
}

void FInv_EquipmentFragment::UnEquip(APlayerController* PC)
{
	//GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Blue,  TEXT("Cloak Unequipped!"));
	if (!bEquipped) return;
	bEquipped = false;
	
	for (auto& Modifier: EquipModifiers)
	{
		auto& EquipModifier = Modifier.GetMutable();
		EquipModifier.UnEquip(PC);
	}
}

void FInv_EquipmentFragment::Assimilate(UInv_CompositeBase* Composite) const
{
	FInv_InventoryItemFragment::Assimilate(Composite);
	for (const auto& Modifier: EquipModifiers)
	{
		auto& ModRef = Modifier.Get();
		ModRef.Assimilate(Composite);
	}
}

void FInv_StrengthModifier::Equip(APlayerController* PC)
{
	GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red,  TEXT("Cloak Equipped!"));
}

void FInv_StrengthModifier::UnEquip(APlayerController* PC)
{
	GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Blue,  TEXT("Cloak UnEquipped!"));
}

void FInv_ImageFragment::Assimilate(UInv_CompositeBase* Composite) const
{
	FInv_InventoryItemFragment::Assimilate(Composite);
	if (!MatchesWidgetTag(Composite)) return;

	UInv_Leaf_Image* Leaf_Image = Cast<UInv_Leaf_Image>(Composite);
	if (!IsValid(Leaf_Image)) return;
	
	Leaf_Image->SetImage(Icon);
	Leaf_Image->SetBoxSize(IconDimensions);
	Leaf_Image->SetImageSize(IconDimensions);
	
}

void FInv_TextFragment::Assimilate(UInv_CompositeBase* Composite) const
{
	FInv_InventoryItemFragment::Assimilate(Composite);
	if (!MatchesWidgetTag(Composite)) return;

	UInv_Leaf_Text* Leaf_Text = Cast<UInv_Leaf_Text>(Composite);
	if (!IsValid(Leaf_Text)) return;

	Leaf_Text->SetText(FragmentText);
}

