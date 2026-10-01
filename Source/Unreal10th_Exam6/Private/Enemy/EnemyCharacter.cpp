// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/EnemyCharacter.h"
#include "AbilitySystemComponent.h"
#include "GAS/Attribute/EnemyAttributeSet.h"
#include "Widget/AttributeBarWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"

AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	EnemyAttributeSet = CreateDefaultSubobject<UEnemyAttributeSet>(TEXT("EnemyAttributeSet"));

	HealthUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthUI"));
	HealthUI->SetupAttachment(GetRootComponent());
	HealthUI->SetWidgetSpace(EWidgetSpace::Screen);
	HealthUI->SetPivot(FVector2D(0.5f, 1.0f));
	HealthUI->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* AEnemyCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	HealthUI->InitWidget();
	HealthBar = Cast<UAttributeBarWidget>(HealthUI->GetUserWidgetObject());
	if (!HealthBar)
	{
		return;
	}
	HealthBar->SetVisibility(ESlateVisibility::HitTestInvisible);

	HealthChangedHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		UEnemyAttributeSet::GetHealthAttribute()).AddUObject(
			this, &AEnemyCharacter::OnHealthChanged);
	MaxHealthChangedHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		UEnemyAttributeSet::GetMaxHealthAttribute()).AddUObject(
			this, &AEnemyCharacter::OnHealthChanged);

	RefreshHealthUI();
}

void AEnemyCharacter::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	RefreshHealthUI();
}

void AEnemyCharacter::RefreshHealthUI()
{
	if (HealthBar)
	{
		HealthBar->SetValues(
			EnemyAttributeSet->GetHealth(),
			EnemyAttributeSet->GetMaxHealth()
		);
	}
}

void AEnemyCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			UEnemyAttributeSet::GetHealthAttribute()).Remove(HealthChangedHandle);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			UEnemyAttributeSet::GetMaxHealthAttribute()).Remove(MaxHealthChangedHandle);
	}
	Super::EndPlay(EndPlayReason);
}