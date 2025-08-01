#include "Chracters/CMotionWarpingComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"

UCMotionWarpingComponent::UCMotionWarpingComponent(const FObjectInitializer& ObjectInitializer)
	: UMotionWarpingComponent(ObjectInitializer)
{

}

void UCMotionWarpingComponent::DoMotionWarping()
{
	CheckFalse(HitActor == nullptr);
	CheckNull(OwnerCharacter);

	LineTrace();



}

void UCMotionWarpingComponent::LineTrace()
{

	FVector start = OwnerCharacter->GetActorLocation();
	FVector end = start + OwnerCharacter->GetActorForwardVector() * TraceDistance;

	ETraceTypeQuery traceType = ETraceTypeQuery::TraceTypeQuery3;

	TArray<AActor*> ignores;
	ignores.Add(OwnerCharacter);


	EDrawDebugTrace::Type type = EDrawDebugTrace::None;

	if (bDebug)
		type = EDrawDebugTrace::ForDuration;

	FHitResult hitResult;
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end, traceType, false, ignores, type, hitResult, true, FLinearColor::Red, FColor::White);

	//찾는 조건 
	HitActor = hitResult.GetActor();
}
