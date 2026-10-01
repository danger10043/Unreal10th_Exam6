// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

class UPlayerStatWidget;
class APawn;

UCLASS()
class UNREAL10TH_EXAM6_API APlayerHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditDefaultsOnly, Category = "Exam|UI")
	TSubclassOf<UPlayerStatWidget> StatUIClass;

	UPROPERTY()
	TObjectPtr<UPlayerStatWidget> StatUIWidget;

	UFUNCTION()
	void OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);
};
