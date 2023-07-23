// Fill out your copyright notice in the Description page of Project Settings.


#include "SWorldUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"

void USWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	FVector2D ScreenPosition;

	if (!IsValid(AttackedActor))
	{
		RemoveFromParent();

		UE_LOG(LogTemp, Warning, TEXT("USWorldUserWidget NativeTick AttackedActor is null"));
		return;
	}
	
	if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttackedActor->GetActorLocation()+ WorldOffset, ScreenPosition))
	{
		float Scale = UWidgetLayoutLibrary::GetViewportScale(this);
		ScreenPosition /= Scale;

		if (ParentSizeBox)
		{
			ParentSizeBox->SetRenderTranslation(ScreenPosition);
		}

	}
}
