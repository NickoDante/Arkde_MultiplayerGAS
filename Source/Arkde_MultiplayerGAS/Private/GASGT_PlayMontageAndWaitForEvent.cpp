// Fill out your copyright notice in the Description page of Project Settings.


#include "GASGT_PlayMontageAndWaitForEvent.h"
#include "GameFramework/Character.h"
#include "GameplayAbilities/Public/AbilitySystemComponent.h"

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

}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

UGASGT_PlayMontageAndWaitForEvent* UGASGT_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(UGameplayAbility* OwningAbility, FName TaskInstanceName, UAnimMontage* MontageToPlay, FGameplayTagContainer EventTags, float Rate /*= 1.0f*/, FName StartSection /*= NAME_None*/, bool bStopWhenAbilityEnds /*= true*/, float AnimRootMotionTranslationScale /*= 1.0f*/)
{
	return nullptr;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

bool UGASGT_PlayMontageAndWaitForEvent::StopPlayingMontage()
{
	return true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void UGASGT_PlayMontageAndWaitForEvent::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void UGASGT_PlayMontageAndWaitForEvent::OnAbilityCancelled()
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void UGASGT_PlayMontageAndWaitForEvent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void UGASGT_PlayMontageAndWaitForEvent::OnGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload)
{

}