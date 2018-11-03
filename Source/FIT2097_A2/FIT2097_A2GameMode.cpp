// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "FIT2097_A2GameMode.h"
#include "FIT2097_A2Character.h"
#include "FIT2097_A2HUD.h"
#include "UObject/ConstructorHelpers.h"

AFIT2097_A2GameMode::AFIT2097_A2GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	DefaultPawnClass = PlayerPawnBPClass.Class;


	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// use our custom HUD class
	//HUDClass = FIT2097_A2HUD::StaticClass();

	static ConstructorHelpers::FClassFinder<AHUD> PlayerHUDClass(TEXT("/Game/ThirdPersonCPP/Blueprints/MyFIT2097_A2HUD"));
	if (PlayerHUDClass.Class != NULL)
	{
		HUDClass = PlayerHUDClass.Class;
	}

};
