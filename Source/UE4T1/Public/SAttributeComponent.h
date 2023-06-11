// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

// 声明 动态 多播 委托
// 参数 委托名 arg1类型 arg1值，arg2类型 arg2值 arg3类型 arg3值 arg4类型 arg4值
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged,AActor*,InstigatorActor, USAttributeComponent*,OwningComp,float, NewHealth, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4T1_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USAttributeComponent();

protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Attributes")
	float Health;


public:	
	UFUNCTION(BlueprintCallable)
	bool IsAlive();
	UPROPERTY(BlueprintAssignable);
	FOnHealthChanged OnHealthChange;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetHealth();
	UFUNCTION(BlueprintCallable,Category = "Attributes")
	bool ApplyHealthChange(float Delta);

		
};
