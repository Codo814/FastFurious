// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemSpawner.h"
#include "ItemSpawner.h"
#include "Components/BillboardComponent.h"

// Sets default values
AItemSpawner::AItemSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	SetRootComponent(BillboardComponent);
}

// Called when the game starts or when spawned
void AItemSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SpawnItem();
	}
	
}

void AItemSpawner::SpawnItem()
{
	if(!HasAuthority()||IsValid(ItemClass) == false)
	{
		return;
	}

	UWorld* World = GetWorld();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	SpawnedItem = World->SpawnActorDeferred<AActor>(ItemClass, GetActorTransform(),this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		SpawnedItem->OnDestroyed.AddDynamic(this, &AItemSpawner::OnItemDestroyed);
		SpawnedItem->FinishSpawning(GetActorTransform());
}

void AItemSpawner::OnItemDestroyed(AActor* DestroyedActor)
{
	if (!HasAuthority() || IsValid(ItemClass) == false) {
		return;
	}
	SpawnedItem = nullptr;
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AItemSpawner::SpawnItem, SpawnInterval, false);
}
