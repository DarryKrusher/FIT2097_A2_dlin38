// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthPack.h"
#include "FIT2097_A2.h"
#include "Net/UnrealNetwork.h"

AHealthPack::AHealthPack()
{

	bReplicateMovement = true;

	GetStaticMeshComponent()->SetSimulatePhysics(true);
	GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);

}


void AHealthPack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHealthPack::BeginPlay()
{
	Super::BeginPlay();

}


