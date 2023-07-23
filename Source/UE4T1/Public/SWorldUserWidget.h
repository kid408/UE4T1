// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SWorldUserWidget.generated.h"

class USizeBox;
/**
 * 
 */
UCLASS()
class UE4T1_API USWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(Meta=(BindWidget))
	USizeBox* ParentSizeBox;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UPROPERTY(EditAnywhere,Category="UI")
	FVector WorldOffset;
	UPROPERTY(BlueprintReadOnly,Category="UI")
	AActor* AttackedActor;
	
};
