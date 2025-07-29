// Fill out your copyright notice in the Description page of Project Settings.


#include "Chracters/CAnimInstance.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapons/ISword.h"
#include "Weapons/IShield.h"
#include "IHittedReaction.h"

void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	CheckNull(OwnerCharacter);

	ShieldAnimationBlendWeight = 0.0f;
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
	if(!!sword)
		bEquip_Sword = sword->IsEquipped();

	IIShield* shield = Cast<IIShield>(OwnerCharacter);
	if (!!shield)
	{
		bDo_Shield = shield->Do_shield();
		if (bDo_Shield)
		{
			if (ShieldAnimationBlendWeight < 1.0f)
			{
				ShieldAnimationBlendWeight += DeltaSeconds * BlendSpeed;
				ShieldAnimationBlendWeight = FMath::Clamp(ShieldAnimationBlendWeight, 0.0f, 1.0f);
			}
			
		}
		else
		{
			if (ShieldAnimationBlendWeight > 0.0f)
			{
				ShieldAnimationBlendWeight -= DeltaSeconds * BlendSpeed;
				ShieldAnimationBlendWeight = FMath::Clamp(ShieldAnimationBlendWeight, 0.0f, 1.0f);
			}
		}
	}
		
	IIHittedReaction* hittedReaction = Cast<IIHittedReaction>(OwnerCharacter);
	if (!!hittedReaction)
	{
		bHit_Air = hittedReaction->Air_Reaction();
		if (bHit_Air)
		{
			hittedReaction->SetAir_Reaction(OwnerCharacter->GetCharacterMovement()->IsFalling());
		}
	}
}
