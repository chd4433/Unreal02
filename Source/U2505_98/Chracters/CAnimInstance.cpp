// Fill out your copyright notice in the Description page of Project Settings.


#include "Chracters/CAnimInstance.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapons/ISword.h"

void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	CheckNull(OwnerCharacter);
}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	CheckNull(OwnerCharacter);

	Speed = OwnerCharacter->GetVelocity().Size2D();

	FRotator rotation = OwnerCharacter->GetVelocity().ToOrientationRotator();
	FRotator rotation2 = OwnerCharacter->GetControlRotation();
	FRotator result = UKismetMathLibrary::NormalizedDeltaRotator(rotation, rotation2);
	Direction = result.Yaw;
	bFalling = OwnerCharacter->GetCharacterMovement()->IsFalling();

	IISword* sword = Cast<IISword>(OwnerCharacter);
	CheckNull(sword);

	bEquip_Sword = sword->IsEquipped();
}
