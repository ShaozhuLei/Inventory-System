// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/Inv_HighlightStaticMesh.h"

void UInv_HighlightStaticMesh::Hightlight_Implementation()
{
	SetOverlayMaterial(HighlightMaterial);
}

void UInv_HighlightStaticMesh::UnHightlight_Implementation()
{
	SetOverlayMaterial(nullptr);
}
