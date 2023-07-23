// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <SAttributeComponent.h>
#include "SCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USInteractionComponent;
class UAnimMontage;
class USAttributeComponent;
class UParticleSystem;


UCLASS()
class UE4T1_API ASCharacter : public ACharacter
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleAnywhere, Category = "Effects")
		FName TimeToHitParamName;
	UPROPERTY(VisibleAnywhere, Category = "Effects")
		FName HandSocketName;


	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
		TSubclassOf<AActor> BlackHoleProjectileClass;
	UPROPERTY(EditAnywhere, Category = "Attack")
		TSubclassOf<AActor> DashProjectileClass;

	UPROPERTY(EditAnywhere,Category="Attack")
	UAnimMontage* AttackAnim;
	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* SecondAttackAnim;

	UPROPERTY(EditAnywhere, Category = "Attack")
		UParticleSystem* CastingEffect;

	FTimerHandle TimerHandle_PrimaryAttack;
	FTimerHandle TimerHandle_BlackHoleAttack;
	FTimerHandle TImerHandle_Dash;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
		float AttackAnimDely;
	

protected:
	UPROPERTY(VisibleAnywhere)
		USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere)
		UCameraComponent* CameraComp;
	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Components")
	USAttributeComponent* AttributeComp;

	void MoveForward(float value);
	void MoveRight(float value);

	void PrimaryTick();
	void PrimaryInteract();
	void PrimaryAttack_TimeElapsed();

	void BlackHoleAttack();
	void BlackholeAttack_TimeElapsed();
	void Dash();
	void Dash_TimeElapsed();

	void StartAttackEffects();

	void SpawnProjectile(TSubclassOf<AActor> classToSpawn);

	UFUNCTION()
		void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

	virtual void PostInitializeComponents() override;
	
public:
	ASCharacter();
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Exec)
	void HealSelf(float Amount=100);

};
