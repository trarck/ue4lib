// Fill out your copyright notice in the Description page of Project Settings.

#include "MyStaticMesh.h"
DEFINE_LOG_CATEGORY_STATIC(LogMyStaticMesh, Log, All);


UMyStaticMesh::UMyStaticMesh(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	UE_LOG(LogMyStaticMesh, Log, TEXT("UMyStaticMesh::Construct"));
}

UMyStaticMesh::~UMyStaticMesh()
{
	UE_LOG(LogMyStaticMesh, Log, TEXT("UMyStaticMesh::Descontruct"));
}
