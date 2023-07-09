// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectileBase.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Camera/CameraShakeBase.h"


// Sets default values
ASProjectileBase::ASProjectileBase()
{
    SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
    SphereComp->SetCollisionProfileName("Projectile");
    SphereComp->OnComponentHit.AddDynamic(this,&ASProjectileBase::OnActorHit);
    RootComponent = SphereComp;

    EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
    EffectComp->SetupAttachment(RootComponent);

	AudioComp = CreateDefaultSubobject<UAudioComponent>("AudioComp");
	AudioComp->SetupAttachment(RootComponent);

    MoveComp = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMoveComp");
    MoveComp->bRotationFollowsVelocity = true;
    MoveComp->bInitialVelocityInLocalSpace = true;
    MoveComp->ProjectileGravityScale =0.0f;
    MoveComp->InitialSpeed = 8000;

    ImpactShakeInnerRadius = 250.f;
    ImpactShakeOuterRadius = 250.f;

}


void ASProjectileBase::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    Explode();
}

//  _Implementation ��ͼ��event����ʵ��
void ASProjectileBase::Explode_Implementation()
{
    // ����Ƿ�ȴ�����
	if (ensure(!IsPendingKill()))
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());

       UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());

        UGameplayStatics::PlayWorldCameraShake(this, ImpactShake, GetActorLocation(), ImpactShakeInnerRadius, ImpactShakeOuterRadius);

		Destroy();
	}
}

void ASProjectileBase::PostInitializeComponents()
{
    Super::PostInitializeComponents();
}


