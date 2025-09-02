// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ItemDescription/Inv_ItemDescription.h"

#include "Components/SizeBox.h"

const FVector2D UInv_ItemDescription::GetSizeBoxSize()
{
	return SizeBox->GetDesiredSize();
}
