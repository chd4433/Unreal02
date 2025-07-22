#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAIGroup.generated.h"

UCLASS()
class U2505_98_API ACAIGroup : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Settings")
	TArray<TSubclassOf<class ACEnemy>> SpawnEnemies;

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (ClampMin = "0"))
	uint8 EnemyCount = 0;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float SpawnSpacing = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bDebugSpawn = true;

public:	
	ACAIGroup();

protected:
	virtual void BeginPlay() override;

public:
	void GoToLocation_AllEnemies(FVector Location);

private:
	FVector2D GetRandomPointMinMax(float MinRadius, float MaxRadius);

private:
#if WITH_EDITOR
	void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
	TArray<class ACEnemy_AI*> Enemies_AI;

private:
	bool bFirstHitted;
};
