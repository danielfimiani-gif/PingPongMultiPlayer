#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MultiplayerSessionsSubsystem.generated.h"

USTRUCT(BlueprintType)
struct FSessionSearchResult
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	FString HostName;
	
	UPROPERTY(BlueprintReadOnly)
	int32 CurrentPlayers = 0;
	
	UPROPERTY(BlueprintReadOnly)
	int32 MaxPlayers = 0;
	
	UPROPERTY(BlueprintReadOnly)
	int32 PingInMs = 0;
	
	UPROPERTY(BlueprintReadOnly)
	int32 SessionIndex = -1;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnCreateSessionComplete, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMultiplayerOnFindSessionComplete, const TArray<FSessionSearchResult>&, SessionResult, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnJoinSessionComplete, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnDestoySessionComplete, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnStartSessionComplete, bool, bWasSuccessful);

UCLASS()
class PINGPONGMULTIPLAYER_API UMultiplayerSessionsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UMultiplayerSessionsSubsystem();
	
	UFUNCTION(BlueprintCallable, Category="Session")
	void CreateSession(int32 NumPublicConnections, FString MatchType, FString HostName);
	
	UFUNCTION(BlueprintCallable, Category="Session")
	void FindSessions(int32 MaxSearchResults);
	
	UFUNCTION(BlueprintCallable, Category="Session")
	void JoinSessionByIndex(int32 SessionIndex);
	
	UFUNCTION(BlueprintCallable, Category="Session")
	void DestroySession();
	
	UFUNCTION(BlueprintCallable, Category="Session")
	void StartSession();
	
	UPROPERTY(BlueprintAssignable, Category="Session")
	FMultiplayerOnCreateSessionComplete MultiplayerOnCreateSessionComplete;
	
	UPROPERTY(BlueprintAssignable, Category="Session")
	FMultiplayerOnFindSessionComplete MultiplayerOnFindSessionComplete;
	
	UPROPERTY(BlueprintAssignable, Category="Session")
	FMultiplayerOnJoinSessionComplete  MultiplayerOnJoinSessionComplete;
	
	UPROPERTY(BlueprintAssignable, Category="Session")
	FMultiplayerOnDestoySessionComplete MultiplayerOnDestroySessionComplete;
	
	UPROPERTY(BlueprintAssignable, Category="Session")
	FMultiplayerOnStartSessionComplete MultiplayerOnStartSessionComplete;
	
protected:
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
	void OnStartSessionComplete(FName SessionName, bool bWasSuccessful);
	
private:
	IOnlineSessionPtr SessionInterface;
	
	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;
	TSharedPtr<FOnlineSessionSearch> LastSessionSearch;
	
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FDelegateHandle CreateSessionCompleteDelegateHandle;
	
	FOnFindSessionsCompleteDelegate FindSessionCompleteDelegate;
	FDelegateHandle FindSessionCompleteDelegateHandle;
	
	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	FDelegateHandle JoinSessionCompleteDelegateHandle;
	
	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
	FDelegateHandle DestroySessionCompleteDelegateHandle;
	
	FOnStartSessionCompleteDelegate StartSessionCompleteDelegate;
	FDelegateHandle StartSessionCompleteDelegateHandle;
};
