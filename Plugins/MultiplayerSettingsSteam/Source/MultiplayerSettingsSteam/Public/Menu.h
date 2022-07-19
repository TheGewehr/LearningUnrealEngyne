// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "Menu.generated.h"


/**
 * 
 */



UCLASS()
class MULTIPLAYERSETTINGSSTEAM_API UMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	void MenuSetup(int32 NumberPublicConnections = 4, FString TypeOfMatch = FString(TEXT("Sandbox")), FString LobbyPath = FString(TEXT("/Game/ThirdPerson/Maps/DefaultMap")));

protected:

	virtual bool Initialize() override;

	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

	//Callbacks for the custom Delegates on the MultiplayerSessionsSubsystem
	UFUNCTION()
	void OnCreateSession(bool wasSuccessful);

	
	void OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool wasSuccessful);

	
	void OnJoinSession(EOnJoinSessionCompleteResult::Type Result);

	UFUNCTION()
		void OnDestroySession(bool wasSuccessful);

	UFUNCTION()
		void OnStartSession(bool wasSuccessful);

private:

	UPROPERTY(meta=(BindWidget))
	class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinButton;

	UFUNCTION()
	void HostButtonClicked();

	UFUNCTION()
	void JoinButtonClicked();

	void MenuTeardown();

	class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;

	int32 NumPublicConnections{4};
	FString MatchType{ TEXT("Sandbox") };
	FString PathToTheLoby{ TEXT("/Game/ThirdPerson/Maps/DefaultMap?listen") };
};
