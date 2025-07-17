#include "Notifies/CAnimNotify_Collision.h"
#include "Global.h"
#include "Weapons/ISword.h"

UCAnimNotify_Collision::UCAnimNotify_Collision()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor::Green;
#endif
}

FString UCAnimNotify_Collision::GetNotifyName_Implementation() const
{
	return UCAnimNotify_Structures::GetNotifyName("Collision", Type);
}

void UCAnimNotify_Collision::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	IISword* sword = Cast<IISword>(MeshComp->GetOwner());
	CheckNull(sword);

	switch (Type)
	{
	case EAnimNotify_Flow::Begin: sword->Begin_Collision(); return;
	case EAnimNotify_Flow::End: sword->End_Collision(); return;
	}
}
