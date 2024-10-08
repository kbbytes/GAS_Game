// Vexar All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GGAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_MULTICAST_DELEGATE_FourParams(FGGAttributeEvent, AActor* /*EffectInstigator*/, AActor* /*EffectCauser*/, const FGameplayEffectSpec& /*EffectSpec*/, float /*EffectMagnitude*/);

/**
 * 
 */
UCLASS()
class GAS_GAME_API UGGAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	UGGAttributeSet();

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Attributes", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UGGAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Attributes", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UGGAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category = "Attributes", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UGGAttributeSet, Armor);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxArmor, Category = "Attributes", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxArmor;
	ATTRIBUTE_ACCESSORS(UGGAttributeSet, MaxArmor);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData InDamage;
	ATTRIBUTE_ACCESSORS(UGGAttributeSet, InDamage);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CritChance, Category = "Attributes", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData CritChance;
	ATTRIBUTE_ACCESSORS(UGGAttributeSet, CritChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CritMulti, Category = "Attributes", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData CritMulti;
	ATTRIBUTE_ACCESSORS(UGGAttributeSet, CritMulti);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_LuckyChance, Category = "Attributes", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData LuckyChance;
	ATTRIBUTE_ACCESSORS(UGGAttributeSet, LuckyChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_DamageAdd, Category = "Attributes", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData DamageAdd;
	ATTRIBUTE_ACCESSORS(UGGAttributeSet, DamageAdd);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_DamageMulti, Category = "Attributes", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData DamageMulti;
	ATTRIBUTE_ACCESSORS(UGGAttributeSet, DamageMulti);

	mutable FGGAttributeEvent OnOutOfHealth;
	mutable FGGAttributeEvent OnOutOfArmor;

protected:

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor);

	UFUNCTION()
	void OnRep_MaxArmor(const FGameplayAttributeData& OldMaxArmor);

	UFUNCTION()
	void OnRep_CritChance(const FGameplayAttributeData& OldCritChance);

	UFUNCTION()
	void OnRep_CritMulti(const FGameplayAttributeData& OldCritMulti);

	UFUNCTION()
	void OnRep_LuckyChance(const FGameplayAttributeData& OldLuckyChance);

	UFUNCTION()
	void OnRep_DamageAdd(const FGameplayAttributeData& OldDamageAdd);

	UFUNCTION()
	void OnRep_DamageMulti(const FGameplayAttributeData& OldDamageMulti);

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void ClampAttributeOnChange(const FGameplayAttribute& Attribute, float& NewValue) const;

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	bool bOutOfHealth;
	bool bOutOfArmor;
};
