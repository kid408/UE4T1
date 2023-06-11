// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SProjectileBase.generated.h"

class USphereComponent;
class UParticleSystemComponent;
class UProjectileMovementComponent;

UCLASS(ABSTRACT) // ABSTRACT ��ʾ������� �ǲ������ģ�
class UE4T1_API ASProjectileBase : public AActor
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
		UParticleSystem* ImpactVFX;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USphereComponent* SphereComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UProjectileMovementComponent* MoveComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UParticleSystemComponent* EffectComp;

	// �麯������������������д
	UFUNCTION()
		virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// BlueprintNativeEvent = c++ ���Լ��ص���ͼ
	// BlueprintCallable �������ഥ����ը
	// ����Ҫ���䣬�Ժ�����ͼ����չ
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
		void Explode();

	virtual void PostInitializeComponents() override;

public:	
	// Sets default values for this actor's properties
	ASProjectileBase();

};