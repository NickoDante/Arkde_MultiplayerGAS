// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "GAS_PlayerState.generated.h"

class UAbilitySystemComponent;
class UGAS_AttributeSet;
class UGAS_GameplayAbility;

/**
 * 
 */
UCLASS()
class ARKDE_MULTIPLAYERGAS_API AGAS_PlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	AGAS_PlayerState();

	/* --- Gameplay Ability System Start --- */

protected:

	UPROPERTY()
	UAbilitySystemComponent* AbilitySystemComponent;

protected:

	UPROPERTY()
	UGAS_AttributeSet* AttributeSet;

public:

	UAbilitySystemComponent* GetAbilitySystemComponent() const;

	UGAS_AttributeSet* GetAttributeSet() const;

	/* --- Gameplay Ability System End --- */

	bool IsAlive() const;

	float GetHealth() const;
	float GetMaxHealth() const;
	float GetHealthRegen() const;

	virtual void BeginPlay() override;

	FDelegateHandle HealthChangeDelegateHandle;
	
	virtual void OnHealthChanged(const FOnAttributeChangeData& Data);

	/* --- Win Condition Start --- */

	int KillCount;

	void ScoreKill();

	/* --- Win Condition End --- */

};
