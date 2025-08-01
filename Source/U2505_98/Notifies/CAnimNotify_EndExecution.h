#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_EndExecution.generated.h"

UCLASS()
class U2505_98_API UCAnimNotify_EndExecution : public UAnimNotify
{
	GENERATED_BODY()

public:
	UCAnimNotify_EndExecution();

private:
	FString GetNotifyName_Implementation() const override;
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
};
