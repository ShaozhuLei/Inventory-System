﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/Composite/Inv_Composite.h"
#include "Inv_ItemDescription.generated.h"

class USizeBox;
/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_ItemDescription : public UInv_Composite
{
	GENERATED_BODY()

public:
	const FVector2D GetSizeBoxSize();

private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox;
};
