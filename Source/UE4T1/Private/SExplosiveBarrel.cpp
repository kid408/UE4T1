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

	//�򿪴�ѡ�ͨ�����'tick'ʩ��С�ĺ㶨��(��ѡ)
	ForceComp->SetAutoActivate(false);

	//ForceComp->Redius = 750.0f;
	ForceComp->ImpulseStrength = 2500.0f;

	//��ѡ���������������'����'(���Ϊfalse������ǿ�Ƚ��ߵö࣬�����������ƶ����������)
	ForceComp->bImpulseVelChange = true;
	ForceComp->bIgnoreOwningActor = true;

	//��ѡ�ģ������Ĭ�Ϲ��캯���Ѿ������4�ֶ������ͣ�������WorldDynamic
	ForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);
	//����actorÿ֡����Tick()�����������Ҫ�������Թر�����������ܡ�
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

