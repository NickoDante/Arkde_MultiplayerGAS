// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS_AttributeSet.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

UGAS_AttributeSet::UGAS_AttributeSet()
{
	MaxHealth = 100.0f;
	Health = MaxHealth;
	HealthRegen = 0.5f;

	MaxMana = 100.0f;
	Mana = MaxMana;
	ManaRegen = 2.0f;

	MaxStamina = 100.0f;
	Stamina = MaxStamina;
	StaminaRegen = 1.0f;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void UGAS_AttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{
		AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
	}
	else if (Attribute == GetMaxManaAttribute())
	{
		AdjustAttributeForMaxChange(Mana, MaxMana, NewValue, GetManaAttribute());
	}
	else if (Attribute == GetMaxStaminaAttribute())
	{
		AdjustAttributeForMaxChange(Stamina, MaxStamina, NewValue, GetStaminaAttribute());
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void UGAS_AttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data)
{
	if (Data.EvaluatedData.Attribute.GetUProperty() == 
		FindFieldChecked<FProperty>(UGAS_AttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UGAS_AttributeSet, Health)))
	{
		Health.SetCurrentValue(FMath::Clamp(Health.GetCurrentValue(), 0.f, MaxHealth.GetCurrentValue()));
		Health.SetBaseValue(FMath::Clamp(Health.GetBaseValue(), 0.f, MaxHealth.GetBaseValue()));
		UE_LOG(LogTemp, Warning, TEXT("Health Changed : %f"), Health.GetCurrentValue());
	}
	else if (Data.EvaluatedData.Attribute.GetUProperty() ==
		FindFieldChecked<FProperty>(UGAS_AttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UGAS_AttributeSet, Mana)))
	{
		Mana.SetCurrentValue(FMath::Clamp(Mana.GetCurrentValue(), 0.f, MaxMana.GetCurrentValue()));
		Mana.SetBaseValue(FMath::Clamp(Mana.GetBaseValue(), 0.f, MaxMana.GetBaseValue()));
		UE_LOG(LogTemp, Warning, TEXT("Mana Changed : %f"), Mana.GetCurrentValue());
	}
	else if (Data.EvaluatedData.Attribute.GetUProperty() ==
		FindFieldChecked<FProperty>(UGAS_AttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UGAS_AttributeSet, Stamina)))
	{
		Stamina.SetCurrentValue(FMath::Clamp(Stamina.GetCurrentValue(), 0.f, MaxStamina.GetCurrentValue()));
		Stamina.SetBaseValue(FMath::Clamp(Stamina.GetBaseValue(), 0.f, MaxStamina.GetBaseValue()));
		UE_LOG(LogTemp, Warning, TEXT("Stamina Changed : %f"), Stamina.GetCurrentValue());
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void UGAS_AttributeSet::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{
	UAbilitySystemComponent* AbilityComponent = GetOwningAbilitySystemComponent(); // Obtain the Ability System Component, no matter the actor.

	if (!IsValid(AbilityComponent))
	{
		return;
	}

	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();

	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue)) // We use it to avoid near zero values
	{
		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		const float NewDelta = CurrentMaxValue > 0.f ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue : NewMaxValue;
		AbilityComponent->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void UGAS_AttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_AttributeSet, Health, OldHealth);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void UGAS_AttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_AttributeSet, MaxHealth, OldMaxHealth);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void UGAS_AttributeSet::OnRep_HealthRegen(const FGameplayAttributeData& OldHealthRegen)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_AttributeSet, HealthRegen, OldHealthRegen);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void UGAS_AttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_AttributeSet, Mana, OldMana);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void UGAS_AttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_AttributeSet, MaxMana, OldMaxMana);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void UGAS_AttributeSet::OnRep_ManaRegen(const FGameplayAttributeData& OldManaRegen)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_AttributeSet, ManaRegen, OldManaRegen);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void UGAS_AttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_AttributeSet, Stamina, OldStamina);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void UGAS_AttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_AttributeSet, MaxStamina, OldMaxStamina);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void UGAS_AttributeSet::OnRep_StaminaRegen(const FGameplayAttributeData& OldStaminaRegen)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_AttributeSet, StaminaRegen, OldStaminaRegen);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void UGAS_AttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UGAS_AttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAS_AttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAS_AttributeSet, HealthRegen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAS_AttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAS_AttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAS_AttributeSet, ManaRegen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAS_AttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAS_AttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAS_AttributeSet, StaminaRegen, COND_None, REPNOTIFY_Always);
}
