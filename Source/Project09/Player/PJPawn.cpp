// PJPawn.cpp

#include "Player/PJPawn.h"
#include "Project09.h"

void APJPawn::BeginPlay()
{
	Super::BeginPlay();

	FString NetRoleString = Project09FunctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(TEXT("PJPawn::BeginPlay() %s [%s]"), *Project09FunctionLibrary::GetNetModeString(this), *NetRoleString);
	Project09FunctionLibrary::MyPrintString(this, CombinedString, 10.f);
}

void APJPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	FString NetRoleString = Project09FunctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(TEXT("PJPawn::PossessedBy() %s [%s]"), *Project09FunctionLibrary::GetNetModeString(this), *NetRoleString);
	Project09FunctionLibrary::MyPrintString(this, CombinedString, 10.f);
}
