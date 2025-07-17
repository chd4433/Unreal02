#include "Notifies/CAnimNotify_Dead.h"
#include "Global.h"
#include "Chracters/CEnemy.h"

UCAnimNotify_Dead::UCAnimNotify_Dead()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor::Red;
#endif
}

FString UCAnimNotify_Dead::GetNotifyName_Implementation() const
{
	return "Dead";
}

void UCAnimNotify_Dead::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACEnemy* enemy = Cast<ACEnemy>(MeshComp->GetOwner());
	CheckNull(enemy);

	enemy->End_Dead();
}
