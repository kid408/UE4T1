// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/characterMovementComponent.h"
#include <SInteractionComponent.h>
// Sets default values
ASCharacter::ASCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	// �����ɱ����õ��������
	SpringArmComp->SetupAttachment(RootComponent);
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	// ����������õ����ɱ���
	CameraComp->SetupAttachment(SpringArmComp);

	// ��ӽ������
	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");
	GetCharacterMovement()->bOrientRotationToMovement = true;

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributesComp");

	bUseControllerRotationYaw = false;

	AttackAnimDely = 0.2f;

}


// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// �����¼�����Ҫ�� project settings��� input�µ�bandings��� axis mappings��Ӷ�Ӧ���¼�MoveForward��ϵͳĬ�ϵĲ�����float
	// ��Ҫ����ʵ�ֺ��� &ASCharacter::MoveForward�����Զ��������¼��Ĳ���
	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	// ��Ӷ����¼�����Ҫ�� project settings��� input�µ�bandings��� action mappings��Ӷ�Ӧ���¼�PrimaryAttack��ϵͳĬ���޲���
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryTick);

	PlayerInputComponent->BindAction("SecondaryAttack", IE_Pressed, this, &ASCharacter::BlackHoleAttack);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ASCharacter::Dash);

	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::Jump);
}

void ASCharacter::MoveForward(float value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.00f;
	ControlRot.Roll = 0.00f;
	// ǰ���� ֱ�ӵ���AddMovementInput(ControlRot.Vector(), value);����
	AddMovementInput(ControlRot.Vector(), value);
}

void ASCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.00f;
	ControlRot.Roll = 0.00f;

	// Ĭ��Ӧ����X�ᣬ�����ǰ���߲���Ҫ���ã���������Ҿ���Ҫ��ʾ�޸�Y��
	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, Value);
}

void ASCharacter::PrimaryTick()
{
	// ���Ŷ�����������Ҫ�� Montage
	PlayAnimMontage(AttackAnim);

	// �����¼����������0.2��֮�󴥷�PrimaryAttack_TimeElapsed ��������Ϊ������̧�ֶ�����0.2��֮�����ȫ̧�֣�
	// ������ʱ���ӳٵĻ����������Żᵼ���ӵ���λ�ú��ֵ�λ�ö�Ӧ����
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, AttackAnimDely);
	
}


void ASCharacter::PrimaryAttack_TimeElapsed()
{
	SpawnProjectile(ProjectileClass);

	return;
	
}

void ASCharacter::BlackHoleAttack()
{
	PlayAnimMontage(SecondAttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_BlackHoleAttack, this, &ASCharacter::BlackholeAttack_TimeElapsed, AttackAnimDely);
}

void ASCharacter::BlackholeAttack_TimeElapsed()
{
	SpawnProjectile(BlackHoleProjectileClass);
}

void ASCharacter::Dash()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TImerHandle_Dash, this, &ASCharacter::Dash_TimeElapsed, AttackAnimDely);
}

void ASCharacter::Dash_TimeElapsed()
{
	SpawnProjectile(DashProjectileClass);
}

void ASCharacter::SpawnProjectile(TSubclassOf<AActor> classToSpawn)
{
	// �˻�: ��ensure�����ϼ����Զ���msg������
	// ensureMsgf(exp, msg, ...);
	// check��verify�����жϳ���, ensureֻ���ӡ��ջ��Ϣ
	// verify�ܱ�֤���ʽһ��ִ�У�check���С������ԽǶȵĻ�verify�ȽϺ�
	// ��һ�δ���
	//if (ensure(ProjectileClass)) {
	// ���Ǵ���
	//if (ensureAlways(ProjectileClass)) {
	if (ensure(classToSpawn)) {
		// �ҵ���ָ�Ĺ���λ��
		FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
		// ����ָ����λ�ð󶨳���

		FActorSpawnParameters SpawnParams;
		// �����ɵ�����ͻ�ķ�����δ������ζ��û����д��ʹ��actor������
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		// ���÷����ߣ������ӵ����õ�ʱ������ж��ӵ��ͷ������ǲ���һ���ˣ�ͬһ���˿���ȡ����ײ
		SpawnParams.Instigator = this;

		FHitResult Hit;
		FVector TraceStart = CameraComp->GetComponentLocation();
		// �յ�վԶ��Ŀ�Ӿ���(��Ҫ̫Զ����Ȼ��δ���е�׼�ǽ��е���)
		FVector TraceEnd = CameraComp->GetComponentLocation() + (GetControlRotation().Vector() * 5000);

		FCollisionShape Shape;
		Shape.SetSphere(20.0f);


		// ���Խ�ɫ
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);

		FRotator ProjRotation;
		// rue������ǵõ�һ����������(���:SweepSingleByChannel �� ECC_WorldDynamic)
		if (GetWorld()->SweepSingleByObjectType(Hit,TraceStart,TraceEnd,FQuat::Identity,ObjParams,Shape,Params))
		{
			// ����λ�ã�������׼׼��
			ProjRotation = FRotationMatrix::MakeFromX(Hit.ImpactPoint - HandLocation).Rotator();
		} 
		else
		{
			// û������
			ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();
		}


		FTransform SpawnTM = FTransform(ProjRotation, HandLocation);
		// �����ӵ�������λ������ָ���򣬲����뷢����
		GetWorld()->SpawnActor<AActor>(classToSpawn, SpawnTM, SpawnParams);
	}
}

void ASCharacter::PrimaryInteract()
{
	// ��ӽ������
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}


