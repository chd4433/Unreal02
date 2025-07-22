#include "Chracters/CPlayer.h"
#include "Global.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"

#include "CAnimInstance.h"
#include "Weapons/CSword.h"
#include "Weapons/CShield.h"
#include "Widgets/CUserWidget_Player.h"

ACPlayer::ACPlayer()
{
	FHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetCapsuleComponent());
	FHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", SpringArm);
	FHelpers::CreateComponent<UStaticMeshComponent>(this, &Sworldsheath, "Sworldsheath", GetMesh(), "Back_Swordsheath");


	USkeletalMesh* mesh;
	FHelpers::GetAsset(&mesh, "/Script/Engine.SkeletalMesh'/Game/MercenaryWarrior/Meshes/SK_MercenaryWarrior_WithoutHelmet.SK_MercenaryWarrior_WithoutHelmet'");
	GetMesh()->SetSkeletalMesh(mesh);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	TSubclassOf<UCAnimInstance> animInstance;
	FHelpers::GetClass<UCAnimInstance>(&animInstance, "/Script/Engine.AnimBlueprint'/Game/ABP_Character.ABP_Character_C'");
	GetMesh()->SetAnimClass(animInstance);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 400.f;

	SpringArm->SetRelativeLocation(FVector(0, 0, 60));
	SpringArm->TargetArmLength = 200;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;

	FHelpers::GetClass<ACSword>(&SwordClass, "/Script/Engine.Blueprint'/Game/Weapons/BP_CSword.BP_CSword_C'");
	
	FHelpers::GetClass<ACShield>(&ShieldClass, "/Script/Engine.Blueprint'/Game/Weapons/BP_CShield.BP_CShield_C'");

	UStaticMesh* sworldsheath;
	FHelpers::GetAsset<UStaticMesh>(&sworldsheath, "/Script/Engine.StaticMesh'/Game/MercenaryWarrior/Meshes/SM_greatswordsheath.SM_greatswordsheath'");
	Sworldsheath->SetStaticMesh(sworldsheath);
	Sworldsheath->SetCollisionProfileName("NoCollision");

	FHelpers::GetClass<UCUserWidget_Player>(&UI_PlayerClass, "/Script/UMGEditor.WidgetBlueprint'/Game/Widgets/WB_Player.WB_Player_C'");

	FHelpers::GetAsset<UAnimMontage>(&DeadMontage, "/Script/Engine.AnimMontage'/Game/Montages/Deaths_Hit_Dazed_Montage.Deaths_Hit_Dazed_Montage'");
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	FTransform transform;
	FActorSpawnParameters params;
	params.Owner = this;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	Sword = GetWorld()->SpawnActor<ACSword>(SwordClass, transform, params);

	Shield = GetWorld()->SpawnActor<ACShield>(ShieldClass, transform, params);

	Health = MaxHealth;
	bCanMove = true;

	UI_Player = CreateWidget<UCUserWidget_Player>(GetController<APlayerController>(), UI_PlayerClass);
	UI_Player->AddToViewport();
	UI_Player->UpdateEquipped(IsEquipped());

}

FGenericTeamId ACPlayer::GetGenericTeamId() const
{
	return FGenericTeamId(TeamID);
}

void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACPlayer::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACPlayer::OnMoveRight);
	PlayerInputComponent->BindAxis("HorizontalLook", this, &ACPlayer::OnHorizontalLook);
	PlayerInputComponent->BindAxis("VerticalLook", this, &ACPlayer::OnVerticalLook);

	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Pressed, this, &ACPlayer::OnRun);
	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Released, this, &ACPlayer::OffRun);

	PlayerInputComponent->BindAction("Sword", EInputEvent::IE_Pressed, this, &ACPlayer::OnSword);
	PlayerInputComponent->BindAction("Action", EInputEvent::IE_Pressed, this, &ACPlayer::OnDoAction);

	PlayerInputComponent->BindAction("Shield", EInputEvent::IE_Pressed, this, &ACPlayer::Begin_shielded);
	PlayerInputComponent->BindAction("Shield", EInputEvent::IE_Released, this, &ACPlayer::End_shielded);
}

void ACPlayer::OnMoveForward(float InValue)
{
	CheckFalse(bCanMove);
	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetForwardVector();

	AddMovementInput(direction, InValue);
}

void ACPlayer::OnMoveRight(float InValue)
{
	CheckFalse(bCanMove);
	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetRightVector();

	AddMovementInput(direction, InValue);
}

void ACPlayer::OnHorizontalLook(float InValue)
{
	AddControllerYawInput(InValue);
}

void ACPlayer::OnVerticalLook(float InValue)
{
	AddControllerPitchInput(InValue);
}

void ACPlayer::OnRun()
{
	CheckFalse(bCanMove);
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

void ACPlayer::OffRun()
{
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
}

float ACPlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (!!Shield)
	{
		ACSword* sword = Cast<ACSword>(DamageCauser);
		if (!!sword && Shield->CheckAttackerSword(sword))
		{
			damage -= Shield->ShieldHealth();
			if (damage <= 0.0f)
				damage = 0.0f;
		}
	}

	FLog::Print(damage);
	Health -= damage;
	Health = FMath::Clamp<float>(Health, 0, MaxHealth);
	UI_Player->UpdateHealth(Health, MaxHealth);

	if (Health <= 0.0f)
	{
		Dead();

		return 0.0f;
	}


	ACharacter* attacker = Cast<ACharacter>(EventInstigator->GetPawn());

	Damaged((FDamagedDataEvent*)&DamageEvent, attacker);

	return damage;
}

void ACPlayer::Damaged(FDamagedDataEvent* InEvent, ACharacter* InAttacker)
{
	CheckNull(InEvent);

	FDamagedData* data = InEvent->DamagedData;

	if (Shield->CompleteShieldAnimation())
		Shield->PlayShieldHittedAnimation();
	else
		data->PlayHitMotion(this);



	if (InEvent->bFirstHit)
	{
		data->PlayHitStop(this, InAttacker);
	}

	FVector point;
	if (InEvent->Collision->GetClosestPointOnCollision(GetActorLocation(), point) > 0.0f)
	{
		data->PlayEffect(this, point);
	}

	bCanMove = false;
}

void ACPlayer::End_Damaged()
{
	bCanMove = true;
	CheckNull(Sword);
	if (Sword->IsAttacking())
		Sword->End_DoAction();
}

void ACPlayer::Dead()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PlayAnimMontage(DeadMontage, DeadMontage_PlayRate);

	if (!!UI_Player)
	{
		UI_Player->RemoveFromParent();
		UI_Player = nullptr;
	}
}

void ACPlayer::End_Dead()
{
	Destroy();

	Destroy_Sword();
	Destroy_Shield();
}

void ACPlayer::OnSword()
{
	CheckNull(Sword);
	Sword->OnEquip();
}

bool ACPlayer::IsEquipped()
{
	CheckNullResult(Sword, false);

	return Sword->IsEquipped();
}

bool ACPlayer::IsEquipping()
{
	CheckNullResult(Sword, false);

	return Sword->IsEquipping();
}

bool ACPlayer::IsAttacking()
{
	CheckNullResult(Sword, false);

	return Sword->IsAttacking();
}

bool ACPlayer::IsAttached_Hand()
{
	CheckNullResult(Sword, false);

	return Sword->IsAttached_Hand();
}

void ACPlayer::Begin_Equip()
{
	CheckNull(Sword);
	Sword->Begin_Equip();
}

void ACPlayer::End_Equip()
{
	CheckNull(Sword);
	Sword->End_Equip();

	UI_Player->UpdateEquipped(IsEquipped());
}

void ACPlayer::OnDoAction()
{
	CheckNull(Sword);

	Sword->DoAction();
}

void ACPlayer::Begin_DoAction()
{
	CheckNull(Sword);

	Sword->Begin_DoAction();
}

void ACPlayer::End_DoAction()
{
	CheckNull(Sword);
	bCanMove = true;
	Sword->End_DoAction();
}

void ACPlayer::Begin_Combo()
{
	CheckNull(Sword);

	Sword->Begin_Combo();
}

void ACPlayer::End_Combo()
{
	CheckNull(Sword);

	Sword->End_Combo();
}

void ACPlayer::Begin_Collision()
{
	CheckNull(Sword);

	Sword->Begin_Collision();
}

void ACPlayer::End_Collision()
{
	CheckNull(Sword);

	Sword->End_Collision();
}

void ACPlayer::Destroy_Sword()
{
	CheckNull(Sword);

	Sword->Destroy_Sword();
	Sword = nullptr;
}

bool ACPlayer::Do_shield()
{
	CheckNullResult(Shield, false);

	return Shield->DoShield();
}

void ACPlayer::Begin_shielded()
{
	CheckNull(Shield);

	Shield->Begin_shielded();
}

void ACPlayer::End_shielded()
{
	CheckNull(Shield);

	Shield->End_shielded();
}

void ACPlayer::Destroy_Shield()
{
	CheckNull(Shield);

	Shield->Destroy_Shield();
	Shield = nullptr;
}

