// Fill out your copyright notice in the Description page of Project Settings.

#include "RemoteButton.h"
#include "FIT2097_A2.h"
#include "Net/UnrealNetwork.h"



ARemoteButton::ARemoteButton()
{

	bReplicateMovement = true;

	GetStaticMeshComponent()->SetSimulatePhysics(true);
	GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);

}


void ARemoteButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARemoteButton::BeginPlay()
{
	Super::BeginPlay();

}
