// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "FIT2097_A2Character.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "GameFramework/InputSettings.h"
#include "Net/UnrealNetwork.h"
#include "ReplicatedActor.h"

//////////////////////////////////////////////////////////////////////////
// AFIT2097_A2Character

AFIT2097_A2Character::AFIT2097_A2Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input


void AFIT2097_A2Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFIT2097_A2Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFIT2097_A2Character::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AFIT2097_A2Character::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AFIT2097_A2Character::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AFIT2097_A2Character::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AFIT2097_A2Character::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AFIT2097_A2Character::OnResetVR);

	//*****************MY CODE*********************************
	PlayerInputComponent->BindAction("Switch", IE_Pressed, this, &AFIT2097_A2Character::SwitchTraceLine);
	PlayerInputComponent->BindAction("SwitchInteract", IE_Pressed, this, &AFIT2097_A2Character::SwitchInteract);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AFIT2097_A2Character::InteractFunction);
	//PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AFIT2097_A2Character::serverFunction);
	//PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AFIT2097_A2Character::clientFunction);
	//*****************MY CODE*********************************
}

//*****************MY CODE*********************************
//posion
void AFIT2097_A2Character::Tick(float DeltaTime)
{
	CallMyTrace();
	currentHealth -= 0.01f;
}
//*****************MY CODE*********************************

void AFIT2097_A2Character::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AFIT2097_A2Character::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AFIT2097_A2Character::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AFIT2097_A2Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFIT2097_A2Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AFIT2097_A2Character::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AFIT2097_A2Character::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}



//***************************************************************************************************
//** Trace functions - used to detect items we are looking at in the world
//***************************************************************************************************
//***************************************************************************************************

//***************************************************************************************************
//** Trace() - called by our CallMyTrace() function which sets up our parameters and passes them through
//***************************************************************************************************

bool AFIT2097_A2Character::Trace(
	UWorld* World,
	TArray<AActor*>& ActorsToIgnore,
	const FVector& Start,
	const FVector& End,
	FHitResult& HitOut,
	ECollisionChannel CollisionChannel = ECC_Pawn,
	bool ReturnPhysMat = false
)
{

	// The World parameter refers to our game world (map/level) 
	// If there is no World, abort
	if (!World)
	{
		return false;
	}

	// Set up our TraceParams object
	FCollisionQueryParams TraceParams(FName(TEXT("My Trace")), true, ActorsToIgnore[0]);

	// Should we simple or complex collision?
	TraceParams.bTraceComplex = true;

	// We don't need Physics materials 
	TraceParams.bReturnPhysicalMaterial = ReturnPhysMat;

	// Add our ActorsToIgnore
	TraceParams.AddIgnoredActors(ActorsToIgnore);

	// When we're debugging it is really useful to see where our trace is in the world
	// We can use World->DebugDrawTraceTag to tell Unreal to draw debug lines for our trace
	// (remove these lines to remove the debug - or better create a debug switch!)
	if (SwitchON == true) {
		const FName TraceTag("MyTraceTag");

		// World->DebugDrawTraceTag = TraceTag;
		TraceParams.TraceTag = TraceTag;
		DrawDebugLine(GetWorld(), Start, End, FColor::Green, true, 2.0f, 2, 2.0f);
	}
	// Force clear the HitData which contains our results
	HitOut = FHitResult(ForceInit);

	// Perform our trace
	World->LineTraceSingleByChannel
	(
		HitOut,		//result
		Start,	//start
		End, //end
		CollisionChannel, //collision channel
		TraceParams
	);

	// If we hit an actor, return true
	return (HitOut.GetActor() != NULL);
}

//***************************************************************************************************
//** CallMyTrace() - sets up our parameters and then calls our Trace() function
//***************************************************************************************************

void AFIT2097_A2Character::CallMyTrace()
{
	// Get the location of the camera (where we are looking from) and the direction we are looking in
	const FVector Start = this->GetActorLocation();
	const FVector ForwardVector = FollowCamera->GetForwardVector();

	// How far in front of our character do we want our trace to extend?
	// ForwardVector is a unit vector, so we multiply by the desired distance
	const FVector End = Start + ForwardVector * 256;

	// Force clear the HitData which contains our results
	FHitResult HitData(ForceInit);

	// What Actors do we want our trace to Ignore?
	TArray<AActor*> ActorsToIgnore;

	//Ignore the player character - so you don't hit yourself!
	ActorsToIgnore.Add(this);

	// Call our Trace() function with the paramaters we have set up
	// If it Hits anything
	if (Trace(GetWorld(), ActorsToIgnore, Start, End, HitData, ECC_Visibility, false))
	{	
		//*****************MY CODE*********************************
		//*****************MY CODE*********************************
		if (SwitchInteractOn) {   
			// Process our HitData
			if (HitData.GetActor())
			{
				// UE_LOG(LogClass, Warning, TEXT("This a testing statement. %s"), *HitData.GetActor()->GetName());
				ProcessTraceHit(HitData, wasInteracted);
			}
			else
			{
				// The trace did not return an Actor
				// An error has occurred
				// Record a message in the error log
			}
			//*****************MY CODE*********************************
			//*****************MY CODE*********************************
		}
		else {
			// Process our HitData
			if (HitData.GetActor())
			{
				// UE_LOG(LogClass, Warning, TEXT("This a testing statement. %s"), *HitData.GetActor()->GetName());
				ProcessTraceHit(HitData);
			}
			else
			{
				// The trace did not return an Actor
				// An error has occurred
				// Record a message in the error log
			}
		}
	}
	else
	{
		// We did not hit an Actor
		ClearPickupInfo();

	}

}

//***************************************************************************************************
//** ProcessTraceHit() - process our Trace Hit result
//***************************************************************************************************

void AFIT2097_A2Character::ProcessTraceHit(FHitResult& HitOut)
{

	// Cast the actor to APickup
	AReplicatedActor* const TestPickup = Cast<AReplicatedActor>(HitOut.GetActor());

	if (TestPickup)
	{
		// Keep a pointer to the Pickup
		CurrentPickup = TestPickup;

		// Set a local variable of the PickupName for the HUD
		//UE_LOG(LogClass, Warning, TEXT("PickupName: %s"), *TestPickup->GetPickupName());
		PickupName = TestPickup->GetPickupName();

		// Set a local variable of the PickupDisplayText for the HUD
		//UE_LOG(LogClass, Warning, TEXT("PickupDisplayText: %s"), *TestPickup->GetPickupDisplayText());
		PickupDisplayText = TestPickup->GetPickupDisplayText();
		PickupFound = true;
		CollectItem(CurrentPickup);
	}
	else
	{
		//UE_LOG(LogClass, Warning, TEXT("TestPickup is NOT a Pickup!"));
		//temp
		ClearPickupInfo();
	}
}

//*****************MY CODE*********************************
//*****************MY CODE*********************************

void AFIT2097_A2Character::ProcessTraceHit(FHitResult& HitOut, bool n_wasInteracted)
{

	// Cast the actor to APickup
	AReplicatedActor* const TestPickup = Cast<AReplicatedActor>(HitOut.GetActor());

	if (TestPickup)
	{
		// Keep a pointer to the Pickup
		CurrentPickup = TestPickup;

		// Set a local variable of the PickupName for the HUD
		//UE_LOG(LogClass, Warning, TEXT("PickupName: %s"), *TestPickup->GetPickupName());
		PickupName = TestPickup->GetPickupName();
		// Set a local variable of the PickupDisplayText for the HUD
		//UE_LOG(LogClass, Warning, TEXT("PickupDisplayText: %s"), *TestPickup->GetPickupDisplayText());
		PickupDisplayText = TestPickup->GetPickupDisplayText();
		PickupFound = true;
		
		if (n_wasInteracted) {
			CollectItem(CurrentPickup);
			wasInteracted = false;
		}
		/*
		// Keep a pointer to the Pickup
		CurrentPickup = TestPickup;

		// Set a local variable of the PickupName for the HUD
		//UE_LOG(LogClass, Warning, TEXT("PickupName: %s"), *TestPickup->GetPickupName());
		PickupName = TestPickup->GetPickupName();

		// Set a local variable of the PickupDisplayText for the HUD
		//UE_LOG(LogClass, Warning, TEXT("PickupDisplayText: %s"), *TestPickup->GetPickupDisplayText());
		PickupDisplayText = TestPickup->GetPickupDisplayText();
		PickupFound = true;
		CollectItem(CurrentPickup);
		*/
	}
	else
	{
		//UE_LOG(LogClass, Warning, TEXT("TestPickup is NOT a Pickup!"));
		//temp
		ClearPickupInfo();
	}
}


FString AFIT2097_A2Character::MyRole() {
	if (Role == ROLE_Authority)
	{
		return TEXT("Server Side");
	}
	else {
		return TEXT("Client Side");
	}
}


void AFIT2097_A2Character::InteractFunction()
{
	//server is called on interacting with items
	wasInteracted = true;
	ServerFunction();
	//Client
	//ClientFunction();
}


bool AFIT2097_A2Character::ServerFunction_Validate() {
	// Code that would be run on the server before implementation
	return true;
}
void AFIT2097_A2Character::ServerFunction_Implementation()
{
	// Code that would be run on the server after validation
	if (Role == ROLE_Authority)
	{
		CallMyTrace();
	}
}


bool AFIT2097_A2Character::CollectItem_Validate(AReplicatedActor* item) {
	return item != nullptr;
}

void AFIT2097_A2Character::CollectItem_Implementation(AReplicatedActor* item) {

	if (Cast<AReplicatedActor>(item) != nullptr) 
	{


		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 12.0f, FColor::Yellow, PickupName);
			if (PickupName == "Key") {
				bhasKey = true;
				DestoryItem(CurrentPickup);
			}
			if (PickupName == "Fuse") {
				bhasFuse = true;
				DestoryItem(CurrentPickup);
			}
			if (PickupName == "HealthPack") {
				currentHealth += 40;
				DestoryItem(CurrentPickup);
			}
			if (PickupName == "PowerBox") {
				if (bhasFuse) {
					bPower = true;
					bhasFuse = false;
					GEngine->AddOnScreenDebugMessage(-1, 12.0f, FColor::Yellow, "You have used Fuse to the PowerBox");
					item->use();
				}
				else {
					GEngine->AddOnScreenDebugMessage(-1, 12.0f, FColor::Yellow, "You need a Fuse!");
				}
			}
			if (PickupName == "RemoteButton1") {
				bRB1 = true;
				GEngine->AddOnScreenDebugMessage(-1, 12.0f, FColor::Yellow, "You pressed the RemoteButton1");
			}
			if (PickupName == "RemoteButton2") {
				bRB2 = true;
				GEngine->AddOnScreenDebugMessage(-1, 12.0f, FColor::Yellow, "You pressed the RemoteButton2");
			}
			if (PickupName == "Door_key") {
				if (bhasKey) {
					GEngine->AddOnScreenDebugMessage(-1, 12.0f, FColor::Yellow, "Key Door is active now");
					item->use();
				}
			}
			if (PickupName == "Door_fuse") {
				if (bPower) {
					GEngine->AddOnScreenDebugMessage(-1, 12.0f, FColor::Yellow, "Fuse Door is active now");
					item->use();
				}
			}
			if (PickupName == "Door_remote1") {
				if (bRB1) {
					GEngine->AddOnScreenDebugMessage(-1, 12.0f, FColor::Yellow, "Remote Door 1 is active now");
					item->use();
				}
			}
			if (PickupName == "Door_remote2") {
				if (bRB1) {
					GEngine->AddOnScreenDebugMessage(-1, 12.0f, FColor::Yellow, "Remote Door 2 is active now");
					item->use();
				}
			}
		}
	}

}

bool AFIT2097_A2Character::DestoryItem_Validate(AReplicatedActor* item) {
	return true; 
}

void AFIT2097_A2Character::DestoryItem_Implementation(AReplicatedActor* item)
{
	if (Role == ROLE_Authority) {
		if (IsValid(item)) {
			item->Destroy(true);
		}
	}
}

void AFIT2097_A2Character::ClearPickupInfo()
{
	PickupName = "";
	PickupDisplayText = "";
}

void AFIT2097_A2Character::SwitchTraceLine()
{
	if (SwitchON == true)
	{
		SwitchON = false;
	}
	else
	{
		SwitchON = true;
	}
}

void AFIT2097_A2Character::SwitchInteract()
{
	if (SwitchInteractOn == true)
	{
		SwitchInteractOn = false;
	}
	else
	{
		SwitchInteractOn = true;
	}
}

//set up network, client to server
//https://answers.unrealengine.com/questions/671789/client-variable-update-for-all-clients-dedicated-s.html
void AFIT2097_A2Character::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFIT2097_A2Character, currentHealth);
	DOREPLIFETIME(AFIT2097_A2Character, bhasKey);
	DOREPLIFETIME(AFIT2097_A2Character, bhasFuse);

}

void AFIT2097_A2Character::setcurrentHealth(float newhealth) {
	currentHealth = newhealth;
	if (Role < ROLE_Authority) {
		ServersetcurrentHealth(currentHealth);
	}
}
bool AFIT2097_A2Character::ServersetcurrentHealth_Validate(float newhealth) {
	return true; 
}

void AFIT2097_A2Character::ServersetcurrentHealth_Implementation(float newhealth)
{
	setcurrentHealth(newhealth);
}

void AFIT2097_A2Character::setbhasFuse(bool newbhasFuse) {
	bhasFuse = newbhasFuse;
	if (Role < ROLE_Authority) {
		ServersetbhasFuse(bhasFuse);
	}
}

bool AFIT2097_A2Character::ServersetbhasFuse_Validate(bool newbhasFuse) {
	return true;
}

void AFIT2097_A2Character::ServersetbhasFuse_Implementation(bool newbhasFuse)
{
	setbhasFuse(newbhasFuse);
}


void AFIT2097_A2Character::setbhasKey(bool newbhasKey) {
	bhasKey = newbhasKey;
	if (Role < ROLE_Authority) {
		ServersetbhasKey(bhasKey);
	}
}

bool AFIT2097_A2Character::ServersetbhasKey_Validate(bool newbhasKey) {
	return true;
}

void AFIT2097_A2Character::ServersetbhasKey_Implementation(bool newbhasKey)
{
	setbhasKey(newbhasKey);
}



void AFIT2097_A2Character::setbpower(bool newbpower) {
	bPower = newbpower;
	if (Role < ROLE_Authority) {
		Serversetbpower(newbpower);
	}
}

bool AFIT2097_A2Character::Serversetbpower_Validate(bool newbpower) {
	return true;
}

void AFIT2097_A2Character::Serversetbpower_Implementation(bool newbpower)
{
	setbpower(newbpower);
}



void AFIT2097_A2Character::setbrb1(bool newbrb1) {
	bRB1 = newbrb1;
	if (Role < ROLE_Authority) {
		Serversetbrb1(newbrb1);
	}
}

bool AFIT2097_A2Character::Serversetbrb1_Validate(bool newbrb1) {
	return true;
}

void AFIT2097_A2Character::Serversetbrb1_Implementation(bool newbrb1)
{
	setbrb1(newbrb1);
}



void AFIT2097_A2Character::setbrb2(bool newbrb2) {
	bRB2 = newbrb2;
	if (Role < ROLE_Authority) {
		Serversetbrb2(newbrb2);
	}
}

bool AFIT2097_A2Character::Serversetbrb2_Validate(bool newbrb2) {
	return true;
}

void AFIT2097_A2Character::Serversetbrb2_Implementation(bool newbrb2)
{
	setbrb2(newbrb2);
}


//void AFIT2097_A2Character::OnRep_status()
//{
//	//TODO
//}
//
//void AFIT2097_A2Character::updateStatus(float dt)
//{
//	if (Role == ROLE_Authority)
//	{
//		OnRep_status();
//	}
//}
//*****************MY CODE*********************************
//*****************MY CODE*********************************
