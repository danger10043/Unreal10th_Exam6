// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbility_Fireball.generated.h"

UCLASS()
class UNREAL10TH_EXAM6_API UGameplayAbility_Fireball : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGameplayAbility_Fireball();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	virtual bool CheckCost(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Exam|Fireball")
	TSubclassOf<class AFireballProjectile> FireballClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Exam|Fireball")
	FVector SpawnOffset = FVector(100.0f, 0.0f, 50.0f);
};
