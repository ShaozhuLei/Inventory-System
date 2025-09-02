// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Inv_PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Inventory.h"
#include "Blueprint/UserWidget.h"
#include "Interaction/Inv_Highlightable.h"
#include "InventoryManagement/Components/Inv_InventoryComponent.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/HUD/Inv_HUDWidget.h"

AInv_PlayerController::AInv_PlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	TraceLength = 500.f;
	ItemTraceChannel = ECC_GameTraceChannel1;
}

void AInv_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogInventory, Log, TEXT("begin play for controller"));

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (IsValid(Subsystem))
	{
		for (auto& DefaultIMC: DefaultIMCs)
		{
			Subsystem->AddMappingContext(DefaultIMC, 0);
		}
	}
	InventoryComponent = FindComponentByClass<UInv_InventoryComponent>();
	CreateWidgets();

	
}

void AInv_PlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	TraceForItem();
}



void AInv_PlayerController::CreateWidgets()
{
	if (!IsLocalController()) return;
	HUDWidget = CreateWidget<UInv_HUDWidget>(this, HUDWidgetClass);
	if (IsValid(HUDWidget)) HUDWidget->AddToViewport();
}

void AInv_PlayerController::TraceForItem()
{
	FVector2d ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);

	FVector2d ViewportCenter = ViewportSize / 2;
	FVector TraceStart;
	FVector Forward;
	if (!UGameplayStatics::DeprojectScreenToWorld(this, ViewportCenter, TraceStart, Forward)) return;
	
	FHitResult HitResult;
	TraceStart = TraceStart + Forward * 100.f;
	FVector TraceEnd = TraceStart + Forward * TraceLength;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ItemTraceChannel);

	LastItem = ThisItem;
	ThisItem = HitResult.GetActor();

	if (!ThisItem.IsValid())
	{
		if (IsValid(HUDWidget))
		{
			HUDWidget->HidePickupMessage();
		}
	} 

	if (LastItem == ThisItem) return;

	if (ThisItem.IsValid())
	{
		//先Hightlight 物品
		if (UActorComponent* Highlightable  = ThisItem->FindComponentByInterface(UInv_Highlightable::StaticClass()); IsValid(Highlightable))
		{
			IInv_Highlightable::Execute_Hightlight(Highlightable );
		}
		
		UInv_ItemComponent* ItemComponent = ThisItem->FindComponentByClass<UInv_ItemComponent>();
		if (!IsValid(ItemComponent)) return;
		
		if (IsValid(HUDWidget)) HUDWidget->ShowPickupMessage(ItemComponent->GetPickupString());
	}
	  
	if (LastItem.IsValid())
	{
		if (UActorComponent* Highlightable  = LastItem->FindComponentByInterface(UInv_Highlightable::StaticClass()); IsValid(Highlightable ))
		{
			IInv_Highlightable::Execute_UnHightlight(Highlightable);
		}
	}
	
}

void AInv_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(PrimaryInteractAction, ETriggerEvent::Started, this, &AInv_PlayerController::PrimaryInteract);
	EnhancedInputComponent->BindAction(ToggleInventoryAction, ETriggerEvent::Started, this, &AInv_PlayerController::ToggleInventory);
}

void AInv_PlayerController::PrimaryInteract()
{
	//测试添加库存
	if (!ThisItem.IsValid()) return;

	UInv_ItemComponent* ItemComp = ThisItem->FindComponentByClass<UInv_ItemComponent>();
	if (!IsValid(ItemComp) || !InventoryComponent.IsValid()) return;

	InventoryComponent->TryAddItem(ItemComp);
}

void AInv_PlayerController::ToggleInventory()
{
	if (!InventoryComponent.IsValid()) return;
	InventoryComponent->ToggleInventory();
}
