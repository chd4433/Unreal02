#include "Weapons/CWeaponStructures.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void FWeaponData::PlayMontage(ACharacter* InCharacter)
{
	if (bCanMove == false)
		InCharacter->GetCharacterMovement()->MaxWalkSpeed = 0;

	InCharacter->PlayAnimMontage(Montage, PlayRate);
}

void FDamagedData::SendDamage(ACharacter* InAttacker, AActor* InAttackCauser, UShapeComponent* InAttackCollision, ACharacter* InOther, bool bFirstHit)
{
	FDamagedDataEvent e;
	e.bFirstHit = bFirstHit;
	e.Collision = InAttackCollision;
	e.DamagedData = this;

	InOther->TakeDamage(Power, e, InAttacker->GetInstigatorController(), InAttackCauser);
}

void FDamagedData::PlayHitMotion(ACharacter* InOwnerCharacter)
{
	CheckNull(InOwnerCharacter);
	CheckNull(Montage);

	InOwnerCharacter->PlayAnimMontage(Montage, PlayRate);
}

void FDamagedData::PlayCameraShake(UWorld* InWorld)
{
	CheckNull(InWorld);
	CheckNull(CameraShake);

	APlayerCameraManager* cameraManager = UGameplayStatics::GetPlayerCameraManager(InWorld, 0);
	CheckNull(cameraManager);

	cameraManager->StartCameraShake(CameraShake);
}

void FDamagedData::PlayHitStop(ACharacter* InOwnerCharacter, ACharacter* InAttacker)
{
	CheckTrue(FMath::IsNearlyZero(StopTime));
	CheckNull(InOwnerCharacter);
	CheckNull(InAttacker);

	InOwnerCharacter->CustomTimeDilation = 1e-3f;
	InAttacker->CustomTimeDilation = 1e-3f;

	FTimerDelegate timerDelegate;
	timerDelegate.BindLambda([=]()
		{
			InOwnerCharacter->CustomTimeDilation = 1.0f;
			InAttacker->CustomTimeDilation = 1.0f;
		});

	FTimerHandle handle;
	InOwnerCharacter->GetWorld()->GetTimerManager().SetTimer(handle, timerDelegate, StopTime, false);
}

void FDamagedData::PlayEffect(ACharacter* InOwnerCharacter, const FVector& InClosetPoint)
{
	CheckNull(InOwnerCharacter);
	CheckNull(Effect);

	FVector location = InClosetPoint;
	FRotator rotator = InOwnerCharacter->GetActorRotation();

	location += rotator.RotateVector(EffectLocation);


	FTransform transform;
	transform.SetLocation(location);
	transform.SetScale3D(EffectScale);

	UGameplayStatics::SpawnEmitterAtLocation(InOwnerCharacter->GetWorld(), Effect, transform);
}
