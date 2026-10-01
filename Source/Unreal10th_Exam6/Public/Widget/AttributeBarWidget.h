// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AttributeBarWidget.generated.h"

class UTextBlock;
class UProgressBar;

UCLASS()
class UNREAL10TH_EXAM6_API UAttributeBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetValues(float CurrentValue, float MaxValue);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentValueText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MaxValueText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar;
};