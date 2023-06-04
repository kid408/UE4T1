// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "SGamePlayInterface.h"
#include <DrawDebugHelpers.h>


// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USInteractionComponent::PrimaryInteract()
{
	// 创建一个查询参数
	FCollisionObjectQueryParams objectQueryParams;
	objectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor* MyOwner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	// 获得眼的位置
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	// 从眼的位置到1000的直线位置
	FVector End = EyeLocation + (EyeRotation.Vector() * 1000);

	/*
	// 线点查找	
	FHitResult Hit;
	bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, objectQueryParams);

	AActor* HitActor = Hit.GetActor();
	if (HitActor)
	{
		// 是否实现ISGamePlayInterface 接口
		if (HitActor->Implements<USGamePlayInterface>())
		{
			APawn* MyPawn = Cast<APawn>(MyOwner);
			ISGamePlayInterface::Execute_Interact(HitActor, MyPawn);
		}
	}
	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;
	DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0, 2.0f);
	*/

	// 球状查找

	// 声明命中结果
	TArray<FHitResult> Hits;
	
	float Radius = 30.f;
	FCollisionShape Shape;
	Shape.SetSphere(Radius);

	// 如果眼看到的1000范围之内命中到了物品
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, objectQueryParams, Shape);
	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;
	for (FHitResult Hit :Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			// 是否实现ISGamePlayInterface 接口
			if (HitActor->Implements<USGamePlayInterface>())
			{
				APawn* MyPawn = Cast<APawn>(MyOwner);
				// 执行被看到物品的交互动作
				ISGamePlayInterface::Execute_Interact(HitActor, MyPawn);
				break;
			}
		}
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, LineColor, false, 2.0f);
	}
	
	
	DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0, 2.0f);
}
