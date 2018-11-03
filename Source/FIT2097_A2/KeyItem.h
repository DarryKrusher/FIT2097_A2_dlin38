// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ReplicatedActor.h"
#include "KeyItem.generated.h"

/**
 * 
 */
UCLASS()
class FIT2097_A2_API AKeyItem : public AReplicatedActor
{
	GENERATED_BODY()
	
public:

	AKeyItem();

	virtual void Tick(float DeltaTime) override;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Property)
	FString Name = "Key Item";

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Property)
	FString DisplayTest = "You can pick this key up";


protected:

	virtual void BeginPlay() override;
	
	
};
