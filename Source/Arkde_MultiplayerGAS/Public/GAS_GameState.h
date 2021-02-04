// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GAS_GameState.generated.h"

/**
 * 
 */
UCLASS()
class ARKDE_MULTIPLAYERGAS_API AGAS_GameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:

	void CheckWinCondition();
};
