// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StructUtils/InstancedStruct.h"
#include "UObject/Object.h"
#include "Items/Manifest/Inv_ItemManifest.h"
#include "Inv_InventoryItem.generated.h"

struct FInv_ItemManifest;
/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_InventoryItem : public UObject
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override {return true;};
	
	void SetItemManifest(FInv_ItemManifest& Manifest);
	const FInv_ItemManifest& GetItemManifest() const {return ItemManifest.Get<FInv_ItemManifest>();};
	FInv_ItemManifest& GetMutableItemManifest() {return ItemManifest.GetMutable<FInv_ItemManifest>();};
	bool IsStackable() const;
	bool IsConsumable() const;
	FORCEINLINE int32 GetTotalStackCount(){return TotalStackCount;};
	FORCEINLINE void SetTotalStackCount(int32 Count){TotalStackCount = Count;};
	

private:

	UPROPERTY(VisibleAnywhere, meta = (BaseStruct = "/Script/Inventory.Inv_InventoryItem"), Replicated)
	FInstancedStruct ItemManifest;

	UPROPERTY(Replicated)
	int32 TotalStackCount;

	
};


template<typename FragmentType>
const FragmentType* GetFragment(const UInv_InventoryItem* Item, const FGameplayTag& Tag)
{
	if (!IsValid(Item)) return nullptr;

	const FInv_ItemManifest& Manifest = Item->GetItemManifest();
	return Manifest.GetFragmentOfTypeWithTag<FragmentType>(Tag);
}