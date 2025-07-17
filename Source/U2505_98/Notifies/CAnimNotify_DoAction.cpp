#include "Notifies/CAnimNotify_DoAction.h"
#include "Global.h"
#include "Weapons/ISword.h"

UCAnimNotify_DoAction::UCAnimNotify_DoAction()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor::Cyan;
#endif
}

FString UCAnimNotify_DoAction::GetNotifyName_Implementation() const
{
	return UCAnimNotify_Structures::GetNotifyName("DoAction", Type);
}

void UCAnimNotify_DoAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	IISword* sword = Cast<IISword>(MeshComp->GetOwner());
	CheckNull(sword);

	switch (Type)
	{
	case EAnimNotify_Flow::Begin: sword->Begin_DoAction(); return;
	case EAnimNotify_Flow::End: sword->End_DoAction(); return;
	}
}
