// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/AttributeBarWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UAttributeBarWidget::SetValues(float CurrentValue, float MaxValue)
{
	if (CurrentValueText)
	{
		CurrentValueText->SetText(FText::AsNumber(CurrentValue));
	}
	if (MaxValueText)
	{
		MaxValueText->SetText(FText::AsNumber(MaxValue));
	}
	if (ProgressBar)
	{
		const float Ratio = MaxValue > 0.0f ? CurrentValue / MaxValue : 0.0f;
		ProgressBar->SetPercent(FMath::Clamp(Ratio, 0.0f, 1.0f));
	}
}