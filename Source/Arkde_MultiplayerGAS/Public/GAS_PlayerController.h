// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GAS_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ARKDE_MULTIPLAYERGAS_API AGAS_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	void GameConclussion(bool bWasSuccesful);

protected:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_GameConclussion(bool bWasSuccesful);

};
