// Fill out your copyright notice in the Description page of Project Settings.

#include "Fuse.h"
#include "FIT2097_A2.h"
#include "Net/UnrealNetwork.h"

AFuse::AFuse()
{

	bReplicateMovement = true;

	GetStaticMeshComponent()->SetSimulatePhysics(true);
	GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);

}


//void AFuse::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

void AFuse::BeginPlay()
{
	Super::BeginPlay();

}
