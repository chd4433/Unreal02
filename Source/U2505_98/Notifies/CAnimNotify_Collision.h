
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_Structures.h"
#include "CAnimNotify_Collision.generated.h"

/**
 * 
 */
UCLASS()
class U2505_98_API UCAnimNotify_Collision : public UAnimNotify
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Type")
	EAnimNotify_Flow Type = EAnimNotify_Flow::Max;

public:
	UCAnimNotify_Collision();

private:
	FString GetNotifyName_Implementation() const override;
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
};
