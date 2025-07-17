#include "Notifies/CAnimNotify_Equip.h"
#include "Global.h"
#include "Weapons/ISword.h"

UCAnimNotify_Equip::UCAnimNotify_Equip()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor::Magenta;
#endif

}

FString UCAnimNotify_Equip::GetNotifyName_Implementation() const
{
	
	return UCAnimNotify_Structures::GetNotifyName("Equip", Type);
}

void UCAnimNotify_Equip::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	IISword* sword = Cast<IISword>(MeshComp->GetOwner());
	CheckNull(sword);

	switch (Type)
	{
	case EAnimNotify_Flow::Begin:
		sword->Begin_Equip();
		return;
	case EAnimNotify_Flow::End:
		sword->End_Equip();
		return;
	}


}
