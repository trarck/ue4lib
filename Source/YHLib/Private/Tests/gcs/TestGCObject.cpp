// Fill out your copyright notice in the Description page of Project Settings.

#include "TestGCObject.h"

DEFINE_LOG_CATEGORY_STATIC(LogTestGC, Log, All);

UTestGCObject::UTestGCObject()
	:MyName(TEXT(""))
{
	UE_LOG(LogTestGC, Log, TEXT("UTestGCObject::Construct"));
}

UTestGCObject::~UTestGCObject()
{
	UE_LOG(LogTestGC, Log, TEXT("UTestGCObject::Destroy[%s]"), *(MyName.ToString()));
}

void UTestGCObject::BeginDestroy()
{
	Super::BeginDestroy();
	UE_LOG(LogTestGC, Log, TEXT("UTestGCObject::BeginDestroy[%s]"), *(MyName.ToString()));
}

void UTestGCObject::FinishDestroy()
{
	Super::FinishDestroy();
	UE_LOG(LogTestGC, Log, TEXT("UTestGCObject::FinishDestroy[%s]"), *(MyName.ToString()));
}
