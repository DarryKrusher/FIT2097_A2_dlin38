// Fill out your copyright notice in the Description page of Project Settings.

#include "KeyItem.h"
#include "FIT2097_A2.h"
#include "Net/UnrealNetwork.h"

AKeyItem::AKeyItem()
{

	bReplicateMovement = true;

	GetStaticMeshComponent()->SetSimulatePhysics(true);
	GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);

}

void AKeyItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKeyItem::BeginPlay()
{
	Super::BeginPlay();

}
