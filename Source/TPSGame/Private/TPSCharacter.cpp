// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "ConstructorHelpers.h"
#include "Animation/AnimBlueprint.h"
#include "Animation/AnimInstance.h"
#include "..\Public\TPSCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ATPSCharacter::ATPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringComp"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->SetRelativeLocation(FVector(0, 0, 90));

	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> FoundAnimBP(TEXT("/Game/Blueprint/BP_Character_AnimBP.BP_Character_AnimBP"));
	check(FoundAnimBP.Succeeded());
	UAnimBlueprintGeneratedClass* PreviewBP = FoundAnimBP.Object->GetAnimBlueprintGeneratedClass();
	if(PreviewBP)
		GetMesh()->SetAnimInstanceClass(PreviewBP);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshContainer(TEXT("SkeletalMesh'/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (MeshContainer.Succeeded())
		GetMesh()->SetSkeletalMesh(MeshContainer.Object);

	bCrouchButtonDown = false;
	MaxSpeed = 300.0f;
}

// Called when the game starts or when spawned
void ATPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATPSCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);
	if (Value > 0.0f)
		UE_LOG(LogTemp, Log, TEXT("MoveForward: %s"), *FString::SanitizeFloat(Value, 3));
}

void ATPSCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);

}

void ATPSCharacter::BeginFire()
{
	UE_LOG(LogTemp, Log, TEXT("BeginFire"));
}

void ATPSCharacter::StopFire()
{
	UE_LOG(LogTemp, Log, TEXT("StopFire"));
}

void ATPSCharacter::BeginCrouch()
{
	bCrouchButtonDown = true;
	GetCharacterMovement()->MaxWalkSpeed = MaxSpeed / 2;
	//CameraComp->SetRelativeLocation(FVector(0, 0, 70));
}

void ATPSCharacter::StopCrouch()
{
	bCrouchButtonDown = false;
	GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;
	//CameraComp->SetRelativeLocation(FVector(0, 0, 90));
}

// Called every frame
void ATPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bCrouchButtonDown)
	{
		
		CameraComp->RelativeLocation.Z = FMath::Lerp(CameraComp->RelativeLocation.Z, 40.0f, 0.1f);
		CameraComp->RelativeLocation.X = FMath::Lerp(CameraComp->RelativeLocation.X, 80.0f, 0.1f);
	}
	else
	{
		CameraComp->RelativeLocation.Z = FMath::Lerp(CameraComp->RelativeLocation.Z, 90.0f, 0.1f);
		CameraComp->RelativeLocation.X = FMath::Lerp(CameraComp->RelativeLocation.X, 0.0f, 0.1f);
	}

}

// Called to bind functionality to input
void ATPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("LookUp", this, &ATPSCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &ATPSCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATPSCharacter::MoveRight);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ATPSCharacter::BeginFire);
	PlayerInputComponent->BindAction("Fire", IE_Released,this,  &ATPSCharacter::StopFire);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ATPSCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ATPSCharacter::StopCrouch);

}

