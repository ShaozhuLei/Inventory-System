// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagement/Components/Inv_InventoryComponent.h"

#include "Field/FieldSystemNodes.h"
#include "InventoryManagement/FastArray/Inv_FastArray.h"
#include "InventoryManagement/FastArray/Inv_FastArray.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Net/DataChannel.h"
#include "Widgets/Inventory/InventoryBase/Inv_InventoryBase.h"
#include "Items/Inv_InventoryItem.h"


// Sets default values for this component's properties
UInv_InventoryComponent::UInv_InventoryComponent(): InventoryList(this)
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicated(true);
	bReplicateUsingRegisteredSubObjectList = true;
	bInventoryMenuOpen = false;
	
}

// Called when the game starts
void UInv_InventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	ConstructInventory();
}

void UInv_InventoryComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UInv_InventoryComponent::ConstructInventory()
{
	OwningController = Cast<APlayerController>(GetOwner());
	checkf(OwningController.IsValid(), TEXT("Owner is not a PlayerController"));

	if (!OwningController->IsLocalController()) return;

	InventoryMenu = CreateWidget<UInv_InventoryBase>(OwningController.Get(), InventoryMenuClass);
	InventoryMenu->AddToViewport();
	CloseInventoryMenu();
}

void UInv_InventoryComponent::ToggleInventory()
{
	if (bInventoryMenuOpen)
	{
		CloseInventoryMenu();
	}
	else
	{
		OpenInventoryMenu();
	}
}

void UInv_InventoryComponent::AddRepSubObj(UObject* Object)
{
	if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && IsValid(Object))
	{
		AddReplicatedSubObject(Object);
	}
}



void UInv_InventoryComponent::TryAddItem(UInv_ItemComponent* ItemComponent)
{
	
	FInv_SlotAvailabilityResult Result  = InventoryMenu->HasRoomForItem(ItemComponent);

	UInv_InventoryItem* FoundItem = InventoryList.FindFirstItembyType(ItemComponent->GetInv_ItemManifest().GetItemType());
	Result.Item = FoundItem;

	//该槽儿已满
	if (Result.TotalRoomToFill == 0)
	{
		NoRoomInInventory.Broadcast();
		return;
	}

	//1. 该物品已经在背包内,且还有空间存放更多
	if (Result.Item.IsValid() && Result.bStackable)
	{
		OnStackChange.Broadcast(Result );
		Server_AddStackToItem(ItemComponent, Result .TotalRoomToFill, Result .Remainder);
	}
	//2. 新来的物品 还要判断是否是可叠加物品
	else if (Result.TotalRoomToFill > 0) 
	{
		Server_AddNewItem(ItemComponent, Result.bStackable? Result .TotalRoomToFill : 0);
	}
}

void UInv_InventoryComponent::Server_ConsumeItem_Implementation(UInv_InventoryItem* Item)
{
	const int32 NewStackCount = Item->GetTotalStackCount() - 1;
	if (NewStackCount <= 0)
	{
		InventoryList.RemoveEntry(Item);
	}
	else
	{
		Item->SetTotalStackCount(NewStackCount);
	}

	if (FInv_ConsumableFragment* ConsumableFragment = Item->GetMutableItemManifest().GetFragmentOfTypeMutable<FInv_ConsumableFragment>())
	{
		ConsumableFragment->OnConsume(OwningController.Get());
	}
	
}

//场景里生成从库存里扔出来的东西
void UInv_InventoryComponent::SpawnDroppedItem(UInv_InventoryItem* Item, int32 StackCount)
{
	APawn* Pawn = OwningController->GetPawn();
	FVector ActorForwardVector = Pawn->GetActorForwardVector();
	FVector ResultVector = ActorForwardVector.RotateAngleAxisRad(FMath::FRandRange(DropSpawnAngleMin, DropSpawnAngleMax), FVector::UpVector);
	FVector SpawnLocation = Pawn->GetActorLocation() + ResultVector;
	SpawnLocation.Z -= ResultVector.Z;
	const FRotator SpawnRotation = FRotator::ZeroRotator;

	//生成Drop item在场景当中，并赋予其Item Manifest
	FInv_ItemManifest& ItemManifest = Item->GetMutableItemManifest();
	if (FInv_StackableFragment* StackableFragment = ItemManifest.GetFragmentOfTypeMutable<FInv_StackableFragment>())
	{
		StackableFragment->SetStackCount(StackCount);
	}
	ItemManifest.SpawnPickupActor(GetWorld(), SpawnLocation, SpawnRotation);
	
}

void UInv_InventoryComponent::Server_DropItem_Implementation(UInv_InventoryItem* Item, int32 StackCount)
{
	const int32 NewStacks = Item->GetTotalStackCount() - StackCount;
	if (NewStacks <= 0)
	{
		//说明全部丢弃
		InventoryList.RemoveEntry(Item);
	}
	else
	{
		Item->SetTotalStackCount(NewStacks);
	}
	SpawnDroppedItem(Item, StackCount);
}

//添加新的
void UInv_InventoryComponent::Server_AddNewItem_Implementation(UInv_ItemComponent* ItemComponent, int32 StackCount)
{
	UInv_InventoryItem* NewItem = InventoryList.AddEntry(ItemComponent);
	NewItem->SetTotalStackCount(StackCount);
	
	if (GetOwner()->GetNetMode() == NM_ListenServer || GetOwner()->GetNetMode() == NM_Standalone)
	{
		OnItemAdded.Broadcast(NewItem);
	}
	ItemComponent->PickedUp();
}

//添加已拥有的
void UInv_InventoryComponent::Server_AddStackToItem_Implementation(UInv_ItemComponent* ItemComponent, int32 StackCount, int32 Remainder)
{
	const FGameplayTag ItemType = IsValid(ItemComponent)? ItemComponent->GetInv_ItemManifest().GetItemType(): FGameplayTag::EmptyTag;
	UInv_InventoryItem* Item  = InventoryList.FindFirstItembyType(ItemType);
	if (!IsValid(Item )) return;
	Item->SetTotalStackCount(Item ->GetTotalStackCount() + StackCount);

	if (Remainder == 0)
	{
		//一次性完全拾取
		ItemComponent->PickedUp();
	}
	else if (FInv_StackableFragment* StackableFragment = ItemComponent->GetInv_ItemManifest().GetFragmentOfTypeMutable<FInv_StackableFragment>())
	{
		//留在原地的物品 累计数量更新
		//Item->SetStackCount(Item->GetStackCount() - StackCount); 
		StackableFragment->SetStackCount(Remainder);
	}
}

void UInv_InventoryComponent::OpenInventoryMenu()
{
	if (!IsValid(InventoryMenu)) return;

	InventoryMenu->SetVisibility(ESlateVisibility::Visible);
	bInventoryMenuOpen = true;
	
	FInputModeGameAndUI InputMode;
	OwningController->SetInputMode(InputMode);
	OwningController->SetShowMouseCursor(true);
}

void UInv_InventoryComponent::CloseInventoryMenu()
{
	if (!IsValid(InventoryMenu)) return;

	//折叠性能更好
	InventoryMenu->SetVisibility(ESlateVisibility::Hidden);
	bInventoryMenuOpen = false;

	if (!OwningController.IsValid()) return;
	
	FInputModeGameOnly InputMode;
	OwningController->SetInputMode(InputMode);
	OwningController->SetShowMouseCursor(false);
}
