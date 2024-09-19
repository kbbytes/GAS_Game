// Vexar All Rights Reserved.


#include "GGDestructible.h"
#include "AbilitySystemComponent.h"
#include "GGAttributeSet.h"

// Sets default values
AGGDestructible::AGGDestructible()
{
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetSimulatePhysics(true);
	StaticMeshComp->SetCollisionObjectType(ECC_PhysicsBody);
	RootComponent = StaticMeshComp;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UGGAttributeSet>(TEXT("AttributeSet"));
}

// Called when the game starts or when spawned
void AGGDestructible::BeginPlay()
{
	Super::BeginPlay();

	if (!AbilitySystemComponent)
		return;

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &AGGDestructible::OnHealthAttributeChanged);
	
}

UAbilitySystemComponent* AGGDestructible::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AGGDestructible::OnHealthAttributeChanged(const FOnAttributeChangeData& Data)
{
	OnHealthChanged(Data.OldValue, Data.NewValue);
}
