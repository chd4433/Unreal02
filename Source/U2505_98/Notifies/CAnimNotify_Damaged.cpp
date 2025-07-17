#include "Notifies/CAnimNotify_Damaged.h"
#include "Global.h"
#include "Chracters/CEnemy.h"

UCAnimNotify_Damaged::UCAnimNotify_Damaged()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor::Purple;
#endif
}

FString UCAnimNotify_Damaged::GetNotifyName_Implementation() const
{
	return "Damaged";
}

void UCAnimNotify_Damaged::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACEnemy* enemy = Cast<ACEnemy>(MeshComp->GetOwner());
	CheckNull(enemy);

	enemy->End_Damaged();
}
