// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS_PlayerController.h"

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void AGAS_PlayerController::GameConclussion(bool bWasSuccesful)
{
	BP_GameConclussion(bWasSuccesful);

	SetViewTargetWithBlend(nullptr, 0.5f, EViewTargetBlendFunction::VTBlend_Cubic);
}
