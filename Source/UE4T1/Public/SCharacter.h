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


UCLASS()
class UE4T1_API ASCharacter : public ACharacter
{
	GENERATED_BODY()
protected:
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

	void SpawnProjectile(TSubclassOf<AActor> classToSpawn);
	
public:
	ASCharacter();
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
