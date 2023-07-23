// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include <AI/SAICharacter.h>
#include <SAttributeComponent.h>
#include <EngineUtils.h>
#include <DrawDebugHelpers.h>

ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval = 2.0f;
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}

void ASGameModeBase::SpawnBotTimerElapsed()
{
	int32 NrofAliveBots = 0;

	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Bot);
		if (AttributeComp && AttributeComp->IsAlive())
		{
			NrofAliveBots++;
		}
	}

	float MaxBotCount = 5.0f;


	if (DifficultCurve)
	{
		MaxBotCount = DifficultCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}


	if (NrofAliveBots >= MaxBotCount)
	{
		return;
	}

	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnQueryCompleted);
	}
	UE_LOG(LogTemp, Log, TEXT("OnQueryCompleted  spawn:%u"), NrofAliveBots);
}

void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp,Warning,TEXT("OnQueryCompleted err,Status:%u"), QueryStatus);
		return;
	}


	TArray<FVector> locations = QueryInstance->GetResultsAsLocations();
	if (locations.Num() > 0)
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, locations[0], FRotator::ZeroRotator);

		// 出生点画个球，测试用
		DrawDebugSphere(GetWorld(), locations[0], 50.0f, 20,FColor::Blue, false, 5.0f);
	}
}

void ASGameModeBase::KillAllAI()
{
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttrbuteComp = USAttributeComponent::GetAttributes(Bot);
		if (ensure(AttrbuteComp) && AttrbuteComp->IsActorAlive(Bot))
		{
			AttrbuteComp->Kill(this);
		}
	}
}

