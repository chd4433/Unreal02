#include "Weapons/CShield.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimMontage.h"

ACShield::ACShield()
{
	FHelpers::CreateComponent<UStaticMeshComponent>(this, &Mesh, "Mesh");
	FHelpers::CreateComponent<UCapsuleComponent>(this, &Capsule, "Capsule", Mesh);
	
	UStaticMesh* mesh;
	FHelpers::GetAsset<UStaticMesh>(&mesh, "/ Script / Engine.StaticMesh'/Game/ShieldAsset/Mesh/Weapons/Weapons_Kit/SM_Shield.SM_Shield'");
	Mesh->SetStaticMesh(mesh);

	Capsule->SetCapsuleHalfHeight(100.f);
	Capsule->SetCapsuleRadius(40.f);

	Mesh->SetCollisionProfileName("NoCollision");

	FHelpers::GetAsset<UAnimMontage>(&ShieldMontage, "/Script/Engine.AnimMontage'/Game/Montages/Deaths_Hit_Dazed_Montage.Deaths_Hit_Dazed_Montage'");
}

void ACShield::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ACShield::BeginPlay()
{
   Super::BeginPlay();
   
   OwnerCharacter = Cast<ACharacter>(GetOwner());
   CheckNull(OwnerCharacter);

   AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), "Hand_Shield");

   Capsule->OnComponentBeginOverlap.AddDynamic(this, &ACShield::OnBeginOverlap);
}

