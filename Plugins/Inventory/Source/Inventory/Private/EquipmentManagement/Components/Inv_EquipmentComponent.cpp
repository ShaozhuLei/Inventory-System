// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentManagement/Components/Inv_EquipmentComponent.h"

#include "GameFramework/Character.h"
#include "InventoryManagement/Inv_InventoryStatics.h"


void UInv_EquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerController = Cast<APlayerController>(GetOwner());
	if (PlayerController.IsValid())
	{
		if (ACharacter* ControlledCharacter = Cast<ACharacter>(PlayerController->GetPawn()); IsValid(ControlledCharacter))
		{
			SkeletalMesh = ControlledCharacter->GetMesh();
		}
		InitInventoryComponent();
	}
}

void UInv_EquipmentComponent::OnItemEquipped(UInv_InventoryItem* Item)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Equipment item selected");
}

void UInv_EquipmentComponent::OnItemUnequipped(UInv_InventoryItem* Item)
{
	
}

void UInv_EquipmentComponent::InitInventoryComponent()
{
	UInv_InventoryComponent* InventoryComponent = UInv_InventoryStatics::GetInventoryComponent(PlayerController.Get());
	if (IsValid(InventoryComponent))
	{
		if (!InventoryComponent->OnItemEquipped.IsAlreadyBound(this, &UInv_EquipmentComponent::OnItemEquipped))
		{
			InventoryComponent->OnItemEquipped.AddDynamic(this, &UInv_EquipmentComponent::OnItemEquipped);
		}

		if (!InventoryComponent->OnItemUnequipped.IsAlreadyBound(this, &UInv_EquipmentComponent::OnItemUnequipped))
		{
			InventoryComponent->OnItemUnequipped.AddDynamic(this, &UInv_EquipmentComponent::OnItemUnequipped);
		}
	}
}
