// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS_AttributeSet.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"

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
	}
	else if (Data.EvaluatedData.Attribute.GetUProperty() ==
		FindFieldChecked<FProperty>(UGAS_AttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UGAS_AttributeSet, Mana)))
	{
		Mana.SetCurrentValue(FMath::Clamp(Mana.GetCurrentValue(), 0.f, MaxMana.GetCurrentValue()));
		Mana.SetBaseValue(FMath::Clamp(Mana.GetBaseValue(), 0.f, MaxMana.GetBaseValue()));
	}
	else if (Data.EvaluatedData.Attribute.GetUProperty() ==
		FindFieldChecked<FProperty>(UGAS_AttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UGAS_AttributeSet, Stamina)))
	{
		Stamina.SetCurrentValue(FMath::Clamp(Stamina.GetCurrentValue(), 0.f, MaxStamina.GetCurrentValue()));
		Stamina.SetBaseValue(FMath::Clamp(Stamina.GetBaseValue(), 0.f, MaxStamina.GetBaseValue()));
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
