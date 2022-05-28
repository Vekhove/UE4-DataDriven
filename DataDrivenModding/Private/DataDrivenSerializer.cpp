#include "DataDrivenSerializer.h"

#include "ImageWriteBlueprintLibrary.h"

ADataDrivenSerializer::ADataDrivenSerializer()
{
	PrimaryActorTick.bCanEverTick = false;
	JsonObject = MakeShareable(new FJsonObject);
}

void ADataDrivenSerializer::BeginPlay()
{
	Super::BeginPlay();
}

TSharedPtr<FJsonObject> ADataDrivenSerializer::GetJsonObject()
{
	return JsonObject;
}

void ADataDrivenSerializer::SetJsonObject(TSharedPtr<FJsonObject> InJsonObject)
{
	JsonObject = InJsonObject;
}

ADataDrivenSerializer* ADataDrivenSerializer::AddStringValue(FString Key, FString Value)
{
	JsonObject->SetStringField(Key, Value);
	return this;
}

ADataDrivenSerializer* ADataDrivenSerializer::AddNameValue(FString Key, FName Value)
{
	JsonObject->SetStringField(Key, Value.ToString());
	return this;
}

ADataDrivenSerializer* ADataDrivenSerializer::AddTextValue(FString Key, FText Value)
{
	JsonObject->SetStringField(Key, Value.ToString());
	return this;
}

ADataDrivenSerializer* ADataDrivenSerializer::AddIntegerValue(FString Key, int32 Value)
{
	JsonObject->SetNumberField(Key, Value);
	return this;
}

ADataDrivenSerializer* ADataDrivenSerializer::AddFloatValue(FString Key, float Value)
{
	JsonObject->SetNumberField(Key, Value);
	return this;
}

ADataDrivenSerializer* ADataDrivenSerializer::AddBooleanValue(FString Key, bool Value)
{
	JsonObject->SetBoolField(Key, Value);
	return this;
}

ADataDrivenSerializer* ADataDrivenSerializer::AddVectorValue(FString Key, FVector Value)
{
	TArray<TSharedPtr<FJsonValue>> VectorArray;
	VectorArray.Add(MakeShareable(new FJsonValueNumber(Value.X)));
	VectorArray.Add(MakeShareable(new FJsonValueNumber(Value.Y)));
	VectorArray.Add(MakeShareable(new FJsonValueNumber(Value.Z)));
	JsonObject->SetArrayField(Key, VectorArray);
	return this;
}

ADataDrivenSerializer* ADataDrivenSerializer::AddStringArrayValue(FString Key, TArray<FString> StringArray)
{
	TArray<TSharedPtr<FJsonValue>> JsonValueArray;

	for (FString String : StringArray) JsonValueArray.Add(MakeShareable(new FJsonValueString(String)));
	JsonObject->SetArrayField(Key, JsonValueArray);
	return this;
}

ADataDrivenSerializer* ADataDrivenSerializer::AddJsonObject(FString Key, ADataDrivenSerializer* Value)
{
	JsonObject->SetObjectField(Key, Value->GetJsonObject());
	return this;
}

ADataDrivenSerializer* ADataDrivenSerializer::AddTextureValue(FString Key, UTexture2D* Value, FString ValuePath, FString FilePath)
{
	JsonObject->SetStringField(Key, ValuePath);
	
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (PlatformFile.FileExists(*FilePath)) return this;

	FImageWriteOptions WriteOptions;
	WriteOptions.Format = EDesiredImageFormat::PNG;
	WriteOptions.bOverwriteFile = true;
	WriteOptions.bAsync = true;
	WriteOptions.CompressionQuality = 100;
	UImageWriteBlueprintLibrary::ExportToDisk(Value, FilePath, WriteOptions);
	
	return this;
}

bool ADataDrivenSerializer::SaveToFile(FString Path, FString FileName)
{
	FString OutputString;
	const TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	
	if (PlatformFile.CreateDirectoryTree(*Path))
	{
		const FString AbsolutePath = Path.Append(FileName.EndsWith(".json") ? FileName : FileName.Append(".json"));

		if (!PlatformFile.FileExists(*AbsolutePath)) FFileHelper::SaveStringToFile(*OutputString, *AbsolutePath);
		return true;
	}
	
	return false;
}

TArray<ADataDrivenSerializer*> ADataDrivenSerializer::LoadFromFolder(FString Folder, TArray<FString> DirectoriesToIgnore, bool& Success, TArray<FString>& FileNames, TArray<FString>& FilePaths)
{
	TArray<ADataDrivenSerializer*> Serializers;
	
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	FLocalTimestampDirectoryVisitor Visitor(PlatformFile, DirectoriesToIgnore, DirectoriesToIgnore, false);
	PlatformFile.IterateDirectory(*Folder, Visitor);

	for (TMap<FString, FDateTime>::TIterator TimestampItr(Visitor.FileTimes); TimestampItr; ++TimestampItr)
	{
		const FString FilePath = TimestampItr.Key();
		const FString FileName = FPaths::GetCleanFilename(FilePath);
		bool bDoSkip = false;

		for (FString DirectoryToIgnore : DirectoriesToIgnore)
		{
			bDoSkip = FilePath.Contains("/" + DirectoryToIgnore + "/");
		}

		if (!bDoSkip && FileName.EndsWith(".json"))
		{
			FileNames.Add(FileName);
			FilePaths.Add(FilePath);
		}
	}

	for (FString Path : FilePaths)
	{
		bool bSuccess;
		ADataDrivenSerializer* DataDrivenSerializer = NewObject<ADataDrivenSerializer>();
		Serializers.Add(DataDrivenSerializer);
		DataDrivenSerializer->LoadFromFile(Path, bSuccess);
	}
	
	Success = Serializers.IsValidIndex(0);
	return Serializers;
}

ADataDrivenSerializer* ADataDrivenSerializer::LoadFromFile(FString File, bool& Success)
{
	FString JsonString;
	FFileHelper::LoadFileToString(JsonString, *File);
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);
	Success = FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid();
	return this;
}

ADataDrivenSerializer* ADataDrivenSerializer::GetStringValue(FString Key, FString& Value, bool& Success)
{
	Success = JsonObject->TryGetStringField(Key, Value);
	return this;
}

ADataDrivenSerializer* ADataDrivenSerializer::GetNameValue(FString Key, FName& Value, bool& Success)
{
	FString ValueStr;
	Success = JsonObject->TryGetStringField(Key, ValueStr);
	if (Success) Value = FName(*ValueStr);
	return this;
}

ADataDrivenSerializer* ADataDrivenSerializer::GetTextValue(FString Key, FText& Value, bool& Success)
{
	FString ValueStr;
	Success = JsonObject->TryGetStringField(Key, ValueStr);
	if (Success) Value = FText::AsCultureInvariant(ValueStr);
	return this;
}

ADataDrivenSerializer* ADataDrivenSerializer::GetIntegerValue(FString Key, int32& Value, bool& Success)
{
	Success = JsonObject->TryGetNumberField(Key, Value);
	return this;
}

ADataDrivenSerializer* ADataDrivenSerializer::GetFloatValue(FString Key, float& Value, bool& Success)
{
	double ValueDouble;
	Success = JsonObject->TryGetNumberField(Key, ValueDouble);
	if (Success) Value = ValueDouble;
	return this;
}

ADataDrivenSerializer* ADataDrivenSerializer::GetBooleanValue(FString Key, bool& Value, bool& Success)
{
	Success = JsonObject->TryGetBoolField(Key, Value);
	return this;
}

ADataDrivenSerializer* ADataDrivenSerializer::GetVectorValue(FString Key, FVector& Value, bool& Success)
{
	TArray<TSharedPtr<FJsonValue>> Array = JsonObject->GetArrayField(Key);
	Success = Array.IsValidIndex(0);
	if (Success) Value = FVector(Array[0].Get()->AsNumber(), Array[1].Get()->AsNumber(), Array[2].Get()->AsNumber());
	return this;
}

ADataDrivenSerializer* ADataDrivenSerializer::GetStringArrayValue(FString Key, TArray<FString>& Value, bool& Success)
{
	TArray<TSharedPtr<FJsonValue>> Array = JsonObject->GetArrayField(Key);
	Success = Array.IsValidIndex(0);
	if (Success)
	{
		for (TSharedPtr<FJsonValue> JsonValue : Array) Value.Add(JsonValue.Get()->AsString());
	}
	return this;
}

ADataDrivenSerializer* ADataDrivenSerializer::GetJsonObject(FString Key, ADataDrivenSerializer*& Value, bool& Success)
{
	TSharedPtr<FJsonObject> JsonObj = JsonObject->GetObjectField(Key);
	Success = JsonObj.IsValid();
	
	if (Success) 
	{
		Value = NewObject<ADataDrivenSerializer>();
		Value->SetJsonObject(JsonObj);
	}
	return this;
}

ADataDrivenSerializer* ADataDrivenSerializer::GetTextureValue(FString Key, FString RootDirectory, UTexture2D*& Value, bool& Success)
{
	FString LocalPath;
	Success = JsonObject->TryGetStringField(Key, LocalPath);
	if (!Success) return this;
	
	FString AbsolutePath = RootDirectory.Append(LocalPath);
	Success = FPaths::FileExists(AbsolutePath);
	if (!Success) return this;

	Value = UKismetRenderingLibrary::ImportFileAsTexture2D(this, AbsolutePath);
	Value->LODGroup = TextureGroup::TEXTUREGROUP_Pixels2D;
	Value->CompressionSettings = TextureCompressionSettings::TC_EditorIcon;
	Value->UpdateResource();

	/*FTexture2DMipMap* MipMaps = &Value->PlatformData->Mips[0];
	FByteBulkData* RawImageData = &MipMaps->BulkData;

	TArray<FColor> OutBMP;
	OutBMP.SetNumUninitialized(Value->GetSizeX(), Value->GetSizeY());
	FColor* FormattedImageData = static_cast<FColor*>(RawImageData->Lock(LOCK_READ_ONLY));
	if (FormattedImageData)
	{
		for (int i = 0; i < (Value->GetSizeX() * Value->GetSizeY()); ++i)
			{
				
			}
	}
	
	FCreateTexture2DParameters Params;
	Params.CompressionSettings = TextureCompressionSettings::TC_EditorIcon;
	Params.bUseAlpha = true;
	Params.TextureGroup = TextureGroup::TEXTUREGROUP_Pixels2D;
	FImageUtils::CreateTexture2D(Value->GetSizeX(), Value->GetSizeY(), static_cast<FColor*>(RawImageData->Lock(LOCK_READ_ONLY)), Value, TEXT(""), EObjectFlags::RF_Dynamic, Params);*/
	
	return this;
}
