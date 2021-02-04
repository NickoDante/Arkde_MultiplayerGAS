// Copyright Epic Games, Inc. All Rights Reserved.

#include "Arkde_MultiplayerGASGameMode.h"
#include "Arkde_MultiplayerGASCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Arkde_MultiplayerGAS/Public/GAS_PlayerController.h"
#include "Arkde_MultiplayerGAS/Public/GAS_GameState.h"

AArkde_MultiplayerGASGameMode::AArkde_MultiplayerGASGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void AArkde_MultiplayerGASGameMode::PlayerKilled(AController* VictimController)
{
	AGAS_PlayerController* VictimPlayerController = Cast<AGAS_PlayerController>(VictimController);
	if (IsValid(VictimPlayerController))
	{
		VictimPlayerController->GameConclussion(false);
	}

	AGAS_GameState* WorldGameState = GetWorld()->GetGameState<AGAS_GameState>();
}
