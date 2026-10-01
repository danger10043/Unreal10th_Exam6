// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/Cue/GameplayCue_Burn.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "NativeGameplayTags.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/PrimitiveComponent.h"

AGameplayCue_Burn::AGameplayCue_Burn()
{
	GameplayCueTag = FGameplayTag::RequestGameplayTag(FName(TEXT("GameplayCue.State.Burn")));
	bAutoDestroyOnRemove = true;
}

void AGameplayCue_Burn::StartBurnEffect(AActor* Target, const FGameplayCueParameters& Parameters)
{
	if (!IsValid(Target) || !BurnEffect || IsValid(BurnComponent))
	{
		return;
	}

	const FHitResult Hit = UAbilitySystemBlueprintLibrary::GetHitResult(Parameters);
	USceneComponent* AttachComponent = Hit.GetComponent();
	if (!AttachComponent || AttachComponent->GetOwner() != Target)
	{
		AttachComponent = Target->GetRootComponent();
	}
	if (!AttachComponent)
	{
		return;
	}

	const FVector Location = Hit.bBlockingHit ? FVector(Hit.ImpactPoint) : Target->GetActorLocation();
	BurnComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
		BurnEffect, 
		AttachComponent, 
		Hit.BoneName, 
		Location, 
		FRotator::ZeroRotator,
		EAttachLocation::KeepWorldPosition,
		false,
		true,
		ENCPoolMethod::None,
		false);
}

bool AGameplayCue_Burn::OnActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	StartBurnEffect(MyTarget, Parameters);
	return true;
}

bool AGameplayCue_Burn::WhileActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	StartBurnEffect(MyTarget, Parameters);
	return true;
}

bool AGameplayCue_Burn::OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	if (IsValid(BurnComponent))
	{
		BurnComponent->DestroyComponent();
	}
	BurnComponent = nullptr;
	return true;
}