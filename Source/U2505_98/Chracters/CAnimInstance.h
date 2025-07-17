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

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Settings")
	bool bEquip_Sword;

public:
	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	class ACharacter* OwnerCharacter;
	
};
