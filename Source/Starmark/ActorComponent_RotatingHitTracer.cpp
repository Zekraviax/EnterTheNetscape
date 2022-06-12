// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent_RotatingHitTracer.h"

// Sets default values for this component's properties
UActorComponent_RotatingHitTracer::UActorComponent_RotatingHitTracer()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UActorComponent_RotatingHitTracer::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UActorComponent_RotatingHitTracer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

