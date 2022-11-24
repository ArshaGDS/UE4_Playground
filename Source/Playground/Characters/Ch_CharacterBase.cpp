// Fill out your copyright notice in the Description page of Project Settings.


#include "Ch_CharacterBase.h"

// Sets default values
ACh_CharacterBase::ACh_CharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

FGenericTeamId ACh_CharacterBase::GetGenericTeamId() const
{
	const FGenericTeamId GenericTeamId(TeamId);
	return GenericTeamId;
}