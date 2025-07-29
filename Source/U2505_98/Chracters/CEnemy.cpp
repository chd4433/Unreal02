#include "Chracters/CEnemy.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkinnedMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Animation/AnimMontage.h"
#include "Engine/DataTable.h"

#include "CAnimInstance.h"
#include "CEnemy_AI.h"
#include "Weapons/CWeaponStructures.h"
#include "Widgets/CUserWidget_Enemy.h"


ACEnemy::ACEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	
	USkeletalMesh* mesh;
	FHelpers::GetAsset(&mesh, "/Script/Engine.SkeletalMesh'/Game/ModularSkeletonArmy/CharacterParts_Skeleton/Meshes/UE5_Skeleton/SK_ma_tal_nrw_skeleton_base_UE5.SK_ma_tal_nrw_skeleton_base_UE5'");
	GetMesh()->SetSkeletalMesh(mesh);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	FHelpers::CreateComponent<USkeletalMeshComponent>(this, &Hand_Mesh, "Hand_Mesh", GetCapsuleComponent());
	FHelpers::CreateComponent<USkeletalMeshComponent>(this, &Armour_Mesh, "Armour_Mesh", GetCapsuleComponent());
	FHelpers::CreateComponent<USkeletalMeshComponent>(this, &Boots_Mesh, "Boots_Mesh", GetCapsuleComponent());
	FHelpers::CreateComponent<USkeletalMeshComponent>(this, &Hat_Mesh, "Hat_Mesh", GetCapsuleComponent());
	Hand_Mesh->SetRelativeLocation(FVector(0, 0, -90));
	Hand_Mesh->SetRelativeRotation(FRotator(0, -90, 0));

	Armour_Mesh->SetRelativeLocation(FVector(0, 0, -90));
	Armour_Mesh->SetRelativeRotation(FRotator(0, -90, 0));

	Boots_Mesh->SetRelativeLocation(FVector(0, 0, -90));
	Boots_Mesh->SetRelativeRotation(FRotator(0, -90, 0));

	Hat_Mesh->SetRelativeLocation(FVector(0, 0, -90));
	Hat_Mesh->SetRelativeRotation(FRotator(0, -90, 0));


	TSubclassOf<UCAnimInstance> animInstance;
	FHelpers::GetClass<UCAnimInstance>(&animInstance, "/Script/Engine.AnimBlueprint'/Game/ABP_Character.ABP_Character_C'");
	GetMesh()->SetAnimClass(animInstance);

	USkeletalMesh* hand_Mesh;
	FHelpers::GetAsset<USkeletalMesh>(&hand_Mesh, "/Script/Engine.SkeletalMesh'/Game/ModularSkeletonArmy/CharacterParts_Skeleton/Meshes/UE5_Skeleton/Bracers_gloves/SK_ma_tal_nrw_skeleton_bracers_03.SK_ma_tal_nrw_skeleton_bracers_03'");
	Hand_Mesh->SetSkeletalMesh(hand_Mesh);

	USkeletalMesh* armour_Mesh;
	FHelpers::GetAsset<USkeletalMesh>(&armour_Mesh, "/Script/Engine.SkeletalMesh'/Game/ModularSkeletonArmy/CharacterParts_Skeleton/Meshes/UE5_Skeleton/Chest/SK_ma_tal_nrw_skeleton_chest_04.SK_ma_tal_nrw_skeleton_chest_04'");
	Armour_Mesh->SetSkeletalMesh(armour_Mesh);

	USkeletalMesh* boots_Mesh;
	FHelpers::GetAsset<USkeletalMesh>(&boots_Mesh, "/Script/Engine.SkeletalMesh'/Game/ModularSkeletonArmy/CharacterParts_Skeleton/Meshes/UE5_Skeleton/Boots/SK_ma_tal_nrw_skeleton_shoe_02.SK_ma_tal_nrw_skeleton_shoe_02'");
	Boots_Mesh->SetSkeletalMesh(boots_Mesh);

	USkeletalMesh* hat_Mesh;
	FHelpers::GetAsset<USkeletalMesh>(&hat_Mesh, "/Script/Engine.SkeletalMesh'/Game/ModularSkeletonArmy/CharacterParts_Skeleton/Meshes/UE5_Skeleton/Helm/SK_ma_tal_nrw_skeleton_helmet_01_a.SK_ma_tal_nrw_skeleton_helmet_01_a'");
	Hat_Mesh->SetSkeletalMesh(hat_Mesh);

	FHelpers::GetAsset<UAnimMontage>(&DeadMontage, "/Script/Engine.AnimMontage'/Game/Montages/Deaths_Hit_Dazed_Montage.Deaths_Hit_Dazed_Montage'");

	FHelpers::CreateComponent<UWidgetComponent>(this, &Widget, "Widget", GetCapsuleComponent());
	Widget->SetRelativeLocation(FVector(0, 0, 110));
	Widget->SetWidgetSpace(EWidgetSpace::World);

	FHelpers::GetClass<UCUserWidget_Enemy>(&UI_EnemyClass, "/Script/UMGEditor.WidgetBlueprint'/Game/Widgets/WB_Enemy.WB_Enemy_C'");
	Widget->SetWidgetClass(UI_EnemyClass);	

	//UpperCapsule
	FHelpers::CreateComponent<UCapsuleComponent>(this, &UpperCapsule, "UpperCapsule", GetCapsuleComponent());

	UpperCapsule->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f));
	UpperCapsule->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	UpperCapsule->SetCapsuleHalfHeight(88.0f);
	UpperCapsule->SetCapsuleRadius(34.0f);
	UpperCapsule->SetCollisionProfileName("NoCollision");
}

void ACEnemy::BeginPlay()
{
	SetMaterialChange(GetMesh());
	SetMaterialChange(Hand_Mesh);
	SetMaterialChange(Armour_Mesh);
	SetMaterialChange(Boots_Mesh);
	SetMaterialChange(Hat_Mesh);

	Super::BeginPlay();

	Health = MaxHealth;

	UI_Enemy = Cast<UCUserWidget_Enemy>(Widget->GetUserWidgetObject());
	UI_Enemy->UpdateHealth(Health, MaxHealth);
	UI_Enemy->UpdateName(GetName());

}

void ACEnemy::OnConstruction(const FTransform& Transform)
{
	Hand_Mesh->SetLeaderPoseComponent(GetMesh());
	Armour_Mesh->SetLeaderPoseComponent(GetMesh());
	Boots_Mesh->SetLeaderPoseComponent(GetMesh());
	Hat_Mesh->SetLeaderPoseComponent(GetMesh());
}

void ACEnemy::SetMaterialChange(USkeletalMeshComponent* meshComp)
{
	TArray<UMaterialInterface*> materials = meshComp->GetMaterials();
	for (int32 i = 0; i < materials.Num(); i++)
	{
		UMaterialInstanceDynamic* material = UMaterialInstanceDynamic::Create(materials[i], this);
		meshComp->SetMaterial(i, material);

		Materials.Add(material);
	}
}

void ACEnemy::ChangeColor(const FLinearColor& InColor)
{
	for (UMaterialInstanceDynamic* material : Materials)
		material->SetVectorParameterValue("Emissive Color", InColor);
}



void ACEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckNull(Widget);
	FTransform transform = Widget->GetComponentToWorld();

	APlayerCameraManager* cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	CheckNull(cameraManager);

	FVector cameraLocation = cameraManager->GetTransformComponent()->GetComponentLocation();

	FRotator rotator = UKismetMathLibrary::FindLookAtRotation(transform.GetLocation(), cameraLocation);
	Widget->SetWorldRotation(rotator);

}


float ACEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Health -= damage;
	Health = FMath::Clamp<float>(Health, 0, MaxHealth);
	UI_Enemy->UpdateHealth(Health, MaxHealth);

	if (Health <= 0.0f)
	{
		Dead();

		return 0.0f;
	}


	ACharacter* attacker = Cast<ACharacter>(EventInstigator->GetPawn());

	Damaged((FDamagedDataEvent*)&DamageEvent, attacker);

	return damage;
}

void ACEnemy::Damaged(FDamagedDataEvent* InEvent, ACharacter* InAttacker)
{
	ChangeColor(FLinearColor::Red);


	FTimerDelegate timerDelegate;
	timerDelegate.BindLambda([&]()
		{
			ChangeColor(OriginColor);
		});

	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, timerDelegate, 0.2f, false);


	CheckNull(InEvent);

	FDamagedData* data = InEvent->DamagedData;

	data->PlayHitMotion(this);


	FVector start = GetActorLocation();
	FVector target = InAttacker->GetActorLocation();
	FVector direction = target - start;
	direction.Normalize();

	if (data->Launch > 0.0f || data->bUpper)
	{
		FVector launchDistance = -direction * data->Launch;		
		if (data->bUpper)
		{
			bAirReaction = data->bUpper;
			FVector uppderDirection = FVector(0.0f, 0.0f, 1.0f) * data->UpperLaunch;
			uppderDirection.X = launchDistance.X;
			uppderDirection.Y = launchDistance.Y;
			LaunchCharacter(uppderDirection, false, false);

			SetChangeCollision(ECollisionType::Air);
		}
		else
			LaunchCharacter(launchDistance, false, false);

	}

	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));


	if (InEvent->bFirstHit)
	{
		data->PlayCameraShake(GetWorld());
		data->PlayHitStop(this, InAttacker);
	}

	FVector point;
	if (!!(InEvent->Collision))
	{
		if (InEvent->Collision->GetClosestPointOnCollision(GetActorLocation(), point) > 0.0f)
		{
			data->PlayEffect(this, point);
		}
	}
	else
	{
		data->PlayEffect(this, GetActorLocation());
	}
		
		
	

}

void ACEnemy::End_Damaged()
{
}

void ACEnemy::Dead()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PlayAnimMontage(DeadMontage, DeadMontage_PlayRate);

	if (!!Widget)
		Widget->DestroyComponent();
}

void ACEnemy::End_Dead()
{
	Destroy();
}

void ACEnemy::SetChangeCollision(ECollisionType InType)
{
	switch (InType)
	{
	case ECollisionType::Ground:
		UpperCapsule->SetCollisionProfileName("NoCollision");
		GetCapsuleComponent()->SetCollisionProfileName("Pawn");
		break;
	case ECollisionType::Air:
		UpperCapsule->SetCollisionProfileName("OverlapAllDynamic");
		GetCapsuleComponent()->SetCollisionProfileName("NoCollision");
		break;
	default:
		break;
	}
}

