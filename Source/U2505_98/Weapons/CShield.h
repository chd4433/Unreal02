#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CWeaponStructures.h"
#include "CShield.generated.h"

UCLASS()
class U2505_98_API ACShield 
	: public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* Box;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	class UAnimMontage* ShieldHitMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	float ShieldHitMontage_PlayRate = 1.0f;


private:
	UPROPERTY(EditDefaultsOnly, Category = "Parring")
	class UAnimMontage* ShieldParringMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Parring")
	float ShieldParringMontage_PlayRate = 2.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Parring")
	float ParringTime = 1.0f;

public:	
	ACShield();

public:
	FORCEINLINE bool DoShield() { return bDoShield; }
	FORCEINLINE bool CompleteShieldAnimation() { return bCompleteShield; }
	FORCEINLINE const float ShieldHealth() { return SaveHealth; }
	FORCEINLINE const bool GetCanParring() { return bCanParring; }

private:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	void Begin_shielded();
	void End_shielded();
	void Destroy_Shield();

public:
	void PlayShieldHittedAnimation();

public:
	void PlayShieldParringAnimation();
	void SendParringDamage(class ACharacter* InOwner, class ACharacter* InAttacker, bool bFirstHit = true);

public:
	bool CheckAttackerSword(class ACSword* InValue);
	void DeleteAttackerSword(class ACSword* InValue);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
private:
	class ACharacter* OwnerCharacter;

private:
	float SaveHealth;

private:
	bool bDoShield;
	bool bCompleteShield;

private:
	FDamagedData ParringDamagedData;
	float ParringCurrentTime;
	bool bCanParring;

private:
	TMap<class ACSword*, bool> SwordOverlapMap;


};
