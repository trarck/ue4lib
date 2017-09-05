// Fill out your copyright notice in the Description page of Project Settings.

#include "YHUtils.h"

int32 UYHUtils::GetObjReferenceCount(UObject* Obj, TArray<UObject*>* OutReferredToObjects)
{
    if(!Obj || !Obj->IsValidLowLevelFast()) 
    {
        return -1;
    }
 
    TArray<UObject*> ReferredToObjects;             //req outer, ignore archetype, recursive, ignore transient
    FReferenceFinder ObjectReferenceCollector( ReferredToObjects, Obj, false, true, true, false);
    ObjectReferenceCollector.FindReferences( Obj );
 
    if(OutReferredToObjects)
    {
        OutReferredToObjects->Append(ReferredToObjects);
        return OutReferredToObjects->Num();
    }
    return -1;
}



