// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/PS_InteractableProp.h"
#include "PS_Prop_Base.generated.h"

// Contains all character's animations needed to interact with the props
USTRUCT(BlueprintType) 
struct FInteractionAnimationsStorage
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    class UAnimationAsset* StartingInteractionAnimation;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    class UAnimationAsset* InteractionAnimation;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    class UAnimationAsset* EndingInteractionAnimation;
};

UCLASS()
class PROJECTSATURN_API APS_Prop_Base : public AActor, public IPS_InteractableProp
{
	GENERATED_BODY()
	
public:	
	APS_Prop_Base();
    
    UFUNCTION()
    virtual void Interact(ACharacter* Character) override;
    UFUNCTION()
    virtual void ShowTooltip(const bool Value) override;
    
protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    class UStaticMeshComponent* StaticMesh;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    class UTextRenderComponent* TextTooltip;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    FInteractionAnimationsStorage AnimationStorage;
    
	virtual void BeginPlay() override;


};
