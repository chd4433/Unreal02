#include "Etcs/CCamera.h"
#include "Global.h"
#include "Camera/CameraComponent.h"

ACCamera::ACCamera()
{
	FHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera");
}

void ACCamera::BeginPlay()
{
	Super::BeginPlay();


}

