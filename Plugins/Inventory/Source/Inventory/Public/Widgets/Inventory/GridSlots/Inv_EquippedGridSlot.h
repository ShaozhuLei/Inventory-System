﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Inv_GridSlot.h"
#include "Inv_EquippedGridSlot.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEquippedGridSlotClicked, UInv_EquippedGridSlot*, EquippedGridSlot, const FGameplayTag&, EquipmentTypeTag)

/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_EquippedGridSlot : public UInv_GridSlot
{
	GENERATED_BODY()
public:
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	FEquippedGridSlotClicked EquippedGridSlotClicked;

private:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	FGameplayTag EquipmentTypeTag;
};
