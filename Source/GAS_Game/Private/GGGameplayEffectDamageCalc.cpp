// Vexar All Rights Reserved.


#include "GGGameplayEffectDamageCalc.h"
#include "GGAttributeSet.h"

struct FDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(InDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritMulti);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LuckyChance);

	FDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGGAttributeSet, InDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGGAttributeSet, CritChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGGAttributeSet, CritMulti, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGGAttributeSet, LuckyChance, Source, false);
	}
};

static const FDamageStatics DamageStatics()
{
	static FDamageStatics DmgStatics;
	return DmgStatics;
}

UGGGameplayEffectDamageCalc::UGGGameplayEffectDamageCalc()
{
	RelevantAttributesToCapture.Add(DamageStatics().InDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().CritChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CritMultiDef);
	RelevantAttributesToCapture.Add(DamageStatics().LuckyChanceDef);
}

void UGGGameplayEffectDamageCalc::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	AActor* TargetActor = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// Retrieve the InDamage value passed to Assign Tag Set By Caller Magnitude (On output of Make Outgoing Spec)
	float InDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().InDamageDef, EvaluationParameters, InDamage);

	FGameplayEffectSpec* MutableSpec = ExecutionParams.GetOwningSpecForPreExecuteMod();
	float CritChance = 0.f;
	float CritMulti = 0.f;
	float LuckyChance = 0.f;
	
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CritChanceDef, EvaluationParameters, CritChance);
	const FHitResult* Hit = Spec.GetContext().GetHitResult();
	if (Hit && Hit->BoneName == "head")
	{
		CritChance += 100.f;
	}

	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CritMultiDef, EvaluationParameters, CritMulti);
	if (CritMulti > 1.f)
	{
		bool IsCrit = FMath::RandRange(0.01f, 100.f) <= CritChance;
		if (IsCrit)
		{
			InDamage *= CritMulti;
		}
	}

	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().LuckyChanceDef, EvaluationParameters, LuckyChance);
	if (LuckyChance > 0.f)
	{
		bool IsLucky = false;
		float LuckyMulti = 1.f;
		while (FMath::RandRange(0.01f, 100.f) <= LuckyChance)
		{
			LuckyChance -= 100.f;
			LuckyMulti += 1.f;
			IsLucky = true;
		}

		if (IsLucky)
		{
			InDamage *= LuckyMulti;
		}
	}

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().InDamageProperty, EGameplayModOp::Additive, InDamage));

	UE_LOG(LogTemp, Log, TEXT("Damage - %f"), InDamage);
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Damage - %f"), InDamage));

}
