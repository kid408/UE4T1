// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SProjectileBase.generated.h"

class USphereComponent;
class UParticleSystemComponent;
class UProjectileMovementComponent;
class UAudioComponent;
class USoundCue;
class UCameraShakeBase;

UCLASS(ABSTRACT) // ABSTRACT 标示着这个类 是不完整的，
class UE4T1_API ASProjectileBase : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Effects|Shake")
		TSubclassOf<UCameraShakeBase> ImpactShake;

	UPROPERTY(EditDefaultsOnly, Category = "Effects|Shake")
		float ImpactShakeInnerRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Effects|Shake")
		float ImpactShakeOuterRadius;


	UPROPERTY(EditDefaultsOnly, Category = "Effects")
		UParticleSystem* ImpactVFX;
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
		USoundCue* ImpactSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USphereComponent* SphereComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UProjectileMovementComponent* MoveComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UParticleSystemComponent* EffectComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UAudioComponent* AudioComp;

	// 虚函数，可以在子类中重写
	UFUNCTION()
		virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// BlueprintNativeEvent = c++ 可以加载到蓝图
	// BlueprintCallable 允许子类触发爆炸
	// 不需要分配，稍后在蓝图里扩展
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
		void Explode();

	virtual void PostInitializeComponents() override;

public:	
	// Sets default values for this actor's properties
	ASProjectileBase();

};
