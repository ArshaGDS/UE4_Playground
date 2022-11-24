// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Character.h"
#include "Ch_CharacterBase.generated.h"

UCLASS()
class PLAYGROUND_API ACh_CharacterBase : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACh_CharacterBase();
	
	virtual FGenericTeamId GetGenericTeamId() const override;

protected:
	
	int TeamId{};
};
