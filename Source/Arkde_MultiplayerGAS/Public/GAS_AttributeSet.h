// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GAS_AttributeSet.generated.h"

// With this macro, every Property created, will have the getter, the setter and the init automatically.
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class ARKDE_MULTIPLAYERGAS_API UGAS_AttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	UGAS_AttributeSet();

public:

	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UGAS_AttributeSet, Health); // Use the definition above to create getter setter and init.

	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UGAS_AttributeSet, MaxHealth);
	
	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_HealthRegen)
	FGameplayAttributeData HealthRegen;
	ATTRIBUTE_ACCESSORS(UGAS_AttributeSet, HealthRegen);

	UPROPERTY(BlueprintReadOnly, Category = "Mana", ReplicatedUsing = OnRep_Mana)
		FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UGAS_AttributeSet, Mana); // Use the definition above to create getter setter and init.

	UPROPERTY(BlueprintReadOnly, Category = "Mana", ReplicatedUsing = OnRep_MaxMana)
		FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UGAS_AttributeSet, MaxMana);

	UPROPERTY(BlueprintReadOnly, Category = "Mana", ReplicatedUsing = OnRep_ManaRegen)
		FGameplayAttributeData ManaRegen;
	ATTRIBUTE_ACCESSORS(UGAS_AttributeSet, ManaRegen);

	UPROPERTY(BlueprintReadOnly, Category = "Stamina", ReplicatedUsing = OnRep_Stamina)
		FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UGAS_AttributeSet, Stamina); // Use the definition above to create getter setter and init.

	UPROPERTY(BlueprintReadOnly, Category = "Stamina", ReplicatedUsing = OnRep_MaxStamina)
		FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UGAS_AttributeSet, MaxStamina);

	UPROPERTY(BlueprintReadOnly, Category = "Stamina", ReplicatedUsing = OnRep_StaminaRegen)
		FGameplayAttributeData StaminaRegen;
	ATTRIBUTE_ACCESSORS(UGAS_AttributeSet, StaminaRegen);

public: 

	// To check Roof Floor of values.
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	// The best way to modify attributes during runtime.
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override;

	// Function to modify attributes
	void AdjustAttributeForMaxChange(
		FGameplayAttributeData& AffectedAttribute,
		const FGameplayAttributeData& MaxAttribute,
		float NewMaxValue,
		const FGameplayAttribute& AffectedAttributeProperty
		);

protected:

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);
	
	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);
	
	UFUNCTION()
	virtual void OnRep_HealthRegen(const FGameplayAttributeData& OldHealthRegen);
	
	UFUNCTION()
	virtual void OnRep_Mana(const FGameplayAttributeData& OldMana);
	
	UFUNCTION()
	virtual void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana);
	
	UFUNCTION()
	virtual void OnRep_ManaRegen(const FGameplayAttributeData& OldManaRegen);
	
	UFUNCTION()
	virtual void OnRep_Stamina(const FGameplayAttributeData& OldStamina);
	
	UFUNCTION()
	virtual void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina);
	
	UFUNCTION()
	virtual void OnRep_StaminaRegen(const FGameplayAttributeData& OldStaminaRegen);

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty> & OutLifetimeProps) const override;
};
