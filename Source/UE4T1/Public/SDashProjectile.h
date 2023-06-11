// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "SDashProjectile.generated.h"

/**
 * 
 */
UCLASS()
class UE4T1_API ASDashProjectile : public ASProjectileBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
		float TeleportDelay;
	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
		float DetonateDelay;

	// ����Ѿ����������壬����ȡ����ը�ӳ�
	FTimerHandle TimerHandle_DelayedDetonate;

	// ����ʹ����ͼEventʵ�֣�������дExplode_Implementation��������Explode()
	virtual void Explode_Implementation() override;

	void TeleprtInstigator();

	virtual void BeginPlay() override;

public:

	ASDashProjectile();
	
};
