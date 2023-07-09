// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <SGamePlayInterface.h>
#include "SPowerupActor.generated.h"

class USphereComponent;

UCLASS()
class UE4T1_API ASPowerupActor : public AActor,public ISGamePlayInterface
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "Powerup")
		float RespawnTime;

	FTimerHandle TimerHandle_RespawnTimer;

	UFUNCTION()
		void ShowPowerup();

	void HideAndCooldownPowerup();
	void SetPowerupState(bool bNewIsActive);

	UPROPERTY(VisibleAnywhere, Category = "Components")
		USphereComponent* SphereComp;

public:
	void Interact_Implementation(APawn* InstigatorPawn) override;
public:	
	ASPowerupActor();


};
