// Copyright Epic Games, Inc. All Rights Reserved.

#include "Arkde_MultiplayerGASGameMode.h"
#include "Arkde_MultiplayerGASCharacter.h"
#include "UObject/ConstructorHelpers.h"

AArkde_MultiplayerGASGameMode::AArkde_MultiplayerGASGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
