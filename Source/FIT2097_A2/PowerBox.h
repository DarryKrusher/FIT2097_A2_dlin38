// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ReplicatedActor.h"
#include "PowerBox.generated.h"

/**
 * 
 */
UCLASS()
class FIT2097_A2_API APowerBox : public AReplicatedActor
{
	GENERATED_BODY()
	

public:

	APowerBox();

	virtual void Tick(float DeltaTime) override;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Property)
	FString Name = "PowerBox";

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Property)
	FString DisplayTest = "It requires a fuse";


protected:

	virtual void BeginPlay() override;

	
	
};
