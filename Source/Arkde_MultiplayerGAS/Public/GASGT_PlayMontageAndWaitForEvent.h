// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "GameplayTags/Classes/GameplayTagContainer.h"
#include "GASGT_PlayMontageAndWaitForEvent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPlayMontageAndWaitForEventSignature, FGameplayTag, EventTag, FGameplayEventData, EventData);

class UGameplayAbility;
class UAnimMontage;

/**
 * 
 */
UCLASS()
class ARKDE_MULTIPLAYERGAS_API UGASGT_PlayMontageAndWaitForEvent : public UAbilityTask
{
	GENERATED_BODY()

public:

	UGASGT_PlayMontageAndWaitForEvent(const FObjectInitializer& ObjectInitializer);

protected:

	UPROPERTY(BlueprintAssignable)
	FPlayMontageAndWaitForEventSignature OnCompleteDelegate;

	UPROPERTY(BlueprintAssignable)
	FPlayMontageAndWaitForEventSignature OnBlendOutDelegate;

	UPROPERTY(BlueprintAssignable)
	FPlayMontageAndWaitForEventSignature OnInterrumptedDelegate;

	UPROPERTY(BlueprintAssignable)
	FPlayMontageAndWaitForEventSignature OnCancelledDelegate;

	UPROPERTY(BlueprintAssignable)
	FPlayMontageAndWaitForEventSignature OnEventReceivedDelegate;

public:

	virtual void Activate();

	virtual void ExternalCancel();

	virtual FString GetDebugString() const;

	virtual void OnDestroy(bool bInOwnerFinished);

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UGASGT_PlayMontageAndWaitForEvent* PlayMontageAndWaitForEvent(
		UGameplayAbility* OwningAbility,
		FName TaskInstanceName,
		UAnimMontage* MontageToPlay,
		FGameplayTagContainer EventTags,
		float Rate = 1.0f,
		FName StartSection = NAME_None,
		bool bStopWhenAbilityEnds = true,
		float AnimRootMotionTranslationScale = 1.0f);

private:

	UPROPERTY()
	UAnimMontage* MontageToPlay;

	UPROPERTY()
	FGameplayTagContainer EventTags;

	UPROPERTY()
	float Rate;

	UPROPERTY()
	FName StartSection;

	UPROPERTY()
	float AnimRootMotionTranslationScale;

	UPROPERTY()
	bool bStopWhenAbilityEnds;

	bool StopPlayingMontage();

	void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);
	
	void OnAbilityCancelled();

	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void OnGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload);

	FOnMontageBlendingOutStarted BlendingOutDelegate;
	FOnMontageEnded MontageEndedDelegate;
	FDelegateHandle CancelledHandle;
	FDelegateHandle EventHandle;
};
