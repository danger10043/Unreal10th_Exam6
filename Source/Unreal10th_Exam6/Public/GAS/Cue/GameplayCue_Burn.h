// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Actor.h"
#include "GameplayCue_Burn.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class UNREAL10TH_EXAM6_API AGameplayCue_Burn : public AGameplayCueNotify_Actor
{
	GENERATED_BODY()
	
public:
	AGameplayCue_Burn();

	virtual bool OnActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;
	virtual bool WhileActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;
	virtual bool OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Exam|Burn")
	TObjectPtr<UNiagaraSystem> BurnEffect;

	UPROPERTY()
	TObjectPtr<UNiagaraComponent> BurnComponent;

private:
	void StartBurnEffect(AActor* Target, const FGameplayCueParameters& Parameters);
};
