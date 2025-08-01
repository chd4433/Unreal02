#include "Notifies/CAnimNotify_EndExecution.h"
#include "Global.h"
#include "Chracters/CPlayer.h"

UCAnimNotify_EndExecution::UCAnimNotify_EndExecution()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor::Orange;
#endif
}

FString UCAnimNotify_EndExecution::GetNotifyName_Implementation() const
{
	return "EndExecution";
}

void UCAnimNotify_EndExecution::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	if (!!player)
	{
		player->EndExecution();
		return;
	}
}
