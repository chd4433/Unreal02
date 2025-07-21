
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Weapons/IDamagable.h"
#include "CEnemy.generated.h"

UCLASS()
class U2505_98_API ACEnemy 
	: public ACharacter
	, public IIDamagable
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Color")
	FColor OriginColor = FColor::Black;

private:
	UPROPERTY(EditAnywhere, Category = "Health")
	float MaxHealth = 100;

private:
	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* Hand_Mesh;

	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* Armour_Mesh;

	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* Boots_Mesh;

	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* Hat_Mesh;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	class UAnimMontage* DeadMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	float DeadMontage_PlayRate = 1.0f;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<class UCUserWidget_Enemy> UI_EnemyClass;

private:
	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* Widget;


public:
	ACEnemy();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	void OnConstruction(const FTransform& Transform) override;

private:
	void ChangeColor(const FLinearColor& InColor);
	void SetMaterialChange(USkeletalMeshComponent* meshComp);

public:
	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void Damaged(struct FDamagedDataEvent* InEvent, class ACharacter* InAttacker);

public:
	virtual void End_Damaged();

protected:
	virtual void Dead();

public:
	void End_Dead();

private:
	TArray<class UMaterialInstanceDynamic*> Materials;

private:
	float Health;

private:
	class UCUserWidget_Enemy* UI_Enemy;
	//class AController* ac;
};
