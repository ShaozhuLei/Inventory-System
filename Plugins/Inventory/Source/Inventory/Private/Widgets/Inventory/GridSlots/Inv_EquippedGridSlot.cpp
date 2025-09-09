// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/GridSlots/Inv_EquippedGridSlot.h"

#include "InventoryManagement/Inv_InventoryStatics.h"
#include "Widgets/Inventory/HoverItem/Inv_HoverItem.h"

void UInv_EquippedGridSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	if (!IsAvailable()) return;

	UInv_HoverItem* HoverItem = UInv_InventoryStatics::GetHoveredItem(GetOwningPlayer());
	if (!HoverItem) return;

	// if (HoverItem->GetItemType().MatchesTagExact(EquipmentTypeTag))
	// {
	// 	SetOccupiedTexture();
	// 	Image_GrayedOutIcon->SetVisibility(ESlateVisibility::Visible);
	// }

	if (HoverItem->GetItemType().MatchesTag(EquipmentTypeTag))
	{
		SetOccupiedTexture();
		Image_GrayedOutIcon->SetVisibility(ESlateVisibility::Visible);
	}
}

void UInv_EquippedGridSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	if (!IsAvailable()) return;

	UInv_HoverItem* HoverItem = UInv_InventoryStatics::GetHoveredItem(GetOwningPlayer());
	if (!HoverItem) return;

	if (HoverItem->GetItemType().MatchesTagExact(EquipmentTypeTag))
	{
		SetUnoccupiedTexture();
		Image_GrayedOutIcon->SetVisibility(ESlateVisibility::Collapsed);
	}
	
}

FReply UInv_EquippedGridSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	EquippedGridSlotClicked.Broadcast(this, EquipmentTypeTag);
	return FReply::Handled();
}

UInv_EquippedSlottedItem* UInv_EquippedGridSlot::OnItemEquipped(UInv_InventoryItem* Item,
	const FGameplayTag& EquipmentTag, float TileSize)
{
	return nullptr;
}
