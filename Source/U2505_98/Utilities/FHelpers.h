#pragma once

#include "CoreMinimal.h"

#define CheckTrue(x) { if(x == true) return; }
#define CheckTrueResult(x, y) { if(x == true) return y; }

#define CheckFalse(x) { if(x == false) return;}
#define CheckFalseResult(x, y) { if(x == false) return y;}

#define CheckNull(x) { if(x == nullptr) return; }
#define CheckNullResult(x, y) { if(x == nullptr) return y;}

#define CreateTextRenderComponent()\
{\
	FHelpers::CreateComponent<UTextRenderComponent>(this, &Text, "Text", Root);\
	Text->SetRelativeLocation(FVector(0, 0, 100));\
	Text->SetRelativeRotation(FRotator(0, 180, 0));\
	Text->SetRelativeScale3D(FVector(2));\
	Text->HorizontalAlignment = EHorizTextAligment::EHTA_Center;\
	Text->TextRenderColor = FColor::Red;\
	Text->Text = FText::FromString(GetName().Replace(L"Default__", L""));\
}

class U2505_98_API FHelpers
{
public:
	template<typename TValueType>
	static void CreateComponent(AActor* InActor, TValueType** OutComponent, FName InName, USceneComponent* InParent = nullptr, FName InSocketName = NAME_None)
	{
		*OutComponent = InActor->CreateDefaultSubobject<TValueType>(InName);

		if (!!InParent)
		{
			(*OutComponent)->SetupAttachment(InParent, InSocketName);

			return;
		}

		InActor->SetRootComponent(*OutComponent);
	}

	template<typename TValueType>
	static void CreateActorComponent(AActor* InActor, TValueType** OutComponent, FName InName)
	{
		*OutComponent = InActor->CreateDefaultSubobject<TValueType>(InName);
	}

	template<typename TValueType>
	static void GetAsset(TValueType** OutObject, FString InPath)
	{
		ConstructorHelpers::FObjectFinder<TValueType> asset(*InPath);
		*OutObject = asset.Object;
	}

	template<typename TValueType>
	static void GetAssetDynamic(TValueType** OutObject, FString InPath)
	{
		*OutObject = Cast<TValueType>(StaticLoadObject(TValueType::StaticClass(), nullptr, *InPath));
	}

	template<typename TClassName>
	static void GetClass(TSubclassOf<TClassName>* OutClass, FString InPath)
	{
		ConstructorHelpers::FClassFinder<TClassName> asset(*InPath);
		*OutClass = asset.Class;
	}

	template<typename TFindType>
	static TFindType* FindActor(UWorld* InWorld)
	{
		for (AActor* actor : InWorld->GetCurrentLevel()->Actors)
		{
			if (!!actor && actor->IsA<TFindType>())
				return Cast<TFindType>(actor);
		}

		return nullptr;
	}

	template<typename TFindType>
	static void FindActors(UWorld* InWorld, TArray<TFindType*>& OutActors)
	{
		for (AActor* actor : InWorld->GetCurrentLevel()->Actors)
		{
			if (!!actor && actor->IsA<TFindType>())
				OutActors.Add(Cast<TFindType>(actor));
		}
	}

	template<typename TFindType>
	static TFindType* GetComponent(AActor* InActor)
	{
		return Cast<TFindType>(InActor->GetComponentByClass(TFindType::StaticClass()));
	}
};
