// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS_PlayerState.h"
#include "AbilitySystemComponent.h"
#include "GAS_AttributeSet.h"
#include "Arkde_MultiplayerGAS/Arkde_MultiplayerGASCharacter.h"

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

AGAS_PlayerState::AGAS_PlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);

	AttributeSet = CreateDefaultSubobject<UGAS_AttributeSet>(TEXT("AttributeSet"));

	NetUpdateFrequency = 100.0f;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

UAbilitySystemComponent* AGAS_PlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

UGAS_AttributeSet* AGAS_PlayerState::GetAttributeSet() const
{
	return AttributeSet;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

bool AGAS_PlayerState::IsAlive() const
{
	return GetHealth() > 0.0f;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

float AGAS_PlayerState::GetHealth() const
{
	return GetAttributeSet()->GetHealth();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

float AGAS_PlayerState::GetMaxHealth() const
{
	return GetAttributeSet()->GetMaxHealth();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

float AGAS_PlayerState::GetHealthRegen() const
{
	return GetAttributeSet()->GetHealthRegen();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void AGAS_PlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(AbilitySystemComponent))
	{
		// Callback Attribute Change / Delegate for Attributes
		HealthChangeDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAttributeSet()->GetHealthAttribute()).AddUObject(this, &AGAS_PlayerState::OnHealthChanged);
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void AGAS_PlayerState::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	if (!IsAlive() && IsValid(AbilitySystemComponent))
	{
		AArkde_MultiplayerGASCharacter* CharacterReference = Cast<AArkde_MultiplayerGASCharacter>(GetPawn());
		if (IsValid(CharacterReference))
		{
			CharacterReference->Die();
		}
	}
}