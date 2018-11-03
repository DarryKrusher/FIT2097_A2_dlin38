// Fill out your copyright notice in the Description page of Project Settings.

#include "ReplicatedActor.h"
#include "Net/UnrealNetwork.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "FIT2097_A2.h"

AReplicatedActor::AReplicatedActor() {

	bReplicates = true;

	PrimaryActorTick.bCanEverTick = false;

	// it's moveable and physics able
	bReplicateMovement = true;

	GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
	GetStaticMeshComponent()->SetSimulatePhysics(true);
}

void AReplicatedActor::InteractWithActor()
{
}

void AReplicatedActor::use_Implementation()
{
	UE_LOG(LogClass, Log, TEXT("AInteractable::WasCollected_Implementation %s"), *GetName());
}

//void AReplicatedActor::interact() {
//
//}
