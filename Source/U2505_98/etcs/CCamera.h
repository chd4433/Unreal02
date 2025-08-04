#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CCamera.generated.h"

UCLASS()
class U2505_98_API ACCamera : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

public:	
	ACCamera();

protected:
	virtual void BeginPlay() override;
};
