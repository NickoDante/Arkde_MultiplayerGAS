// Fill out your copyright notice in the Description page of Project Settings.


#include "GASGT_PlayMontageAndWaitForEvent.h"

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

UGASGT_PlayMontageAndWaitForEvent::UGASGT_PlayMontageAndWaitForEvent(const FObjectInitializer& ObjectInitializer)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void UGASGT_PlayMontageAndWaitForEvent::Activate()
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void UGASGT_PlayMontageAndWaitForEvent::ExternalCancel()
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

FString UGASGT_PlayMontageAndWaitForEvent::GetDebugString() const
{
	return FString();
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