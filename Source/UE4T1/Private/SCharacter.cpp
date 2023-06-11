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
	// 将弹簧臂设置到根组件上
	SpringArmComp->SetupAttachment(RootComponent);
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	// 将摄像机设置到弹簧臂上
	CameraComp->SetupAttachment(SpringArmComp);

	// 添加交互组件
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
	// 绑定轴事件，需要在 project settings里的 input下的bandings添加 axis mappings添加对应的事件MoveForward，系统默认的参数是float
	// 需要传递实现函数 &ASCharacter::MoveForward，会自动接受轴事件的参数
	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	// 添加动作事件，需要在 project settings里的 input下的bandings添加 action mappings添加对应的事件PrimaryAttack，系统默认无参数
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
	// 前后走 直接调用AddMovementInput(ControlRot.Vector(), value);就行
	AddMovementInput(ControlRot.Vector(), value);
}

void ASCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.00f;
	ControlRot.Roll = 0.00f;

	// 默认应该是X轴，如果是前后走不需要设置，如果是左右就需要显示修改Y轴
	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, Value);
}

void ASCharacter::PrimaryTick()
{
	// 播放动画，动画需要是 Montage
	PlayAnimMontage(AttackAnim);

	// 设置事件句柄，设置0.2秒之后触发PrimaryAttack_TimeElapsed 函数，因为动画有抬手动作，0.2秒之后才完全抬手，
	// 不设置时间延迟的话，立即播放会导致子弹的位置和手的位置对应不上
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
	// 人话: 在ensure基础上加上自定义msg供调试
	// ensureMsgf(exp, msg, ...);
	// check和verify都会中断程序, ensure只会打印堆栈信息
	// verify能保证表达式一定执行，check不行。从无脑角度的话verify比较好
	// 第一次触发
	//if (ensure(ProjectileClass)) {
	// 总是触发
	//if (ensureAlways(ProjectileClass)) {
	if (ensure(classToSpawn)) {
		// 找到手指的骨骼位置
		FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
		// 将手指骨骼位置绑定朝向

		FActorSpawnParameters SpawnParams;
		// 在生成点解决冲突的方法。未定义意味着没有重写，使用actor的设置
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		// 设置发起者，后续子弹调用的时候可以判断子弹和发起者是不是一个人，同一个人可以取消碰撞
		SpawnParams.Instigator = this;

		FHitResult Hit;
		FVector TraceStart = CameraComp->GetComponentLocation();
		// 终点站远到目视距离(不要太远，仍然对未命中的准星进行调整)
		FVector TraceEnd = CameraComp->GetComponentLocation() + (GetControlRotation().Vector() * 5000);

		FCollisionShape Shape;
		Shape.SetSphere(20.0f);


		// 忽略角色
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);

		FRotator ProjRotation;
		// rue如果我们得到一个阻塞命中(替代:SweepSingleByChannel 与 ECC_WorldDynamic)
		if (GetWorld()->SweepSingleByObjectType(Hit,TraceStart,TraceEnd,FQuat::Identity,ObjParams,Shape,Params))
		{
			// 调整位置，最终瞄准准星
			ProjRotation = FRotationMatrix::MakeFromX(Hit.ImpactPoint - HandLocation).Rotator();
		} 
		else
		{
			// 没有命中
			ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();
		}


		FTransform SpawnTM = FTransform(ProjRotation, HandLocation);
		// 生成子弹，生成位置是手指方向，并传入发起者
		GetWorld()->SpawnActor<AActor>(classToSpawn, SpawnTM, SpawnParams);
	}
}

void ASCharacter::PrimaryInteract()
{
	// 添加交互组件
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}


