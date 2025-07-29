#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "Engine/DamageEvents.h"
#include "CWeaponStructures.generated.h"

UENUM()
enum class EWeaponDataType
{
	Equip = 0, Unequip, Attack, Max,
};

USTRUCT()
struct FWeaponData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	EWeaponDataType Type = EWeaponDataType::Max;

	UPROPERTY(EditAnywhere)
	UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
	float PlayRate = 1.0f;

	UPROPERTY(EditAnywhere)
	bool bCanMove;

	UPROPERTY(EditAnywhere)
	bool bFixedCamera;

public:
	void PlayMontage(class ACharacter* InCharacter);
};


USTRUCT()
struct FDamagedData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
	float PlayRate = 1.0f;

	UPROPERTY(EditAnywhere)
	float Power;

	UPROPERTY(EditAnywhere)
	float Launch;

	UPROPERTY(EditAnywhere)
	float StopTime;

	UPROPERTY(EditAnywhere)
	class USoundWave* Sound = nullptr;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* Effect = nullptr;

	UPROPERTY(EditAnywhere)
	FVector EffectLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
	FVector EffectScale = FVector::OneVector;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCameraShakeBase> CameraShake = nullptr;

	UPROPERTY(EditAnywhere)
	bool bUpper;

	UPROPERTY(EditAnywhere)
	float UpperLaunch;

public:
	void SendDamage(class ACharacter* InAttacker, class AActor* InAttackCauser, class UShapeComponent* InAttackCollision, class ACharacter* InOther, bool bFirstHit);

	void PlayHitMotion(class ACharacter* InOwnerCharacter);
	void PlayCameraShake(UWorld* InWorld);
	void PlayHitStop(class ACharacter* InOwnerCharacter, class ACharacter* InAttacker);
	void PlayEffect(class ACharacter* InOwnerCharacter, const FVector& InClosetPoint);
};

USTRUCT()
struct FDamagedDataEvent : public FDamageEvent
{
	GENERATED_BODY()

public:
	bool bFirstHit;
	class UShapeComponent* Collision;
	FDamagedData* DamagedData;
};

UCLASS()
class U2505_98_API UCWeaponStructures : public UObject
{
	GENERATED_BODY()
	
};
