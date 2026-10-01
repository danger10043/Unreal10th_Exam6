// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/FireballProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffect.h"
#include "NativeGameplayTags.h"
#include "Enemy/EnemyCharacter.h"
#include "GAS/Attribute/EnemyAttributeSet.h"

AFireballProjectile::AFireballProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SetRootComponent(Collision);
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Collision->SetCollisionObjectType(ECC_WorldDynamic);
	Collision->SetCollisionResponseToAllChannels(ECR_Block);
	Collision->SetGenerateOverlapEvents(false);
	Collision->OnComponentHit.AddDynamic(this, &AFireballProjectile::OnProjectileHit);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->SetUpdatedComponent(Collision);
	ProjectileMovement->Velocity = FVector(1.0f, 0.0f, 0.0f);
	ProjectileMovement->bInitialVelocityInLocalSpace = true;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.0f;

	InitialLifeSpan = 5.0f;
}

void AFireballProjectile::BeginPlay()
{
	Super::BeginPlay();
	Collision->IgnoreActorWhenMoving(GetOwner(), true);
}

void AFireballProjectile::OnProjectileHit(
	UPrimitiveComponent* HitComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComponent,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	if (OtherActor == GetOwner() || bHitProcessed)
	{
		return;
	}
	bHitProcessed = true;

	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OtherActor);


	if (Enemy)
	{
		UAbilitySystemComponent* SourceASC =
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
		UAbilitySystemComponent* TargetASC = Enemy->GetAbilitySystemComponent();

		if (SourceASC && 
			TargetASC && 
			HitDamageEffectClass && 
			BurnEffectClass &&
			TargetASC->HasAttributeSetForAttribute(UEnemyAttributeSet::GetHealthAttribute()))
		{
			const FGameplayTag BurnTag = FGameplayTag::RequestGameplayTag(
				FName(TEXT("Entity.State.Burn")));

			const bool bAlreadyBurning = TargetASC->HasMatchingGameplayTag(BurnTag);
			const float Damage = bAlreadyBurning ? 20.0f : 10.0f;

			FGameplayEffectContextHandle Context = SourceASC->MakeEffectContext();
			Context.AddInstigator(GetInstigator(), this);
			Context.AddSourceObject(GetOwner());
			Context.AddHitResult(Hit, true);

			FGameplayEffectSpecHandle HitSpec =
				SourceASC->MakeOutgoingSpec(HitDamageEffectClass, 1.0f, Context);
			FGameplayEffectSpecHandle BurnSpec =
				SourceASC->MakeOutgoingSpec(BurnEffectClass, 1.0f, Context);

			const FGameplayTag DamageTag = FGameplayTag::RequestGameplayTag(
				FName(TEXT("Entity.Data.Damage")));

			if (HitSpec.IsValid() && BurnSpec.IsValid())
			{
				HitSpec.Data->SetSetByCallerMagnitude(DamageTag, Damage);
				SourceASC->ApplyGameplayEffectSpecToTarget(*HitSpec.Data.Get(), TargetASC);
				SourceASC->ApplyGameplayEffectSpecToTarget(*BurnSpec.Data.Get(), TargetASC);
			}
		}
	}

	Destroy();
}
