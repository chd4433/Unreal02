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
    /** �÷��̾� Context�� ���� AI ��ġ�� ������� �ð���� ���� ��� */
    UPROPERTY(EditDefaultsOnly, Category = "Test")
    TSubclassOf<UEnvQueryContext> PlayerContext;

    /** �ð�������� �ݽð�������� ���� */
    UPROPERTY(EditDefaultsOnly, Category = "Test")
    bool bClockwise = true;
};
