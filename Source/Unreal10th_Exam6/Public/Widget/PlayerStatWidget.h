// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AttributeSet.h"
#include "PlayerStatWidget.generated.h"

class UAbilitySystemComponent;
class UAttributeBarWidget;
struct FOnAttributeChangeData;

UCLASS()
class UNREAL10TH_EXAM6_API UPlayerStatWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindToAbilitySystem(UAbilitySystemComponent* InASC);

protected:
	virtual void NativeDestruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAttributeBarWidget> HealthBarWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAttributeBarWidget> ManaBarWidget;

private:
	void UnbindAbilitySystem();
	void RefreshValues();
	void OnAttributeChanged(const FOnAttributeChangeData& Data);

	UPROPERTY()
	TWeakObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	TArray<FGameplayAttribute> ObservedAttributes;

	TArray<FDelegateHandle> AttributeHandles;
};