// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inv_EquipmentComponent.generated.h"


class UInv_InventoryItem;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class INVENTORY_API UInv_EquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:

protected:
	virtual void BeginPlay() override;

private:
	TWeakObjectPtr<UInv_EquipmentComponent> EquipmentComponent;
	TWeakObjectPtr<APlayerController> PlayerController;
	TWeakObjectPtr<USkeletalMeshComponent> SkeletalMesh;

	UFUNCTION()
	void OnItemEquipped(UInv_InventoryItem* Item);

	UFUNCTION()
	void OnItemUnequipped(UInv_InventoryItem* Item);

	void InitInventoryComponent();
};


