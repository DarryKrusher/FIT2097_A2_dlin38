// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ReplicatedActor.h"
#include "Fuse.generated.h"

/**
 * 
 */
UCLASS()
class FIT2097_A2_API AFuse : public AReplicatedActor
{
	GENERATED_BODY()

public:

	AFuse();

	//virtual void Tick(float DeltaTime) override;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Property)
	FString Name = "Fuse";

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Property)
	FString DisplayTest = "You can pick this fuse up";


	
protected:

	virtual void BeginPlay() override;
	
};
