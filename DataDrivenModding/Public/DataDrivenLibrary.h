#pragma once

#include "CoreMinimal.h"
#include "DataDrivenSerializer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DataDrivenLibrary.generated.h"

UENUM(BlueprintType)
enum class EAttribute : uint8
{
	Strength,
	Defence,
	Health,
	Agility
};

UCLASS()
class DATADRIVENMODDING_API UDataDrivenLibrary : public UBlueprintFunctionLibrary
{
	
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, Category="DataDriven")
	static ADataDrivenSerializer* CreateJsonObject();
};