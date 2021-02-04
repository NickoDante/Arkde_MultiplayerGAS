// Fill out your copyright notice in the Description page of Project Settings.


#include "GASGA_Firebolt.h"
#include "Arkde_MultiplayerGAS/Arkde_MultiplayerGASCharacter.h"
#include "Arkde_MultiplayerGAS/Public/GASGT_PlayMontageAndWaitForEvent.h"
#include "Arkde_MultiplayerGAS/Public/GAS_Projectile.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void UGASGA_Firebolt::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	UGASGT_PlayMontageAndWaitForEvent* MontageTask = UGASGT_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(
		this,
		NAME_None,
		CastMontage,
		FGameplayTagContainer(),
		1.0f,
		NAME_None,
		false,
		1.0f
	);

	MontageTask->OnBlendOutDelegate.AddDynamic(this, &UGASGA_Firebolt::OnMontageCompleted);
	MontageTask->OnCompleteDelegate.AddDynamic(this, &UGASGA_Firebolt::OnMontageCompleted);
	MontageTask->OnInterrumptedDelegate.AddDynamic(this, &UGASGA_Firebolt::OnMontageCancelled);
	MontageTask->OnCancelledDelegate.AddDynamic(this, &UGASGA_Firebolt::OnMontageCancelled);
	MontageTask->OnEventReceivedDelegate.AddDynamic(this, &UGASGA_Firebolt::EventReceived);

	MontageTask->ReadyForActivation();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void UGASGA_Firebolt::OnMontageCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void UGASGA_Firebolt::OnMontageCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void UGASGA_Firebolt::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (GetOwningActorFromActorInfo()->GetLocalRole() == ROLE_Authority && EventTag == ProjectileSpawnTag)
	{
		AArkde_MultiplayerGASCharacter* Character = Cast<AArkde_MultiplayerGASCharacter>(GetAvatarActorFromActorInfo());
		if (!IsValid(Character))
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		}

		FVector StartLocation = Character->GetMesh()->GetSocketLocation(AbilitySocketName);
		FVector EndLocation = Character->GetCameraBoom()->GetComponentLocation() + (Character->GetFollowCamera()->GetForwardVector() * ProjectileRange);
		FRotator ProjectileRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, EndLocation);

		FTransform SpawnTransform = Character->GetMesh()->GetSocketTransform(AbilitySocketName);
		SpawnTransform.SetRotation(ProjectileRotation.Quaternion());

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AGAS_Projectile* Firebolt = GetWorld()->SpawnActorDeferred<AGAS_Projectile>(ProjectileClass, SpawnTransform, GetOwningActorFromActorInfo(), Character, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		if (IsValid(Firebolt))
		{
			Firebolt->Multicast_IgnoreActor(Character);
			Firebolt->Range = ProjectileRange;
			Firebolt->FinishSpawning(SpawnTransform);
		}
	}
}
