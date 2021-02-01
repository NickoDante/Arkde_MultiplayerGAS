// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Arkde_MultiplayerGAS/Arkde_MultiplayerGAS.h"
#include "GAS_GameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class ARKDE_MULTIPLAYERGAS_API UGAS_GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UGAS_GameplayAbility();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay Ability")
	EGAS_AbilityInputID AbilityInputID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay Ability")
	EGAS_AbilityInputID AbilityID;	
};
