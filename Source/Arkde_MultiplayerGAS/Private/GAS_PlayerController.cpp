// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS_PlayerController.h"
#include "Kismet/GameplayStatics.h"

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void AGAS_PlayerController::GameConclussion(bool bWasSuccesful)
{
	BP_GameConclussion(bWasSuccesful);

	TArray<AActor*> ViewpointActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), SpectatingViewpointClass, ViewpointActors);

	SetViewTargetWithBlend(ViewpointActors[0], 0.5f, EViewTargetBlendFunction::VTBlend_Cubic);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void AGAS_PlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	SetViewTarget(aPawn);
}
