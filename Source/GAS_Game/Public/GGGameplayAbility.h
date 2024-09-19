// Vexar All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "../Gas_Game.h"
#include "GGGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class GAS_GAME_API UGGGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EAbilityInputID AbilityInputID { EAbilityInputID::None };
	
};
