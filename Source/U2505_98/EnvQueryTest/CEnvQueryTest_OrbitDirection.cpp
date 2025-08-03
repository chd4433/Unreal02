#include "EnvQueryTest/CEnvQueryTest_OrbitDirection.h"
#include "Global.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_VectorBase.h"
#include "EnvironmentQuery/Contexts/EnvQueryContext_Querier.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Actor.h"

UCEnvQueryTest_OrbitDirection::UCEnvQueryTest_OrbitDirection()
{
    Cost = EEnvTestCost::Low;
    ValidItemType = UEnvQueryItemType_VectorBase::StaticClass();
    TestPurpose = EEnvTestPurpose::Score;
    //bUsesContext = true;
}

void UCEnvQueryTest_OrbitDirection::RunTest(FEnvQueryInstance& QueryInstance) const
{
    UObject* QueryOwner = QueryInstance.Owner.Get();
    if (!QueryOwner) return;

    // 플레이어 위치 가져오기
    FVector PlayerLocation = FVector::ZeroVector;
    TArray<AActor*> ContextActors;
    QueryInstance.PrepareContext(PlayerContext, ContextActors);
    if (ContextActors.Num() > 0)
    {
        PlayerLocation = ContextActors[0]->GetActorLocation();
    }

    // AI 현재 위치
    AActor* QuerierActor = Cast<AActor>(QueryOwner);
    FVector AI_Location = QuerierActor ? QuerierActor->GetActorLocation() : FVector::ZeroVector;

    // AI 방향 벡터
    FVector CurrentDir = (AI_Location - PlayerLocation).GetSafeNormal2D();
    float CurrentAngle = FMath::Atan2(CurrentDir.Y, CurrentDir.X);

    // 각 후보 점 평가
    for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
    {
        FVector Candidate = GetItemLocation(QueryInstance, It.GetIndex());
        FVector CandidateDir = (Candidate - PlayerLocation).GetSafeNormal2D();
        float CandidateAngle = FMath::Atan2(CandidateDir.Y, CandidateDir.X);

        // 시계방향(+) 또는 반시계방향(-)으로 회전했을 때의 각도 차이
        float DeltaAngle = FMath::FindDeltaAngleRadians(CurrentAngle, CandidateAngle);
        if (!bClockwise)
        {
            DeltaAngle *= -1.f;
        }

        // 0보다 큰 값(앞쪽 방향)일수록 높은 점수
        bool Score = (DeltaAngle > 0) ? true : false;
        It.SetScore(TestPurpose, FilterType, Score, false);
    }
}
