// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SPowerup_HealthPotion.h"
#include "Components/StaticMeshComponent.h"
#include "SAttributeComponent.h"

ASPowerup_HealthPotion::ASPowerup_HealthPotion()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);
}

void ASPowerup_HealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	UE_LOG(LogTemp, Log, TEXT("Interact_Implementation"));
	if (ensureMsgf(InstigatorPawn, TEXT("SPowerup_HealthPotion InstigatorPawn is null")))
	{
		return;
	}

	USAttributeComponent* AttributComp = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
	if (ensureMsgf(AttributComp, TEXT("SPowerup_HealthPotion AttributCom is null")))
	{
		if (AttributComp->ApplyHealthChange(this,AttributComp->GetHealthMax()))
		{
			HideAndCooldownPowerup();
		}
	}
}