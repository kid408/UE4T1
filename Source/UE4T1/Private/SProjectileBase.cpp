// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectileBase.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ASProjectileBase::ASProjectileBase()
{
    SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
    SphereComp->SetCollisionProfileName("Projectile");
    SphereComp->OnComponentHit.AddDynamic(this,&ASProjectileBase::OnActorHit);
    RootComponent = SphereComp;

    EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
    EffectComp->SetupAttachment(RootComponent);

    MoveComp = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMoveComp");
    MoveComp->bRotationFollowsVelocity = true;
    MoveComp->bInitialVelocityInLocalSpace = true;
    MoveComp->ProjectileGravityScale =0.0f;
    MoveComp->InitialSpeed = 8000;

}


void ASProjectileBase::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    Explode();
}

//  _Implementation 蓝图的event进行实现
void ASProjectileBase::Explode_Implementation()
{
    // 检查是否等待销毁
	if (ensure(!IsPendingKill()))
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());

		Destroy();
	}
}

void ASProjectileBase::PostInitializeComponents()
{
    Super::PostInitializeComponents();
}


