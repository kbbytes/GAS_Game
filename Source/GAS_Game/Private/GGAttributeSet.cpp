// Vexar All Rights Reserved.


#include "GGAttributeSet.h"
#include <Net/UnrealNetwork.h>
#include "GameplayEffectExtension.h"

UGGAttributeSet::UGGAttributeSet()
	: Health(40.f), MaxHealth(60.f), Armor(50.f), MaxArmor(50.f), CritChance(0.f), CritMulti(1.5f), LuckyChance(5.f), DamageAdd(0.f), DamageMulti(1.f)
{

}

void UGGAttributeSet::ClampAttributeOnChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}

	if (Attribute == GetArmorAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxArmor());
	}
}

void UGGAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetInDamageAttribute())
	{
		float InDamageDone = GetInDamage();
		SetInDamage(0.f);
		if (InDamageDone > 0.f)
		{
			const float NewArmor = GetArmor() - InDamageDone;
			SetArmor(FMath::ClampAngle(NewArmor, 0.f, GetMaxArmor()));
			if ((GetArmor() <= 0.f) && !bOutOfArmor)
			{
				if (OnOutOfArmor.IsBound())
				{
					const FGameplayEffectContextHandle& EffectContext = Data.EffectSpec.GetEffectContext();
					AActor* Instigator = EffectContext.GetOriginalInstigator();
					AActor* Causer = EffectContext.GetEffectCauser();

					OnOutOfArmor.Broadcast(Instigator, Causer, Data.EffectSpec, Data.EvaluatedData.Magnitude);
				}
			}
			bOutOfArmor = (GetArmor() <= 0.f);

			float NewHealth = GetHealth();
			if (NewArmor < 0.0f)
			{
				NewHealth += NewArmor;
			}

			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));
			if ((GetHealth() <= 0.f) && !bOutOfHealth)
			{
				if (OnOutOfHealth.IsBound())
				{
					const FGameplayEffectContextHandle& EffectContext = Data.EffectSpec.GetEffectContext();
					AActor* Instigator = EffectContext.GetOriginalInstigator();
					AActor* Causer = EffectContext.GetEffectCauser();

					OnOutOfHealth.Broadcast(Instigator, Causer, Data.EffectSpec, Data.EvaluatedData.Magnitude);
				}
			}
		}
	}
}

void UGGAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	ClampAttributeOnChange(Attribute, NewValue);
}

void UGGAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	ClampAttributeOnChange(Attribute, NewValue);
}

void UGGAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UGGAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGGAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UGGAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGGAttributeSet, MaxArmor, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UGGAttributeSet, CritChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGGAttributeSet, CritMulti, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGGAttributeSet, LuckyChance, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UGGAttributeSet, DamageAdd, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGGAttributeSet, DamageMulti, COND_None, REPNOTIFY_Always);
}

void UGGAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGGAttributeSet, Health, OldHealth);
}

void UGGAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGGAttributeSet, MaxHealth, OldMaxHealth);
}

void UGGAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGGAttributeSet, MaxArmor, OldArmor);
}

void UGGAttributeSet::OnRep_MaxArmor(const FGameplayAttributeData& OldMaxArmor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGGAttributeSet, MaxArmor, OldMaxArmor);
}

void UGGAttributeSet::OnRep_CritChance(const FGameplayAttributeData& OldCritChance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGGAttributeSet, CritChance, OldCritChance);
}

void UGGAttributeSet::OnRep_CritMulti(const FGameplayAttributeData& OldCritMulti)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGGAttributeSet, CritMulti, OldCritMulti);
}

void UGGAttributeSet::OnRep_LuckyChance(const FGameplayAttributeData& OldLuckyChance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGGAttributeSet, LuckyChance, OldLuckyChance);
}

void UGGAttributeSet::OnRep_DamageAdd(const FGameplayAttributeData& OldDamageAdd)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGGAttributeSet, DamageAdd, OldDamageAdd);
}

void UGGAttributeSet::OnRep_DamageMulti(const FGameplayAttributeData& OldDamageMulti)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGGAttributeSet, DamageMulti, OldDamageMulti);
}
