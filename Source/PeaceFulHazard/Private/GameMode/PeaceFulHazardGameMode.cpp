// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameMode/PeaceFulHazardGameMode.h"
#include "Character/PeaceFulHazardCharacter.h"
#include "UObject/ConstructorHelpers.h"

APeaceFulHazardGameMode::APeaceFulHazardGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
