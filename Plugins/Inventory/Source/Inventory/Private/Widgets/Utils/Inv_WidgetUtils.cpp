// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Utils/Inv_WidgetUtils.h"

#include "Blueprint/SlateBlueprintLibrary.h"

int32 UInv_WidgetUtils::GetIndexFromPosition(const FIntPoint& Position, const int32 Columns)
{
	//Position 是 (x, y) (Row, Column)
	return  Position.X + Position.Y * Columns;
}

FIntPoint UInv_WidgetUtils::GetPositionFromIndex(int32 Index, const int32 Columns)
{
	return FIntPoint(Index % Columns, Index/Columns);
}

FVector2D UInv_WidgetUtils::GetClampedWidgetPosition(const FVector2D& Boundary, const FVector2D& WidgetSize, const FVector2D& MousePos)
{
	FVector2D ClampedWidgetPosition = MousePos;
	if (MousePos.X + WidgetSize.X > Boundary.X) ClampedWidgetPosition.X = Boundary.X - WidgetSize.X;

	if (MousePos.X < 0) ClampedWidgetPosition.X = 0.f;
	
	if (MousePos.Y + WidgetSize.Y > Boundary.Y) ClampedWidgetPosition.Y = Boundary.Y - WidgetSize.Y;

	if (MousePos.Y < 0) ClampedWidgetPosition.Y = 0.f;

	return ClampedWidgetPosition;
	
}

//返回Widget在屏幕视口中的坐标。
FVector2D UInv_WidgetUtils::GetWidgetPosition(UWidget* Widget)
{
	const FGeometry Geometry = Widget->GetCachedGeometry();
	FVector2D PixelPosition;
	FVector2D ViewportPosition;
	USlateBlueprintLibrary::LocalToViewport(Widget, Geometry, USlateBlueprintLibrary::GetLocalTopLeft(Geometry), PixelPosition, ViewportPosition);
	return ViewportPosition;
}

FVector2D UInv_WidgetUtils::GetWidgetSize(UWidget* Widget)
{
	const FGeometry Geometry = Widget->GetCachedGeometry();
	return Geometry.GetLocalSize();
	
}

//检查鼠标是否在区域内
bool UInv_WidgetUtils::IsWithinBounds(const FVector2D& WidgetStartPoint, const FVector2D& WidgetSize, const FVector2D& MousePos)
{
	return MousePos.X >= WidgetStartPoint.X && MousePos.X <= (WidgetStartPoint.X + WidgetSize.X) &&
		MousePos.Y >= WidgetStartPoint.Y && MousePos.Y <= (WidgetStartPoint.Y + WidgetSize.Y);
}

