// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS_GameState.h"
#include "GAS_PlayerController.h"
#include "GAS_PlayerState.h"
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void AGAS_GameState::CheckWinCondition()
{
	int AlivePlayersCounter = 0;
	AGAS_PlayerController* WinningPlayerController = nullptr;

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		AGAS_PlayerController* PlayerController = Cast<AGAS_PlayerController>(It->Get());
		if (IsValid(PlayerController))
		{
			AGAS_PlayerState* PlayerState = PlayerController->GetPlayerState<AGAS_PlayerState>();
			if (IsValid(PlayerState))
			{
				if (PlayerState->IsAlive())
				{
					AlivePlayersCounter++;

					if (AlivePlayersCounter > 1)
					{
						return;
					}
					else
					{
						WinningPlayerController = PlayerController;
					}
				}
			}
		}
	}

	WinningPlayerController->GameConclussion(true);
}
