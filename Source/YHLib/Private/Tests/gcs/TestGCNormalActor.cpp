// Fill out your copyright notice in the Description page of Project Settings.

#include "TestGCNormalActor.h"
#include "TestGCActorComponent.h"
#include "TestGCObject.h"
#include "Utils/YHUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogTestGC, Log, All);
// Sets default values
ATestGCNormalActor::ATestGCNormalActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

ATestGCNormalActor::~ATestGCNormalActor()
{
	UE_LOG(LogTestGC, Log, TEXT("ATestGCNormalActor::Destroy"));
}

// Called when the game starts or when spawned
void ATestGCNormalActor::BeginPlay()
{
	Super::BeginPlay();	
	UE_LOG(LogTestGC, Log, TEXT("ATestGCNormalActor::Begin"));


	FirstComponent = NewObject<UTestGCActorComponent>(this, TEXT("FirstFirst"));
	FirstComponent->MyName = TEXT("FirstFirst");

	SecondComponent = NewObject<UTestGCActorComponent>(this, TEXT("SecondSecond"));
	SecondComponent->MyName = TEXT("SecondSecond");

    OneObject = NewObject<UTestGCObject>(this, TEXT("OneOne"));
	OneObject->MyName = TEXT("OneOne");

	TwoObject = NewObject<UTestGCObject>(this, TEXT("TwoTwo"));
	TwoObject->MyName = TEXT("TwoTwo");	

	/*UTestGCObject* A= NewObject<UTestGCObject>(this, TEXT("AAAAA"));
	A->MyName = TEXT("AAAAA");
	MyFirstObjs.Add(A);

	UTestGCObject* B = NewObject<UTestGCObject>(this, TEXT("BBBBB"));
	B->MyName = TEXT("BBBBB");
	MySecondObjs.Add(B);


	UTestGCObject* One = NewObject<UTestGCObject>(this, TEXT("11111"));
	One->MyName = TEXT("11111");
	MyFirstMaps.Add(TEXT("11111"), One);

	UTestGCObject* Two = NewObject<UTestGCObject>(this, TEXT("222222"));
	Two->MyName = TEXT("222222");
	MySecondMaps.Add(TEXT("222222"), Two);*/
}

// Called every frame
void ATestGCNormalActor::Tick(float DeltaTime)
{
    
}
