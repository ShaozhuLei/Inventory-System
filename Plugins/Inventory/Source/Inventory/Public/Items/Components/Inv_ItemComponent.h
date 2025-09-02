// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/Manifest/Inv_ItemManifest.h"
#include "StructUtils/InstancedStruct.h"
#include "Inv_ItemComponent.generated.h"


struct FInv_ItemManifest;
struct FInv_ItemFragment;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class INVENTORY_API UInv_ItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInv_ItemComponent();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	FORCEINLINE FString GetPickupString() const {return PickupString; };

	void InitItemManifest(FInv_ItemManifest CopyOfManifest);

	FInv_ItemManifest GetInv_ItemManifest() const {return ItemManifest;};
	void PickedUp();
protected:

	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void OnPickedUp();

private:
	
	UPROPERTY(EditAnywhere, Replicated, Category="Inventory")
	FInv_ItemManifest ItemManifest;
	

	UPROPERTY(EditAnywhere, Category="Inventory")
	FString PickupString;
	
};
