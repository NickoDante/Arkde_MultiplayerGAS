// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS_GameplayAbility.h"
#include "GASGA_Firebolt.generated.h"

class UAnimMontage;
class AGAS_Projectile;

/**
 * 
 */
UCLASS()
class ARKDE_MULTIPLAYERGAS_API UGASGA_Firebolt : public UGAS_GameplayAbility
{
	GENERATED_BODY()
	
public: 

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* CastMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AGAS_Projectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag ProjectileSpawnTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ProjectileSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ProjectileRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName AbilitySocketName;

public:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:

	UFUNCTION()
	void OnMontageCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void OnMontageCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);
};
