// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "Fuse.h"
#include "HealthPack.h"
#include "RemoteButton.h"
#include "KeyItem.h"
#include "FIT2097_A2Character.generated.h"

UCLASS(config=Game)
class AFIT2097_A2Character : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AFIT2097_A2Character();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface
	virtual void Tick(float DeltaTime);


public:

	//set up network
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


	//***************************************************************************************************
//** Trace functions - used to detect items we are looking at in the world
//** Adapted from code found on the unreal wiki https://wiki.unrealengine.com/Trace_Functions
//***************************************************************************************************

	bool Trace(
		UWorld* World,
		TArray<AActor*>& ActorsToIgnore,
		const FVector& Start,
		const FVector& End,
		FHitResult& HitOut,
		ECollisionChannel CollisionChannel,
		bool ReturnPhysMat
	);

	void CallMyTrace();

	void ProcessTraceHit(FHitResult& HitOut);

	void ProcessTraceHit(FHitResult& HitOut, bool n_wasInteracted);

	bool wasInteracted = false;

	//*****************MY CODE*********************************
	//*****************MY CODE*********************************

	UFUNCTION(BlueprintPure, Category = "Role")
		FString MyRole();

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GamePlay)
	//bool bhasFuse = false;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GamePlay)
	//bool bhasKey = false;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GamePlay)
	//	float currentHealth = 100;
	/*UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Status")
		void updateStatus(float dt);*/

		//set up network
	//https://answers.unrealengine.com/questions/671789/client-variable-update-for-all-clients-dedicated-s.html

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = GamePlay)
		float currentHealth = 100;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = GamePlay)
		bool bhasFuse = false;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = GamePlay)
		bool bhasKey = false;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = GamePlay)
		bool bPower = false;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = GamePlay)
		bool bRB1 = false;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = GamePlay)
		bool bRB2 = false;

	void setcurrentHealth(float newhealth);
	void setbhasFuse(bool newbhasFuse);
	void setbhasKey(bool newbhasKey);
	void setbpower(bool newbpower);
	void setbrb1(bool newbrb1);
	void setbrb2(bool newbrb2);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GamePlay)
		float maxHealth = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		class AReplicatedActor* CurrentPickup;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
		FString PickupName;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
		FString PickupDisplayText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		bool PickupFound = false;

	void ClearPickupInfo();

	//void clientFunction();
	//void serverFunction();

protected:

	bool TraceLineSwitch = true;

	UFUNCTION(BlueprintCallable, category = gameplay)
		void InteractFunction();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerFunction();
		virtual bool ServerFunction_Validate();
		virtual void ServerFunction_Implementation();
	//UFUNCTION(Server, Reliable, WithValidation)
		//void ClientFunction();

	//Interact with replicated item
	UFUNCTION(Server, Reliable, WithValidation)
		void CollectItem(AReplicatedActor* item);
		virtual bool CollectItem_Validate(AReplicatedActor* item);
		virtual void CollectItem_Implementation(AReplicatedActor* item);

	UFUNCTION(Server, Reliable, WithValidation)
		void DestoryItem(AReplicatedActor* item);
		virtual bool DestoryItem_Validate(AReplicatedActor* item);
		virtual void DestoryItem_Implementation(AReplicatedActor* item);

	void SwitchTraceLine();
	bool SwitchON = true;

	void SwitchInteract();
	bool SwitchInteractOn = true;

	UFUNCTION(Server, Reliable, WithValidation)
		void ServersetcurrentHealth(float newhealth);
		bool ServersetcurrentHealth_Validate(float newhealth);
		void ServersetcurrentHealth_Implementation(float newhealth);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServersetbhasFuse(bool newbhasFuse);
		bool ServersetbhasFuse_Validate(bool newbhasFuse);
		void ServersetbhasFuse_Implementation(bool newbhasFuse);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServersetbhasKey(bool newbhasKey);
		bool ServersetbhasKey_Validate(bool newbhasKey);
		void ServersetbhasKey_Implementation(bool newbhasKey);
	UFUNCTION(Server, Reliable, WithValidation)
		void Serversetbpower(bool newbpower);
		bool Serversetbpower_Validate(bool newbpower);
		void Serversetbpower_Implementation(bool newbpower);
	UFUNCTION(Server, Reliable, WithValidation)
		void Serversetbrb1(bool newbrb1);
		bool Serversetbrb1_Validate(bool newbrb1);
		void Serversetbrb1_Implementation(bool newbrb1);
	UFUNCTION(Server, Reliable, WithValidation)
		void Serversetbrb2(bool newbrb2);
		bool Serversetbrb2_Validate(bool newbrb2);
		void Serversetbrb2_Implementation(bool newbrb2);

	//*****************MY CODE*********************************
	//*****************MY CODE*********************************
};