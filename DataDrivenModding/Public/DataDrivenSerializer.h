#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IImageWrapper.h"
#include "ImageUtils.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Misc/LocalTimestampDirectoryVisitor.h"
#include "DataDrivenSerializer.generated.h"


UCLASS()
class ADataDrivenSerializer : public AActor
{
	GENERATED_BODY()

	TSharedPtr<FJsonObject> JsonObject;

public:	
	ADataDrivenSerializer();

protected:
	virtual void BeginPlay() override;

public:
	
	TSharedPtr<FJsonObject> GetJsonObject();
	void SetJsonObject(TSharedPtr<FJsonObject> InJsonObject);

	// Serialization

	UFUNCTION(BlueprintCallable, Category="DataDriven")
	ADataDrivenSerializer* AddStringValue(FString Key, FString Value);
	
	UFUNCTION(BlueprintCallable, Category="DataDriven")
	ADataDrivenSerializer* AddNameValue(FString Key, FName Value);
	
	UFUNCTION(BlueprintCallable, Category="DataDriven")
	ADataDrivenSerializer* AddTextValue(FString Key, FText Value);

	UFUNCTION(BlueprintCallable, Category="DataDriven")
	ADataDrivenSerializer* AddIntegerValue(FString Key, int32 Value);

	UFUNCTION(BlueprintCallable, Category="DataDriven")
	ADataDrivenSerializer* AddFloatValue(FString Key, float Value);

	UFUNCTION(BlueprintCallable, Category="DataDriven")
	ADataDrivenSerializer* AddBooleanValue(FString Key, bool Value);

	UFUNCTION(BlueprintCallable, Category="DataDriven")
	ADataDrivenSerializer* AddVectorValue(FString Key, FVector Value);
	
	UFUNCTION(BlueprintCallable, Category="DataDriven")
	ADataDrivenSerializer* AddStringArrayValue(FString Key, TArray<FString> StringArray);

	UFUNCTION(BlueprintCallable, Category="DataDriven")
	ADataDrivenSerializer* AddJsonObject(FString Key, ADataDrivenSerializer* Value);

	UFUNCTION(BlueprintCallable, Category="DataDriven")
	ADataDrivenSerializer* AddTextureValue(FString Key, UTexture2D* Value, FString ValuePath, FString FilePath);
	
	UFUNCTION(BlueprintCallable, Category="DataDriven")
	bool SaveToFile(FString Path, FString FileName);

	// Deserialization

	UFUNCTION(BlueprintCallable, Category="DataDriven")
	TArray<ADataDrivenSerializer*> LoadFromFolder(FString Folder, TArray<FString> DirectoriesToIgnore, bool&Success, TArray<FString>& FileNames, TArray<FString>& FilePaths);
	
	UFUNCTION(BlueprintCallable, Category="DataDriven")
	ADataDrivenSerializer* LoadFromFile(FString File, bool& Success);

	UFUNCTION(BlueprintCallable, Category="DataDriven")
	ADataDrivenSerializer* GetStringValue(FString Key, FString& Value, bool& Success);

	UFUNCTION(BlueprintCallable, Category="DataDriven")
	ADataDrivenSerializer* GetNameValue(FString Key, FName& Value, bool& Success);

	UFUNCTION(BlueprintCallable, Category="DataDriven")
	ADataDrivenSerializer* GetTextValue(FString Key, FText& Value, bool& Success);

	UFUNCTION(BlueprintCallable, Category="DataDriven")
	ADataDrivenSerializer* GetIntegerValue(FString Key, int32& Value, bool& Success);

	UFUNCTION(BlueprintCallable, Category="DataDriven")
	ADataDrivenSerializer* GetFloatValue(FString Key, float& Value, bool& Success);

	UFUNCTION(BlueprintCallable, Category="DataDriven")
	ADataDrivenSerializer* GetBooleanValue(FString Key, bool& Value, bool& Success);

	UFUNCTION(BlueprintCallable, Category="DataDriven")
	ADataDrivenSerializer* GetVectorValue(FString Key, FVector& Value, bool& Success);

	UFUNCTION(BlueprintCallable, Category="DataDriven")
	ADataDrivenSerializer* GetStringArrayValue(FString Key, TArray<FString>& Value, bool& Success);

	UFUNCTION(BlueprintCallable, Category="DataDriven")
	ADataDrivenSerializer* GetJsonObject(FString Key, ADataDrivenSerializer*& Value, bool& Success);

	UFUNCTION(BlueprintCallable, Category="DataDriven")
	ADataDrivenSerializer* GetTextureValue(FString Key, FString RootDirectory, UTexture2D*& Value, bool& Success);
};
