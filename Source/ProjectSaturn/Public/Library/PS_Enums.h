#pragma once

UENUM(BlueprintType)
enum class EWeaponType: uint8
{
    Unarmed = 0,
    BareHands,
    RangeWeapon_OneHanded,
    RangeWeapon_TwoHanded,
    MeleeWeapon_OneHanded
};