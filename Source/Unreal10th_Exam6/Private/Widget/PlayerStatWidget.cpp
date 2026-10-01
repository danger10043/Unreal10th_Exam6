// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/PlayerStatWidget.h"
#include "Widget/AttributeBarWidget.h"
#include "AbilitySystemComponent.h"
#include "GAS/Attribute/PlayerAttributeSet.h"

void UPlayerStatWidget::BindToAbilitySystem(UAbilitySystemComponent* InASC)
{
	UnbindAbilitySystem();

	ObservedAttributes = {
		UPlayerAttributeSet::GetHealthAttribute(),
		UPlayerAttributeSet::GetMaxHealthAttribute(),
		UPlayerAttributeSet::GetManaAttribute(),
		UPlayerAttributeSet::GetMaxManaAttribute()
	};

	if (!IsValid(InASC))
	{
		RefreshValues();
		return;
	}

	for (const FGameplayAttribute& Attribute : ObservedAttributes)
	{
		if (!InASC->HasAttributeSetForAttribute(Attribute))
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerStatWidget::BindToAbilitySystem - PlayerAttributeSet의 속성을 찾을 수 없습니다."));
			RefreshValues();
			return;
		}
	}

	AbilitySystemComponent = InASC;
	for (const FGameplayAttribute& Attribute : ObservedAttributes)
	{
		AttributeHandles.Add(
			InASC->GetGameplayAttributeValueChangeDelegate(Attribute)
			.AddUObject(this, &UPlayerStatWidget::OnAttributeChanged));
	}

	RefreshValues();
}

void UPlayerStatWidget::UnbindAbilitySystem()
{
	if (UAbilitySystemComponent* ASC = AbilitySystemComponent.Get())
	{
		for (int32 Index = 0; Index < AttributeHandles.Num(); ++Index)
		{
			ASC->GetGameplayAttributeValueChangeDelegate(ObservedAttributes[Index])
				.Remove(AttributeHandles[Index]);
		}
	}

	AttributeHandles.Reset();
	ObservedAttributes.Reset();
	AbilitySystemComponent.Reset();
}

void UPlayerStatWidget::OnAttributeChanged(const FOnAttributeChangeData& Data)
{
	RefreshValues();
}

void UPlayerStatWidget::RefreshValues()
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	const float Health = ASC ? 
		ASC->GetNumericAttribute(UPlayerAttributeSet::GetHealthAttribute()) : 0.0f;
	const float MaxHealth = ASC ? 
		ASC->GetNumericAttribute(UPlayerAttributeSet::GetMaxHealthAttribute()) : 0.0f;
	const float Mana = ASC ? 
		ASC->GetNumericAttribute(UPlayerAttributeSet::GetManaAttribute()) : 0.0f;
	const float MaxMana = ASC ? 
		ASC->GetNumericAttribute(UPlayerAttributeSet::GetMaxManaAttribute()) : 0.0f;

	if (HealthBarWidget)
	{
		HealthBarWidget->SetValues(Health, MaxHealth);
	}
	if (ManaBarWidget)
	{
		ManaBarWidget->SetValues(Mana, MaxMana);
	}
}

void UPlayerStatWidget::NativeDestruct()
{
	UnbindAbilitySystem();
	Super::NativeDestruct();
}