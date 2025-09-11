// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/GridSlots/Inv_EquippedGridSlot.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "InventoryManagement/Inv_InventoryStatics.h"
#include "Items/Inv_InventoryItem.h"
#include "Items/Fragments/Inv_FragmentTags.h"
#include "Widgets/Inventory/HoverItem/Inv_HoverItem.h"
#include "Widgets/Inventory/SlottedItems/Inv_EquippedSlottedItem.h"

void UInv_EquippedGridSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	if (!IsAvailable()) return;

	UInv_HoverItem* HoverItem = UInv_InventoryStatics::GetHoveredItem(GetOwningPlayer());
	if (!HoverItem) return;

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
	//判断HoverItem的Tag 和本框的Tag是否一致
	if (!EquipmentTag.MatchesTagExact(EquipmentTypeTag)) return nullptr;

	//计算即将在Slot中图标的draw size
	const FInv_GridFragment* GridFragment = GetFragment<FInv_GridFragment>(Item, FragmentTags::GridFragment);
	if (!GridFragment) return nullptr;
	const FIntPoint GridDimensions  = GridFragment->GetGridSize();
	
	const float IconTileWidth  = TileSize - (2 * GridFragment->GetGridPadding());
	const FVector2D DrawSize = GridDimensions * IconTileWidth;

	//生成EquippedSlottedItem
	EquippedSlottedItem = CreateWidget<UInv_EquippedSlottedItem>(GetOwningPlayer(), EquippedGridSlotClass);

	// Set the Slotted Item's Inventory Item
	EquippedSlottedItem->SetInventoryItem(Item);
	
	// Set the Slotted Item's Equipment Type Tag
	EquippedSlottedItem->SetEquipmentTypeTag(EquipmentTag);
	
	// Hide the Stack Count widget on the Slotted Item
	EquippedSlottedItem->UpdateStackAmount(0);
	
	// Set Inventory Item on this class (the Equipped Grid Slot)
	SetInventoryItem(Item);
	
	// Set the Image Brush on the Equipped Slotted Item
	const FInv_ImageFragment* ImageFragment = GetFragment<FInv_ImageFragment>(Item, FragmentTags::IconFragment);
	if (!ImageFragment) return nullptr;

	FSlateBrush SlateBrush;
	SlateBrush.SetResourceObject(ImageFragment->GetIcon());
	SlateBrush.DrawAs = ESlateBrushDrawType::Image;
	SlateBrush.ImageSize = DrawSize;
	EquippedSlottedItem->SetImageBrush(SlateBrush);
	
	// Add the Slotted Item as a child to this widget's Overlay
	OverlayRoot->AddChild(EquippedSlottedItem);
	FGeometry OverlayGeometry = OverlayRoot->GetCachedGeometry();
	
	FVector2D OverlayPos = OverlayGeometry.GetAbsolutePosition();
	FVector2D OverlaySize = OverlayGeometry.GetAbsoluteSize();

	const float LeftPadding = OverlaySize.X/2.f - DrawSize.X/2.f;
	const float TopPadding = OverlaySize.Y/2.f - DrawSize.Y/2.f;

	UOverlaySlot* OverlaySlot = UWidgetLayoutLibrary::SlotAsOverlaySlot(EquippedSlottedItem);
	OverlaySlot->SetPadding(FMargin(LeftPadding, TopPadding));

	HideImageGrayedOutIcon(true);
	
	// Return the Equipped Slotted Item widget
	return EquippedSlottedItem;
}

void UInv_EquippedGridSlot::HideImageGrayedOutIcon(bool bHide)
{
	if (bHide) Image_GrayedOutIcon->SetVisibility(ESlateVisibility::Collapsed);
	else Image_GrayedOutIcon->SetVisibility(ESlateVisibility::Visible);
	
}
