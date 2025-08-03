#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAIGroup.generated.h"

enum class Group_State : uint8
{
	None = 0, GroupFighting, PlayerFighting, Max
};

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

	UPROPERTY(EditAnywhere, Category = "Settings")
	int PlayerAttackerCount = 2;

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bDebugSpawn = true;

public:
	FORCEINLINE const ACAIGroup* GetFightingGroup() { return FightingGroup; }
	FORCEINLINE void SetFightingGroup(ACAIGroup* InGroup) { FightingGroup = InGroup; }

	FORCEINLINE TArray<class ACEnemy_AI*> GetEnemiesArray() { return Enemies_AI; }

	FORCEINLINE const Group_State GetState() { return State; }
	FORCEINLINE void SetState(Group_State InState) { State = InState; }
public:	
	ACAIGroup();

protected:
	virtual void BeginPlay() override;

private:
	void Tick(float DeltaTime) override;

public:
	void GoToLocation_AllEnemies(FVector Location, Group_State InState);

private:
	FVector2D GetRandomPointMinMax(float MinRadius, float MaxRadius);

public:
	void FindFightingEnemy();
	void RemoveEnemy(class ACEnemy_AI* InEnemy);

public:
	void SetPlayerAttacker(class ACPlayer* InPlayer = nullptr);

private:
#if WITH_EDITOR
	void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
	TArray<class ACEnemy_AI*> Enemies_AI;

private:
	bool bFirstHitted;

private:
	ACAIGroup* FightingGroup;
	class ACPlayer* FightingPlayer;

private:
	Group_State State = Group_State::None;
};
