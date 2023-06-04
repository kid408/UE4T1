// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetSimulatePhysics(true);
	RootComponent = MeshComp;
	ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComp");
	ForceComp->SetupAttachment(MeshComp);

	//打开此选项，通过组件'tick'施加小的恒定力(可选)
	ForceComp->SetAutoActivate(false);

	//ForceComp->Redius = 750.0f;
	ForceComp->ImpulseStrength = 2500.0f;

	//可选，忽略其他对象的'质量'(如果为false，脉冲强度将高得多，根据质量来推动大多数对象)
	ForceComp->bImpulseVelChange = true;
	ForceComp->bIgnoreOwningActor = true;

	//可选的，组件的默认构造函数已经添加了4种对象类型，不包括WorldDynamic
	ForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);
	//设置actor每帧调用Tick()。如果您不需要它，可以关闭它以提高性能。
	PrimaryActorTick.bCanEverTick = true;

}

void ASExplosiveBarrel::OnActiorHit(UPrimitiveComponent* HitCoponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ForceComp->FireImpulse();

	//UE_LOG(LogTemp,log,TEXT("OnActorHit in Explosive Barrel");
	UE_LOG(LogTemp, Log, TEXT("test"));
	UE_LOG(LogTemp, Warning, TEXT(" name:%s,time:%f"), *GetNameSafe(OtherActor),GetWorld()->TimeSeconds);
	FString CombinedString = FString::Printf(TEXT("Hit at location:%s"), *Hit.ImpactPoint.ToString());
	DrawDebugString(GetWorld(),Hit.ImpactPoint,CombinedString,nullptr,FColor::Red,2.0f,true);
}

// Called when the game starts or when spawned
void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

}

