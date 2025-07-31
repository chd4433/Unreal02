// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class U2505_98_API UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Settings")
	float Speed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Settings")
	bool bFalling;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Settings")
	float Direction;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Settings")
	bool bHit_Air;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Settings")
	bool bPlayerAttackJump;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Settings")
	bool bPlayer;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Settings")
	bool bDownAttack;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Settings")
	bool bEquip_Sword;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Settings")
	bool bDo_Shield;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Settings")
	float ShieldAnimationBlendWeight;

public:
	FORCEINLINE bool CompleteShieldAnimation() { return ShieldAnimationBlendWeight >= 1.0f; }

public:
	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	class ACharacter* OwnerCharacter;
	

private:
	float BlendSpeed = 1.0f / 0.1f;
	
};
