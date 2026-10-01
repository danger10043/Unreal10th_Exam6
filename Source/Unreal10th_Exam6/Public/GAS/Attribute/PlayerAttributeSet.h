// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "PlayerAttributeSet.generated.h"


UCLASS()
class UNREAL10TH_EXAM6_API UPlayerAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UPlayerAttributeSet();

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	ATTRIBUTE_ACCESSORS_BASIC(UPlayerAttributeSet, Health);
	ATTRIBUTE_ACCESSORS_BASIC(UPlayerAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS_BASIC(UPlayerAttributeSet, Mana);
	ATTRIBUTE_ACCESSORS_BASIC(UPlayerAttributeSet, MaxMana);

protected:
	// ------------------------------------------------------------------
	UPROPERTY(BlueprintReadOnly, Category = "Exam|Stat")
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, Category = "Exam|Stat")
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, Category = "Exam|Stat")
	FGameplayAttributeData Mana;

	UPROPERTY(BlueprintReadOnly, Category = "Exam|Stat")
	FGameplayAttributeData MaxMana;
	// ------------------------------------------------------------------
	UPROPERTY(BlueprintReadOnly, Category = "Exam|Stat|Meta Attribute")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS_BASIC(UPlayerAttributeSet, Damage);

	UPROPERTY(BlueprintReadOnly, Category = "Exam|Stat|Meta Attribute")
	FGameplayAttributeData ManaCost;
	ATTRIBUTE_ACCESSORS_BASIC(UPlayerAttributeSet, ManaCost);
};
