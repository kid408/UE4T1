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

	// 如果已经击中了物体，可以取消爆炸延迟
	FTimerHandle TimerHandle_DelayedDetonate;

	// 基类使用蓝图Event实现，必须重写Explode_Implementation，而不是Explode()
	virtual void Explode_Implementation() override;

	void TeleprtInstigator();

	virtual void BeginPlay() override;

public:

	ASDashProjectile();
	
};
