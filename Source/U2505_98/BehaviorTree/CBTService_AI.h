// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_AI.generated.h"

UCLASS()
class U2505_98_API UCBTService_AI : public UBTService
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FName TargetKey = "Target";

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FName AIStateKey = "AIState";

	UPROPERTY(EditAnywhere, Category = "Action")
	float ActionDistance = 120.0f;

private:
	UPROPERTY(EditAnywhere, Category = "Mode")
	bool bPatrol = false;

public:
	UCBTService_AI();

protected:
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
