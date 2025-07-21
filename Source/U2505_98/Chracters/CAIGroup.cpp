#include "Chracters/CAIGroup.h"
#include "Global.h"
#include "CEnemy.h"
#include "CEnemy_AI.h"

ACAIGroup::ACAIGroup()
{
   
}

void ACAIGroup::BeginPlay()
{
   Super::BeginPlay();

   const FVector BaseLocation = GetActorLocation();
   const FVector RightVector = GetActorRightVector();
   if (bDebugSpawn)
   {
	   for (int32 i = 0; i < SpawnEnemies.Num(); ++i)
	   {
		   if (SpawnEnemies[i] == nullptr)
			   continue;

		   FVector SpawnLocation = BaseLocation + RightVector * SpawnSpacing * i;
		   SpawnLocation.Z = BaseLocation.Z + 90;
		   FRotator SpawnRotation = GetActorRotation();

		   FActorSpawnParameters params;
		   params.Owner = this;
		   params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		   ACEnemy* SpawnedEnemy = GetWorld()->SpawnActor<ACEnemy>(
			   SpawnEnemies[i],
			   SpawnLocation,
			   SpawnRotation,
			   params
		   );


		   ACEnemy_AI* enemy_Ai = Cast<ACEnemy_AI>(SpawnedEnemy);
		   if (!!enemy_Ai)
		   {
			   enemy_Ai->SetAiGroupManager(this);
			   enemy_Ai->AutoPossessAI = EAutoPossessAI::Spawned;
		   }


	   }
   }
   
   
}


#if WITH_EDITOR
void ACAIGroup::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName name = (PropertyChangedEvent.Property != nullptr)
		? PropertyChangedEvent.Property->GetFName()
		: NAME_None;

	if (name == GET_MEMBER_NAME_CHECKED(ACAIGroup, EnemyCount))
	{
		const int32 NewCount = static_cast<int32>(EnemyCount);

		if (SpawnEnemies.Num() < NewCount)
		{
			SpawnEnemies.AddDefaulted(NewCount - SpawnEnemies.Num());
		}
		else if (SpawnEnemies.Num() > NewCount)
		{
			SpawnEnemies.SetNum(NewCount);
		}
	}
}
#endif
