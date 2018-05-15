// Copyleft

#include "Mannequin.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "../Weapons/Gun.h"

// Sets default values
AMannequin::AMannequin()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera respect the CapsuleComponent
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);//Create Attachment of the arms to the Camera
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);//Respect the Camera
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);
	
}

// Called when the game starts or when spawned
void AMannequin::BeginPlay()
{
	Super::BeginPlay();
	if (!GunBlueprint)
		{
		UE_LOG(LogTemp, Warning, TEXT("Mannequin needs a GunBlueprint!!"));
		return;
		}

	Gun = GetWorld()->SpawnActor<AGun>(GunBlueprint);
	Gun ->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	Gun->AnimInstance = GetMesh()->GetAnimInstance();

	
	
}

// Called every frame
void AMannequin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMannequin::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// InputComponent is for AI controller, If this mannequin is possessed by a Player will be Null(requires Protection).The oposite inn PlayerInputComponent
	if (!InputComponent) { return; }
	InputComponent->BindAction("Fire", IE_Pressed, this, &AMannequin::PullTrigger);

}

//Called OnFire method in class Gun.
void AMannequin::PullTrigger()
{
	if (!Gun) { return; }
	Gun->OnFire();
}
