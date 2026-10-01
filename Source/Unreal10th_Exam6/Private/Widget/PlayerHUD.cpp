// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/PlayerHUD.h"
#include "Widget/PlayerStatWidget.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = GetOwningPlayerController();
	if (!PC || !PC->IsLocalController() || !StatUIClass)
	{
		return;
	}

	StatUIWidget = CreateWidget<UPlayerStatWidget>(PC, StatUIClass);
	if (!StatUIWidget)
	{
		return;
	}

	StatUIWidget->AddToPlayerScreen();
	PC->OnPossessedPawnChanged.AddDynamic(this, &APlayerHUD::OnPossessedPawnChanged);
	OnPossessedPawnChanged(nullptr, PC->GetPawn());
}

void APlayerHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (APlayerController* PC = GetOwningPlayerController())
	{
		PC->OnPossessedPawnChanged.RemoveDynamic(this, &APlayerHUD::OnPossessedPawnChanged);
	}
	if (StatUIWidget)
	{
		StatUIWidget->RemoveFromParent();
		StatUIWidget = nullptr;
	}

	Super::EndPlay(EndPlayReason);
}

void APlayerHUD::OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	if (!StatUIWidget)
	{
		return;
	}

	UAbilitySystemComponent* ASC = NewPawn
		? UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(NewPawn) : nullptr;

	StatUIWidget->BindToAbilitySystem(ASC);
	StatUIWidget->SetVisibility(ASC ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
}
