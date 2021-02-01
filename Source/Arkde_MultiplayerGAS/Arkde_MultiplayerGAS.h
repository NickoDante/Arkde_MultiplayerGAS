// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EGAS_AbilityInputID : uint8
{
	// 0 None
	InputID_None		UMETA(DisplayName = "None"),
	// 1 Confirm
	InputID_Confirm		UMETA(DisplayName = "Confirm"),
	// 2 Cancel
	InputID_Cancel		UMETA(DisplayName = "Cancel"),
	// 3 Ability1
	InputID_Ability1	UMETA(DisplayName = "Ability 1"),
	// 4 Ability2
	InputID_Ability2	UMETA(DisplayName = "Ability 2"),
	// 5 Sprint
	InputID_Sprint		UMETA(DisplayName = "Sprint"),
	// 6 Jump
	InputID_Jump		UMETA(DisplayName = "Jump")
};