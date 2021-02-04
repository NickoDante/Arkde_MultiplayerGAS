// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Arkde_MultiplayerGASGameMode.generated.h"

UCLASS(minimalapi)
class AArkde_MultiplayerGASGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AArkde_MultiplayerGASGameMode();

	void PlayerKilled(AController* VictimController);
};



