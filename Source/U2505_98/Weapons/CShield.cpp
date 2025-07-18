#include "Weapons/CShield.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Animation/AnimMontage.h"

#include "Chracters/CAnimInstance.h"


ACShield::ACShield()
{
	PrimaryActorTick.bCanEverTick = true;

	FHelpers::CreateComponent<UStaticMeshComponent>(this, &Mesh, "Mesh");
	FHelpers::CreateComponent<UBoxComponent>(this, &Box, "Box", Mesh);


	UStaticMesh* mesh;
	FHelpers::GetAsset<UStaticMesh>(&mesh, "/ Script / Engine.StaticMesh'/Game/ShieldAsset/Mesh/Weapons/Weapons_Kit/SM_Shield.SM_Shield'");
	Mesh->SetStaticMesh(mesh);

	Box->SetBoxExtent(FVector(80, 8, 100));

	Mesh->SetCollisionProfileName("NoCollision");

	FHelpers::GetAsset<UAnimMontage>(&ShieldHitMontage, "/Script/Engine.AnimMontage'/Game/Montages/Shield/Shield_Hit_Montage.Shield_Hit_Montage'");

	ParringCurrentTime = 0.0f;
	SaveHealth = 10.0f;
}

void ACShield::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ACShield::Begin_shielded()
{
	CheckNull(OwnerCharacter);

	bDoShield = true;

}

void ACShield::End_shielded()
{
	CheckNull(OwnerCharacter);

	bDoShield = false;
	ParringCurrentTime = 0.0f;
	bCanParring = false;
}

void ACShield::PlayShieldHittedAnimation()
{
	CheckNull(OwnerCharacter);

	OwnerCharacter->PlayAnimMontage(ShieldHitMontage, ShieldHitMontage_PlayRate);

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

