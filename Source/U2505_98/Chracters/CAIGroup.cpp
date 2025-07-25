#include "Chracters/CAIGroup.h"
#include "Global.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "CAIController.h"
#include "CAIStructures.h"
#include "CEnemy.h"
#include "CEnemy_AI.h"

ACAIGroup::ACAIGroup()
{
	PrimaryActorTick.bCanEverTick = true;
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
		   int32 FrontSpacing = FMath::RandRange(100, 300);
		   int32 code = i % 2 == 0 ? (1) : (-1);
		   SpawnLocation.Y += static_cast<double>(code * FrontSpacing);
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
			   Enemies_AI.Add(enemy_Ai);
			   enemy_Ai->SetAiGroupManager(this);
			   enemy_Ai->AutoPossessAI = EAutoPossessAI::Spawned;
		   }


	   }
   }
   
   
}

void ACAIGroup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!!FightingGroup)
	{
		bool bAllDead = true;
		for (ACEnemy_AI* enemy : FightingGroup->GetEnemiesArray())
			if (enemy->GetDead() == false)
			{
				bAllDead = false;
				break;
			}
		
		if (bAllDead)
		{
			FightingGroup = nullptr;
			bFirstHitted = false;
			for (ACEnemy_AI* enemy : Enemies_AI)
				enemy->InitializeGroupFighting();
		}
	}
}

void ACAIGroup::GoToLocation_AllEnemies(FVector Location)
{
	if (bFirstHitted == false)
	{
		for (ACEnemy_AI* enemy : Enemies_AI)
		{
			if (enemy->GetFirstHitted())
				continue;
			FVector2D rand = GetRandomPointMinMax(200, 500);
			FVector randLocation = FVector(Location.X + rand.X, Location.Y + rand.Y, Location.Z);
			//DrawDebugSphere(GetWorld(), randLocation, 25, 40, FColor::Green, false, 5.0f);
			ACAIController* controller = enemy->GetController<ACAIController>();
			CheckNull(controller);
			UBlackboardComponent* blackboard = controller->GetBlackboardComponent();
			CheckNull(blackboard);
			blackboard->SetValueAsEnum("AIState", (uint8)EAIStateType::GoToLocation);
			blackboard->SetValueAsVector("GoToLocation", randLocation);
		}
		bFirstHitted = true;
	}
}

FVector2D ACAIGroup::GetRandomPointMinMax(float MinRadius, float MaxRadius)
{
	float Angle = FMath::FRandRange(0.f, 2 * PI);

	float Radius = FMath::Sqrt(FMath::FRandRange(MinRadius * MinRadius, MaxRadius * MaxRadius));

	float X = Radius * FMath::Cos(Angle);
	float Y = Radius * FMath::Sin(Angle);

	return FVector2D(X, Y);
}

void ACAIGroup::FindFightingEnemy()
{
	if (!!FightingGroup)
	{
		for (ACEnemy_AI* enemy : Enemies_AI)
		{
			if (enemy->GetFinishGoToLocation())
			{
				if (enemy->GetCountEnemy() == nullptr)
				{
					ACEnemy_AI* ClosestEnemy = nullptr;
					float ClosestDistance = TNumericLimits<float>::Max();

					for (ACEnemy_AI* FightingEnemy : FightingGroup->GetEnemiesArray())
					{
						if (FightingEnemy->GetCountEnemy() == nullptr && FightingEnemy->GetDead() == false)
						{
							float Distance = FVector::Dist(enemy->GetActorLocation(), FightingEnemy->GetActorLocation());

							if (Distance < ClosestDistance)
							{
								ClosestDistance = Distance;
								ClosestEnemy = FightingEnemy;
							}
						}
					}//for FightingGroup->GetEnemiesArray()

					if (!!ClosestEnemy)
					{
						enemy->SetCountEnemy(ClosestEnemy);
						ClosestEnemy->SetCountEnemy(enemy);
					}
				}
			}
		}//for Enemies_AI

	}
}

void ACAIGroup::RemoveEnemy(ACEnemy_AI* InEnemy)
{
	CheckFalse(InEnemy->GetDead());

	Enemies_AI.Remove(InEnemy);
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
