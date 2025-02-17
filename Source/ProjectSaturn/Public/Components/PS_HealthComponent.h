// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PS_HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTSATURN_API UPS_HealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPS_HealthComponent();

    UFUNCTION(BlueprintPure)
    float GetCurrentHealth() const { return CurrentHealth; };
    UFUNCTION(BlueprintCallable)
    void SetCurrentHealth(const float Value) { CurrentHealth = Value; };
    UFUNCTION(BlueprintCallable)
    void AddHealth(const float Value);
    
protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
    float MaxHealth = 100;
    UPROPERTY(BlueprintReadOnly, Category="Health")
    float CurrentHealth = MaxHealth;
	virtual void BeginPlay() override;

};
