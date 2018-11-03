// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ReplicatedActor.h"
#include "RemoteButton.generated.h"

/**
 * 
 */
UCLASS()
class FIT2097_A2_API ARemoteButton : public AReplicatedActor
{
	GENERATED_BODY()
	
public:

	ARemoteButton();

	virtual void Tick(float DeltaTime) override;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Property)
	FString Name = "RemoteButton";

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Property)
	FString DisplayTest = "You can interact with this button to open a remote door";


protected:

	virtual void BeginPlay() override;
	
	
};
