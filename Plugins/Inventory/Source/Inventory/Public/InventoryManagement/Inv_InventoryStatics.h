// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Inv_InventoryComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Types/Inv_GridTypes.h"
#include "Widgets/Utils/Inv_WidgetUtils.h"
#include "Inv_InventoryStatics.generated.h"

class UInv_HoverItem;
/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_InventoryStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category="Inventory")
	static UInv_InventoryComponent* GetInventoryComponent(const APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	static EInv_ItemCategory GetItemCategoryFromItemComp(UInv_ItemComponent* ItemComponent);

	template<typename T, typename FuncT>
	static void ForEach2D(TArray<T>& Array, int32 Index, const FIntPoint& Range2D, int32 GridColumns, const FuncT& Func);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static void ItemHovered(APlayerController* PC, UInv_InventoryItem* Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static void ItemUnhovered(APlayerController* PC);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static UInv_HoverItem* GetHoveredItem(APlayerController* PC);
};

template <typename T, typename FuncT>
void UInv_InventoryStatics::ForEach2D(TArray<T>& Array, int32 Index, const FIntPoint& Range2D, int32 GridColumns, const FuncT& Func)
{
	for (int i = 0; i < Range2D.Y; i++)
	{
		for (int j = 0; j < Range2D.X; j++)
		{
			const FIntPoint TargetCoordinate = UInv_WidgetUtils::GetPositionFromIndex(Index, GridColumns) + FIntPoint(j, i);
			const int32 TileIndex = UInv_WidgetUtils::GetIndexFromPosition(TargetCoordinate, GridColumns);
			if (Array.IsValidIndex(TileIndex))
			{
				Func(Array[TileIndex]);
			}
			
		}
	}
}
