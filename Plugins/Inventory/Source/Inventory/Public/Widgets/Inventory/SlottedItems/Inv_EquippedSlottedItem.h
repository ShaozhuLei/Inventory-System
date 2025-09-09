// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Inv_SlottedItem.h"
#include "Inv_EquippedSlottedItem.generated.h"


struct FGameplayTag;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEquippedSlottedItemClicked, UInv_EquippedSlottedItem*, EquippedSlottedItem)
;

/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_EquippedSlottedItem : public UInv_SlottedItem
{
	GENERATED_BODY()
public:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	FORCEINLINE void SetEquipmentTypeTag(const FGameplayTag& InTag){EquipmentTypeTag = InTag;};
	FORCEINLINE FGameplayTag GetEquipmentTypeTag() const {return EquipmentTypeTag;}

	FEquippedSlottedItemClicked OnEquippedSlottedItemClicked;

private:
	UPROPERTY()
	FGameplayTag EquipmentTypeTag;
	
};
