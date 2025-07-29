#include "Weapons/CSword.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimMontage.h"
#include "Engine/DataTable.h"
#include "GenericTeamAgentInterface.h"

#include "IDamagable.h"
#include "Chracters/CPlayer.h"
#include "Chracters/CEnemy_AI.h"
#include "CShield.h"

ACSword::ACSword()
{
	FHelpers::CreateComponent<UStaticMeshComponent>(this, &Mesh, "Mesh");
	FHelpers::CreateComponent<UCapsuleComponent>(this, &Capsule, "Capsule", Mesh);

	

	UStaticMesh* mesh;
	FHelpers::GetAsset<UStaticMesh>(&mesh, "/Script/Engine.StaticMesh'/Game/MercenaryWarrior/Meshes/SM_greatsword.SM_greatsword'");
	Mesh->SetStaticMesh(mesh);

	Capsule->SetRelativeLocation(FVector(0.5f, 3.8f, -70.0f));
	Capsule->SetRelativeRotation(FRotator(179.0f, 0.0f, 0.0f));
	Capsule->SetCapsuleHalfHeight(50.f);
	Capsule->SetCapsuleRadius(7.f);

	Mesh->SetCollisionProfileName("NoCollision");
	//Capsule->SetCollisionProfileName("NoCollision");

	FHelpers::GetAsset<UDataTable>(&WeaponDataTable, "/Script/Engine.DataTable'/Game/Weapons/DT_PlayerAttack.DT_PlayerAttack'");

	//FHelpers::GetAsset<UDataTable>(&DamagedDataTable, "/Script/Engine.DataTable'/Game/Weapons/DT_DamagedData.DT_DamagedData'");
	FHelpers::GetAsset<UDataTable>(&DamagedDataTable, "/Script/Engine.DataTable'/Game/Weapons/DT_DamagedData.DT_DamagedData'");

}

void ACSword::SettingbyType(ACharacter* InCharacter)
{
	ACPlayer* Player = Cast<ACPlayer>(InCharacter);
	if (!!Player)
	{
		UStaticMesh* mesh;
		FHelpers::GetAssetDynamic<UStaticMesh>(&mesh, "/Script/Engine.StaticMesh'/Game/MercenaryWarrior/Meshes/SM_greatsword.SM_greatsword'");
		Mesh->SetStaticMesh(mesh);

		Capsule->SetRelativeLocation(FVector(0.5f, 3.8f, -70.0f));
		Capsule->SetRelativeRotation(FRotator(179.0f, 0.0f, 0.0f));
		Capsule->SetCapsuleHalfHeight(50.f);
		Capsule->SetCapsuleRadius(7.f);

		return;
	}

	ACEnemy_AI* Enemy_AI = Cast<ACEnemy_AI>(InCharacter);
	if (!!Enemy_AI)
	{
		UStaticMesh* mesh;
		FHelpers::GetAssetDynamic<UStaticMesh>(&mesh, "/Script/Engine.StaticMesh'/Game/ModularSkeletonArmy/CharacterParts_Skeleton/Meshes/Weapons/SM_wp_sword_01.SM_wp_sword_01'");
		Mesh->SetStaticMesh(mesh);

		Capsule->SetRelativeLocation(FVector(0.6f, 3.8f, 50.0f));
		Capsule->SetRelativeRotation(FRotator(180.0f, 0.0f, 180.0f));
		Capsule->SetCapsuleHalfHeight(50.f);
		Capsule->SetCapsuleRadius(7.f);

		return;
	}

}


void ACSword::OnEquip()
{
	if (bEquipped)
	{
		Unequip();

		return;
	}
	else
		Equip();


}

void ACSword::Equip()
{
	CheckTrue(bEquipping);
	bEquipping = true;

	OwnerCharacter->PlayAnimMontage(EquipData.Montage, EquipData.PlayRate);

}

void ACSword::Unequip()
{
	CheckTrue(bEquipping);
	bEquipping = true;

	OwnerCharacter->PlayAnimMontage(UnequipData.Montage, UnequipData.PlayRate);
}

void ACSword::Begin_Equip()
{
	if (bEquipped)
	{
		AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), "Back_Sword");
		return;
	}
	else
		AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), "Hand_Sword");
}

void ACSword::End_Equip()
{
	bEquipping = false;

	if (bEquipped)
	{
		bEquipped = false;

		OwnerCharacter->bUseControllerRotationYaw = false;
		OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;

	}
	else
	{
		bEquipped = true;

		ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
		if (player == nullptr)
		{
			OwnerCharacter->bUseControllerRotationYaw = true;
			OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
			return;
		}

		player->SetbRotation(true);
	}
}

void ACSword::DoAction(ESwordAttackType InType)
{
	CheckFalse(bEquipped);
	CheckTrue(bEquipping);

	if (bEnable)
	{
		bEnable = false;
		bExist = true;

		return;
	}

	CheckTrue(bAttacking);
	bAttacking = true;
	Index = static_cast<uint8>(InType);
	ActionDatas[Index].PlayMontage(OwnerCharacter);

	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	CheckNull(OwnerCharacter);
	if (ActionDatas[Index].bFixedCamera)
		player->OnFixedCamera();
}

void ACSword::Begin_DoAction()
{
	CheckFalse(bExist);
	bExist = false;

	ActionDatas[++Index].PlayMontage(OwnerCharacter);
}

void ACSword::End_DoAction()
{
	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	if(!!player)
		if (ActionDatas[Index].bFixedCamera)
			player->OffFixedCamera();

	bAttacking = false;
	Index = static_cast<uint8>(ESwordAttackType::BasicAttack_First);
	bEnable = false;
	bExist = false;

	if (FMath::IsNearlyZero(OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed))
		OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = 400;

	for (ACShield* shield : Shields)
	{
		shield->DeleteAttackerSword(this);
	}
	Shields.Empty();
}

void ACSword::Begin_Combo()
{
	bEnable = true;
}

void ACSword::End_Combo()
{
	bEnable = false;
}

void ACSword::Begin_Collision()
{
	Hitted.Empty();

	Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

}

void ACSword::End_Collision()
{
	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (ActionDatas[Index].bFixedCamera == false)
	{
		Hitted.Empty();

		return;
	}

	float angle = FLT_MIN;

	FVector location = OwnerCharacter->GetActorLocation();

	for (IIDamagable* idamage : Hitted)
	{
		ACharacter* character = Cast<ACharacter>(idamage);
		if (character == nullptr)
			continue;
		FVector direction = character->GetActorLocation() - location;
		direction = direction.GetUnsafeNormal2D();

		FVector forward = OwnerCharacter->GetActorForwardVector();
		float dot = FVector::DotProduct(direction, forward);

		if (dot < HittedAngle || dot < angle)
			continue;

		angle = dot;
		Candidate = character;
	}

	Hitted.Empty();
}

void ACSword::Destroy_Sword()
{
	Destroy();
}

void ACSword::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(OtherActor == OwnerCharacter);

	CheckNull(TeamIdOwnerCharacter);

	IGenericTeamAgentInterface* teamIdOtherActor = Cast<IGenericTeamAgentInterface>(OtherActor);
	CheckNull(teamIdOtherActor);

	CheckTrue(TeamIdOwnerCharacter->GetGenericTeamId() == teamIdOtherActor->GetGenericTeamId());

	ACShield* shield = Cast<ACShield>(OtherActor);
	if (!!shield)
	{
		Shields.AddUnique(shield);
	}


	IIDamagable* other = Cast<IIDamagable>(OtherActor);
	CheckNull(other);

	CheckFalse(Hitted.Find(other) == INDEX_NONE);
	Hitted.AddUnique(other);

	CheckFalse(Index < (int32)DamagedDatas.Num());


	ACharacter* attacker = Cast<ACharacter>(GetOwner());
	UShapeComponent* collision = Cast<UShapeComponent>(OverlappedComponent);
	ACharacter* ohterCharacter = Cast<ACharacter>(OtherActor);
	


	DamagedDatas[Index].SendDamage(attacker, this, collision, ohterCharacter, Hitted.Num() <= 1);
}

void ACSword::BeginPlay()
{
   Super::BeginPlay();
   
   OwnerCharacter = Cast<ACharacter>(GetOwner());
   CheckNull(OwnerCharacter);

   TeamIdOwnerCharacter = Cast<IGenericTeamAgentInterface>(OwnerCharacter);

   AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), "Back_Sword");

   //WeaponDataTable
   {
	   ensure(WeaponDataTable != nullptr);
	   CheckNull(WeaponDataTable);

	   TArray<FWeaponData*> rows;
	   WeaponDataTable->GetAllRows("", rows);

	   for (int i = 0; i < rows.Num(); i++)
	   {
		   switch (rows[i]->Type)
		   {
		   case EWeaponDataType::Equip:
			   EquipData = *rows[i];
			   break;

		   case EWeaponDataType::Unequip:
			   UnequipData = *rows[i];
			   break;

		   case EWeaponDataType::Attack:
			   ActionDatas.Add(*rows[i]);
			   break;
		   }
	   }//for(i)
   }

   //DamagedDataTable
   {
	   ensure(DamagedDataTable != nullptr);
	   CheckNull(DamagedDataTable);

	   TArray<FDamagedData*> rows;
	   DamagedDataTable->GetAllRows("", rows);

	   for (int i = 0; i < rows.Num(); i++)
		   DamagedDatas.Add(*rows[i]);
   }


   End_Collision();

   Capsule->OnComponentBeginOverlap.AddDynamic(this, &ACSword::OnBeginOverlap);
}

void ACSword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckNull(Candidate);

	APlayerController* controller = OwnerCharacter->GetController<APlayerController>();
	CheckNull(controller);


	FRotator ownerToTarget = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), Candidate->GetActorLocation());
	FRotator controlRotation = OwnerCharacter->GetControlRotation();

	ownerToTarget.Pitch = controlRotation.Pitch;

	if (controlRotation.Equals(ownerToTarget, FinishHittedAngle))
	{
		Candidate = nullptr;
		controller->SetControlRotation(ownerToTarget);

		return;
	}


	ownerToTarget.Roll = controlRotation.Roll;
	FRotator result = UKismetMathLibrary::RInterpTo(controlRotation, ownerToTarget, DeltaTime, RotationSpeed);

	controller->SetControlRotation(result);
}


bool ACSword::IsAttached_Hand()
{
	FName name = GetAttachParentSocketName();


	return name.IsEqual("Hand_Sword");
}