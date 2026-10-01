// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/Ability/GameplayAbility_Fireball.h"
#include "Player/FireballProjectile.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GAS/Attribute/PlayerAttributeSet.h"

UGameplayAbility_Fireball::UGameplayAbility_Fireball()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGameplayAbility_Fireball::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	ACharacter* Character = ActorInfo
		? Cast<ACharacter>(ActorInfo->AvatarActor.Get()) : nullptr;
	UWorld* World = Character ? Character->GetWorld() : nullptr;

	if (!Character || 
		!World || 
		!FireballClass ||
		!GetCostGameplayEffect() || 
		!GetCooldownGameplayEffect())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// 파이어볼 스폰 위치를 계산
	const FVector Direction = Character->GetActorForwardVector();
	const FVector Offset = Character->GetActorTransform().TransformVectorNoScale(SpawnOffset);
	const FVector SpawnLocation = Character->GetActorLocation() + Offset;
	const FTransform SpawnTransform(Direction.Rotation(), SpawnLocation);

	// 파이어볼 스폰 가능 여부를 확인
	AFireballProjectile* Projectile = 
		World->SpawnActorDeferred<AFireballProjectile>(
			FireballClass, 
			SpawnTransform, 
			Character, 
			Character,
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);

	if (!Projectile)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		Projectile->Destroy();
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

bool UGameplayAbility_Fireball::CheckCost(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	UAbilitySystemComponent* ASC = ActorInfo ? ActorInfo->AbilitySystemComponent.Get() : nullptr;

	if (!ASC || 
		!GetCostGameplayEffect() ||
		!ASC->HasAttributeSetForAttribute(UPlayerAttributeSet::GetManaAttribute()))
	{
		return false;
	}

	const float CurrentMana =
		ASC->GetNumericAttribute(UPlayerAttributeSet::GetManaAttribute());
	constexpr float RequiredMana = 10.0f;

	if (CurrentMana < RequiredMana)
	{
		return false;
	}

	return Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags);
}