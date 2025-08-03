#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "CEnvQueryTest_OrbitDirection.generated.h"

/**
 * 
 */
UCLASS()
class U2505_98_API UCEnvQueryTest_OrbitDirection : public UEnvQueryTest
{
	GENERATED_BODY()
	

public:
	UCEnvQueryTest_OrbitDirection();

protected:
    virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;

public:
    /** 플레이어 Context와 현재 AI 위치를 기반으로 시계방향 점수 계산 */
    UPROPERTY(EditDefaultsOnly, Category = "Test")
    TSubclassOf<UEnvQueryContext> PlayerContext;

    /** 시계방향인지 반시계방향인지 선택 */
    UPROPERTY(EditDefaultsOnly, Category = "Test")
    bool bClockwise = true;
};
