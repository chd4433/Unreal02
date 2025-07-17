#include "Notifies/CAnimNotify_Combo.h"
#include "Global.h"
#include "Weapons/ISword.h"


UCAnimNotify_Combo::UCAnimNotify_Combo()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor::Emerald;
#endif
}

FString UCAnimNotify_Combo::GetNotifyName_Implementation() const
{
	return UCAnimNotify_Structures::GetNotifyName("Combo", Type);
}

void UCAnimNotify_Combo::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	IISword* sword = Cast<IISword>(MeshComp->GetOwner());
	CheckNull(sword);

	switch (Type)
	{
	case EAnimNotify_Flow::Begin: sword->Begin_Combo(); return;
	case EAnimNotify_Flow::End: sword->End_Combo(); return;
	}
}
