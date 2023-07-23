// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SBTTask_RangedAttack.generated.h"

/**
 * 
 */
UCLASS()
class UE4T1_API USBTTask_RangedAttack : public UBTTaskNode
{
	GENERATED_BODY()

	USBTTask_RangedAttack();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;

public:
	UPROPERTY(EditAnywhere, Category = "AI")
		float MaxBulletSpread;

		

protected:
	UPROPERTY(EditAnywhere, Category = "AI")
		TSubclassOf<AActor> ProjectileClass;
};
