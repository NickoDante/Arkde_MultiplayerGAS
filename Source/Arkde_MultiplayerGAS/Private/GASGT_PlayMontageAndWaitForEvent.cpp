// Fill out your copyright notice in the Description page of Project Settings.


#include "GASGT_PlayMontageAndWaitForEvent.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "AbilitySystemGlobals.h"

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

UGASGT_PlayMontageAndWaitForEvent::UGASGT_PlayMontageAndWaitForEvent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Rate = 1.0f;
	bStopWhenAbilityEnds = true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// When the task happens
void UGASGT_PlayMontageAndWaitForEvent::Activate()
{
	if (Ability == nullptr)
	{
		return;
	}

	bool bMontagePlayed = false;

	if (AbilitySystemComponent)
	{
		const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
		UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();

		if (AnimInstance != nullptr)
		{
			EventHandle = AbilitySystemComponent->AddGameplayEventTagContainerDelegate(EventTags, FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this,
				&UGASGT_PlayMontageAndWaitForEvent::OnGameplayEvent));

			float CurrentMontage = AbilitySystemComponent->PlayMontage(Ability, Ability->GetCurrentActivationInfo(), MontageToPlay, Rate, StartSection);

			if (CurrentMontage > 0.0f)
			{
				if (ShouldBroadcastAbilityTaskDelegates() == false)
				{
					return;
				}

				CancelledHandle = Ability->OnGameplayAbilityCancelled.AddUObject(this, &UGASGT_PlayMontageAndWaitForEvent::OnAbilityCancelled);

				BlendingOutDelegate.BindUObject(this, &UGASGT_PlayMontageAndWaitForEvent::OnMontageBlendingOut);
				AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, MontageToPlay);

				MontageEndedDelegate.BindUObject(this, &UGASGT_PlayMontageAndWaitForEvent::OnMontageEnded);
				AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, MontageToPlay);

				ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
				if (IsValid(Character) && (Character->GetLocalRole() == ROLE_Authority || 
					(Character->GetLocalRole() == ROLE_AutonomousProxy && Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted)))
				{
					Character->SetAnimRootMotionTranslationScale(AnimRootMotionTranslationScale);
				}

				bMontagePlayed = true;
			}
		}
	}

	if (!bMontagePlayed)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnCancelledDelegate.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void UGASGT_PlayMontageAndWaitForEvent::ExternalCancel()
{
	check(AbilitySystemComponent);

	OnAbilityCancelled();

	Super::ExternalCancel();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

FString UGASGT_PlayMontageAndWaitForEvent::GetDebugString() const
{
	UAnimMontage* PlayingMontage = nullptr;
	if (Ability)
	{
		const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
		UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();

		if (AnimInstance != nullptr)
		{
			PlayingMontage = AnimInstance->Montage_IsActive(MontageToPlay) ? MontageToPlay : AnimInstance->GetCurrentActiveMontage();
		}
	}

	return FString::Printf(TEXT("PlayMontageAndWaitForEvent. MontageToPlay: %s (CurrentPlaying): %s"), *GetNameSafe(MontageToPlay), *GetNameSafe(PlayingMontage));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void UGASGT_PlayMontageAndWaitForEvent::OnDestroy(bool bInOwnerFinished)
{
	if (Ability)
	{
		Ability->OnGameplayAbilityCancelled.Remove(CancelledHandle);
		if (bInOwnerFinished && bStopWhenAbilityEnds)
		{
			StopPlayingMontage();
		}
	}

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->RemoveGameplayEventTagContainerDelegate(EventTags, EventHandle);
	}

	Super::OnDestroy(bInOwnerFinished);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

UGASGT_PlayMontageAndWaitForEvent* UGASGT_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(UGameplayAbility* OwningAbility, FName TaskInstanceName, UAnimMontage* MontageToPlay, FGameplayTagContainer EventTags, float Rate /*= 1.0f*/, FName StartSection /*= NAME_None*/, bool bStopWhenAbilityEnds /*= true*/, float AnimRootMotionTranslationScale /*= 1.0f*/)
{
	UAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Rate(Rate);

	UGASGT_PlayMontageAndWaitForEvent* MyObj = NewAbilityTask<UGASGT_PlayMontageAndWaitForEvent>(OwningAbility, TaskInstanceName);
	MyObj->MontageToPlay = MontageToPlay;
	MyObj->EventTags = EventTags;
	MyObj->Rate = Rate;
	MyObj->StartSection = StartSection;
	MyObj->AnimRootMotionTranslationScale = AnimRootMotionTranslationScale;
	MyObj->bStopWhenAbilityEnds = bStopWhenAbilityEnds;

	return MyObj;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

bool UGASGT_PlayMontageAndWaitForEvent::StopPlayingMontage()
{
	const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
	if (!ActorInfo)
	{
		return false;
	}

	UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
	if (AnimInstance == nullptr)
	{
		return false;
	}

	if (AbilitySystemComponent && Ability)
	{
		if (AbilitySystemComponent->GetAnimatingAbility() == Ability && AbilitySystemComponent->GetCurrentMontage() == MontageToPlay)
		{
			FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage(MontageToPlay);
			if (MontageInstance)
			{
				MontageInstance->OnMontageBlendingOutStarted.Unbind();
				MontageInstance->OnMontageEnded.Unbind();
			}

			AbilitySystemComponent->CurrentMontageStop();
			return true;
		}
	}

	return false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void UGASGT_PlayMontageAndWaitForEvent::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (Ability && Ability->GetCurrentMontage() == MontageToPlay)
	{
		if (Montage == MontageToPlay)
		{
			AbilitySystemComponent->ClearAnimatingAbility(Ability);
			ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
			if (IsValid(Character) && (Character->GetLocalRole() == ROLE_Authority ||
				(Character->GetLocalRole() == ROLE_AutonomousProxy && Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted)))
			{
				Character->SetAnimRootMotionTranslationScale(1.0f);
			}
		}
	}

	if (bInterrupted)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnInterrumptedDelegate.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}
	else
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnBlendOutDelegate.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void UGASGT_PlayMontageAndWaitForEvent::OnAbilityCancelled()
{
	if (StopPlayingMontage())
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnCancelledDelegate.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void UGASGT_PlayMontageAndWaitForEvent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnCompleteDelegate.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}

	EndTask();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void UGASGT_PlayMontageAndWaitForEvent::OnGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		FGameplayEventData tempData = *Payload;
		tempData.EventTag = EventTag;

		OnEventReceivedDelegate.Broadcast(EventTag, tempData);
	}
}