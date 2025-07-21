#include "Weapons/CShield.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Animation/AnimMontage.h"

#include "Chracters/CAnimInstance.h"
#include "CSword.h"


ACShield::ACShield()
{
	PrimaryActorTick.bCanEverTick = true;

	FHelpers::CreateComponent<UStaticMeshComponent>(this, &Mesh, "Mesh");
	FHelpers::CreateComponent<UBoxComponent>(this, &Box, "Box", Mesh);


	UStaticMesh* mesh;
	FHelpers::GetAsset<UStaticMesh>(&mesh, "/ Script / Engine.StaticMesh'/Game/ShieldAsset/Mesh/Weapons/Weapons_Kit/SM_Shield.SM_Shield'");
	Mesh->SetStaticMesh(mesh);

	Box->SetBoxExtent(FVector(50, 8, 60));

	Mesh->SetCollisionProfileName("NoCollision");
	Box->SetCollisionProfileName("NoCollision");

	FHelpers::GetAsset<UAnimMontage>(&ShieldHitMontage, "/Script/Engine.AnimMontage'/Game/Montages/Shield/Shield_Hit_Montage.Shield_Hit_Montage'");

	ParringCurrentTime = 0.0f;
	SaveHealth = 5.0f;
}

void ACShield::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(OtherActor == OwnerCharacter);

	ACSword* sword = Cast<ACSword>(OtherActor);
	CheckNull(sword);

	if (!SwordOverlapMap.Contains(sword))
	{
		SwordOverlapMap.Add(sword, true);
	}

}

void ACShield::Begin_shielded()
{
	CheckNull(OwnerCharacter);

	bDoShield = true;
	Box->SetCollisionProfileName("OverlapAllDynamic");

}

void ACShield::End_shielded()
{
	CheckNull(OwnerCharacter);

	bDoShield = false;
	ParringCurrentTime = 0.0f;
	bCanParring = false;
	Box->SetCollisionProfileName("NoCollision");
}

void ACShield::PlayShieldHittedAnimation()
{
	CheckNull(OwnerCharacter);

	OwnerCharacter->PlayAnimMontage(ShieldHitMontage, ShieldHitMontage_PlayRate);

}

bool ACShield::CheckAttackerSword(ACSword* InValue)
{
	if (SwordOverlapMap.Contains(InValue))
	{
		return SwordOverlapMap[InValue];
	}
	
	return false;
}

void ACShield::DeleteAttackerSword(ACSword* InValue)
{
	if (SwordOverlapMap.Contains(InValue))
	{
		SwordOverlapMap.Remove(InValue);
	}
}

void ACShield::BeginPlay()
{
   Super::BeginPlay();
   
   OwnerCharacter = Cast<ACharacter>(GetOwner());
   CheckNull(OwnerCharacter);

   AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), "Hand_Shield");

   Box->OnComponentBeginOverlap.AddDynamic(this, &ACShield::OnBeginOverlap);
}

void ACShield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	CheckNull(OwnerCharacter);
	UCAnimInstance* animInstance = Cast<UCAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());

	CheckNull(animInstance);
	
	bCompleteShield = animInstance->CompleteShieldAnimation();
	if (bCompleteShield)
	{
		ParringCurrentTime += DeltaTime;
		if (ParringCurrentTime <= ParringTime)
			bCanParring = true;
		else
			bCanParring = false;


	}
}

