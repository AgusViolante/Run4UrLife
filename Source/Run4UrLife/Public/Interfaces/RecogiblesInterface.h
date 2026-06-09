// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Publicl/Items/ItemBase.h"
#include "UObject/Interface.h"
#include "RecogiblesInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URecogiblesInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RUN4URLIFE_API IRecogiblesInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void RecibirObjeto(EItemState NuevoEstado, int32 Cantidad) = 0;
};
