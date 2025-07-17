#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
	class UCapsuleComponent* Capsule;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	class UAnimMontage* ShieldMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	float ShieldMontage_PlayRate = 1.0f;

public:	
	ACShield();

private:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	void Begin_shielded();
	void End_shielded();

protected:
	virtual void BeginPlay() override;

private:
	class ACharacter* OwnerCharacter;
};
