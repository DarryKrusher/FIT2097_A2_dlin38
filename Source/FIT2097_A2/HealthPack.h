// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ReplicatedActor.h"
#include "HealthPack.generated.h"

/**
 * 
 */
UCLASS()
class FIT2097_A2_API AHealthPack : public AReplicatedActor
{
	GENERATED_BODY()
	
public:

	AHealthPack();

	virtual void Tick(float DeltaTime) override;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Property)
	FString Name = "HealthPack";

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Property)
	FString DisplayTest = "You can get healing from it";


protected:

	virtual void BeginPlay() override;
	
	
};
