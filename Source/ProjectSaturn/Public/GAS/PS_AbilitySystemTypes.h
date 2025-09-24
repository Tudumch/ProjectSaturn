// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum class EPSAbilityInputID: uint8
{
    None UMETA(DisplayName = "None"),
    PrimaryAction UMETA(DsplayName = "Primary Action"),
    SecondaryAction UMETA(DsplayName = "Secondary Action")
};

