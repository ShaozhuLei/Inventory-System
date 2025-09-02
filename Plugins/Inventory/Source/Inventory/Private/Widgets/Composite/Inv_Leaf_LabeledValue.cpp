// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Composite/Inv_Leaf_LabeledValue.h"

#include "Components/TextBlock.h"

void UInv_Leaf_LabeledValue::SetText_Label(const FText& Text, bool bCollapes)
{
	if (bCollapes)
	{
		Text_Label->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	Text_Label->SetText(Text);
}

void UInv_Leaf_LabeledValue::SetText_Value(const FText& Text, bool bCollapes)
{
	if (bCollapes)
	{
		Text_Value->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	Text_Value->SetText(Text);
}

void UInv_Leaf_LabeledValue::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	FSlateFontInfo LabelFontInfo = Text_Label->GetFont();
	LabelFontInfo.Size = FontSize_Label;
	Text_Label->SetFont(LabelFontInfo);

	FSlateFontInfo ValueFontInfo = Text_Value->GetFont();
	LabelFontInfo.Size = FontSize_Value;
	Text_Value->SetFont(ValueFontInfo);
}
