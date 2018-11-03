// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "ReplicatedActor.generated.h"

/**
 * 
 */
UCLASS()
class FIT2097_A2_API AReplicatedActor : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:
	
	// make a constructor
	AReplicatedActor();
	
	//network require
	//void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


//*****************MY CODE*********************************
//*****************MY CODE*********************************

	//	// Create Event
	//UFUNCTION(BlueprintImplementableEvent, Category = gameplay)
	//	void interact();

	//create event for bp interact
	UFUNCTION(BlueprintNativeEvent, Category = gameplay)
		void use();

	virtual void use_Implementation();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Property, Replicated)
		//FString Name;
		FString Name = "Pickup Name";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Property, Replicated)
		//FString DisplayText;
		FString DisplayText = "Pickup object";

	FString GetPickupName() { return Name; }

	FString GetPickupDisplayText() { return DisplayText; }

protected:


public:
	virtual void InteractWithActor();

private:


		//*****************MY CODE*********************************
//*****************MY CODE*********************************
};
