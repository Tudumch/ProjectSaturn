#pragma once

#include "PS_Structs.generated.h"

USTRUCT(BlueprintType)
struct FMeleeAttackParams
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Height = 50.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Distance = 60.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TraceRadius = 20.f;
};