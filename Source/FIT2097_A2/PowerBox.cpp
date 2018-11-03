// Fill out your copyright notice in the Description page of Project Settings.

#include "PowerBox.h"
#include "FIT2097_A2.h"
#include "Net/UnrealNetwork.h"



APowerBox::APowerBox()
{

	bReplicateMovement = true;

	GetStaticMeshComponent()->SetSimulatePhysics(true);
	GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);

}


void APowerBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APowerBox::BeginPlay()
{
	Super::BeginPlay();

}