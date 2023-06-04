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

}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, 0.2f);
	
}


void ASCharacter::PrimaryAttack_TimeElapsed()
{
	// �˻�: ��ensure�����ϼ����Զ���msg������
	// ensureMsgf(exp, msg, ...);
	// check��verify�����жϳ���, ensureֻ���ӡ��ջ��Ϣ
	// verify�ܱ�֤���ʽһ��ִ�У�check���С������ԽǶȵĻ�verify�ȽϺ�
	// ��һ�δ���
	//if (ensure(ProjectileClass)) {
	// ���Ǵ���
	//if (ensureAlways(ProjectileClass)) {
	if (ensure(ProjectileClass)) {
		// �ҵ���ָ�Ĺ���λ��
		FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
		// ����ָ����λ�ð󶨳���
		FTransform SpawnTM = FTransform(GetControlRotation(), HandLocation);
		FActorSpawnParameters SpawnParams;
		// �����ɵ�����ͻ�ķ�����δ������ζ��û����д��ʹ��actor������
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		// ���÷����ߣ������ӵ����õ�ʱ������ж��ӵ��ͷ������ǲ���һ���ˣ�ͬһ���˿���ȡ����ײ
		SpawnParams.Instigator = this;

		// �����ӵ�������λ������ָ���򣬲����뷢����
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
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
