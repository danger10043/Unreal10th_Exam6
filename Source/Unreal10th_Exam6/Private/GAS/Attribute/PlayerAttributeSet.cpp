// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/Attribute/PlayerAttributeSet.h"

UPlayerAttributeSet::UPlayerAttributeSet()
{
	InitHealth(100.0f);
	InitMaxHealth(100.0f);
	InitMaxMana(100.0f);
	InitMana(100.0f);
}

void UPlayerAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, FMath::Max(0.0f, GetMaxHealth()));
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.0f);
	}

	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, FMath::Max(0.0f, GetMaxMana()));
	}
	else if (Attribute == GetMaxManaAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.0f);
	}
}

void UPlayerAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		UE_LOG(
			LogTemp,
			Log,
			TEXT("[PlayerAttributeSet] Health 변경됨 : [%.1f] ▷ [%.1f]"),
			OldValue,
			NewValue
		);
	}
	else if (Attribute == GetManaAttribute())
	{
		UE_LOG(
			LogTemp,
			Log,
			TEXT("[PlayerAttributeSet] Mana 변경됨 : [%.1f] ▷ [%.1f]"),
			OldValue,
			NewValue
		);
	}
}

void UPlayerAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		const float LocalDamage = GetDamage();
		SetDamage(0.0f);

		if (LocalDamage > 0)
		{
			const float FinalDamage = LocalDamage;
			const float HealthBefore = GetHealth();
			const float NewHealth = FMath::Clamp(HealthBefore - FinalDamage, 0.0f, GetMaxHealth());
			SetHealth(NewHealth);
		}
	}
	else if (Data.EvaluatedData.Attribute == GetManaCostAttribute())
	{
		const float LocalCost = GetManaCost();
		SetManaCost(0.0f);

		if (LocalCost > 0)
		{
			const float FinalCost = LocalCost;
			const float ManaBefore = GetMana();
			const float NewMana = FMath::Clamp(ManaBefore - FinalCost, 0.0f, GetMaxMana());
			SetMana(NewMana);
		}
	}

	else if (
		Data.EvaluatedData.Attribute == GetHealthAttribute() ||
		Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, FMath::Max(0.0f, GetMaxHealth())));
	}
	else if (
		Data.EvaluatedData.Attribute == GetManaAttribute() ||
		Data.EvaluatedData.Attribute == GetMaxManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.0f, FMath::Max(0.0f, GetMaxMana())));
	}
}
