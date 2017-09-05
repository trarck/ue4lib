// Fill out your copyright notice in the Description page of Project Settings.

#include "TestCollectionActor.h"
#include "TestItemComponent.h"
#include "Utils/YHUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogTestConllection, Log, All);
// Sets default values
ATestCollectionActor::ATestCollectionActor()
	:Elapsed(0),
	Seq(0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

ATestCollectionActor::~ATestCollectionActor()
{
	UE_LOG(LogTestConllection, Log, TEXT("ATestCollectionActor::Destroy"));
}

// Called when the game starts or when spawned
void ATestCollectionActor::BeginPlay()
{
	Super::BeginPlay();	
	UE_LOG(LogTestConllection, Log, TEXT("ATestCollectionActor::Begin"));

	ArrayObjs.Empty();

	FirstTestItemComponent = NewObject<UTestItemComponent>(this, TEXT("FirstFirst"));
	FirstTestItemComponent->MyName = TEXT("FirstFirst");

	SecondTestItemComponent = NewObject<UTestItemComponent>(this, TEXT("Second"));
	SecondTestItemComponent->MyName = TEXT("Second");
	
	//ShowRefs(FirstTestItemComponent);
}

// Called every frame
void ATestCollectionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Elapsed += DeltaTime;
	if (Elapsed > 1.0f) {
		Elapsed -= 1.0f;
		UE_LOG(LogTestConllection, Log, TEXT("ArrayObjs:%d"),ArrayObjs.Num());

		for (UObject* Each : ArrayObjs)
		{
			UTestItemComponent* Item = (UTestItemComponent*)Each;
			if (Each->IsValidLowLevel())
			{
				UE_LOG(LogTestConllection, Log, TEXT("Item Ok:%s"), *(Item->MyName.ToString()));
			}else{
				UE_LOG(LogTestConllection, Log, TEXT("Item:Des:%s"), *(Item->MyName.ToString()));
			}
		}

		UE_LOG(LogTestConllection, Log, TEXT("NormalArrayObjs:%d"), NormalArrayObjs.Num());

		for (UObject* Each : NormalArrayObjs)
		{
			UTestItemComponent* Item = (UTestItemComponent*)Each;
			if (Each->IsValidLowLevel())
			{
				UE_LOG(LogTestConllection, Log, TEXT("Normal Item Ok:%s"), *(Item->MyName.ToString()));
			}
			else {
				UE_LOG(LogTestConllection, Log, TEXT("Normal Item:Des:%s"), *(Item->MyName.ToString()));
			}
		}

		GetWorld()->ForceGarbageCollection();
		/*	if (FirstTestItemComponent->IsValidLowLevel()) {
				UE_LOG(LogTestConllection, Log, TEXT("First is ok"));
			}else{
				UE_LOG(LogTestConllection, Log, TEXT("First is destroy"));
			}*/
	}
}

void ATestCollectionActor::AddStaticMeshs()
{

	UStaticMesh* MyStaticMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Meshes/Geometry/SM_MyMesh"));
	if (MyStaticMesh != nullptr)
	{
		ArrayItems.Add(MyStaticMesh);
	}

	MyStaticMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Meshes/Geometry/SM_MyMesh2"));
	if (MyStaticMesh != nullptr)
	{
		NormalArrayItems.Add(MyStaticMesh);
	}
}

void ATestCollectionActor::RemoveStaticMeshs()
{
	
}

void ATestCollectionActor::AddObjs()
{
	UTestItemComponent* TestComponent = NULL;// CreateDefaultSubobject<UTestItemComponent>(TEXT("TestA"));

	FName TestName;
	TestName = FName(*FString::Printf(TEXT("TestA-%d"), ++Seq));
	TestComponent = NewObject<UTestItemComponent>(this, TestName);
	TestComponent->MyName = TestName;
	ArrayObjs.Add(TestComponent);

	//ShowRefs(TestComponent);

	TestName = FName(*FString::Printf(TEXT("TestB-%d"), ++Seq));
	TestComponent = NewObject<UTestItemComponent>(this, TestName);
	TestComponent->MyName = TestName;
	NormalArrayObjs.Add(TestComponent);

	//ShowRefs(TestComponent);

	TestName = FName(*FString::Printf(TEXT("TestC-%d"), ++Seq));
	TestComponent = NewObject<UTestItemComponent>(this, TestName);
	TestComponent->MyName = TestName;
	//TestComponent->ConditionalBeginDestroy();
	//ShowRefs(TestComponent);
}

void ATestCollectionActor::RemoveObjs()
{

}
void ATestCollectionActor::ShowRefs(UObject* obj)
{
	TArray<UObject*> ReferredToObjs;

	UYHUtils::GetObjReferenceCount(obj, &ReferredToObjs);

	for (UObject* Each : ReferredToObjs)
	{
		if (Each)
		{
			UE_LOG(LogTestConllection, Log, TEXT("%s"), *Each->GetName());
		}
	}
}

