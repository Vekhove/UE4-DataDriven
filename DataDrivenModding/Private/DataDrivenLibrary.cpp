#include "DataDrivenLibrary.h"

#include "DataDrivenSerializer.h"
#include "GameFramework/Character.h"

ADataDrivenSerializer* UDataDrivenLibrary::CreateJsonObject()
{
	return NewObject<ADataDrivenSerializer>();
}
