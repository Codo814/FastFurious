// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/CXPlayerState.h"
#include "CXPlayerState.h"
#include "Net/UnrealNetwork.h"

static int32 GMyTestCoin = 0;

//FAutoConstolevariableRef 정의 및 람다 콜백 등록
static FAutoConsoleVariableRef CVarAddCoinServer(
	TEXT("Cheat.addcoin.server"),//콘솔 명령창에 입력할 이름
	GMyTestCoin, //연동할 C++ 변수 레퍼런스
	TEXT("add coin request to server."), //도움말
	FConsoleVariableDelegate::CreateLambda([](IConsoleVariable* CVar) //람다 콜백
		{
				UE_LOG(LogTemp, Warning, TEXT("콘솔 변수 변경됨. GMYTestCoin 값: %d"), GMyTestCoin);
				if (GWorld) {
					if (GWorld->GetFirstPlayerController())
					{
						GWorld->GetFirstPlayerController()->GetPlayerState<ACXPlayerState>()->ClientToServerAddCoin(GMyTestCoin);
					}

				GMyTestCoin = 0;
			}
		}),
	ECVF_Cheat//플래그(치트 기능 등으로 분류
);

static FAutoConsoleVariableRef CVarAddCoinClient(
	TEXT("Cheat.addcoin.client"),//콘솔 명령창에 입력할 이름
	GMyTestCoin, //연동할 C++ 변수 레퍼런스
	TEXT("add coin test for client only."), //도움말
	FConsoleVariableDelegate::CreateLambda([](IConsoleVariable* CVar) //람다 콜백
		{
			UE_LOG(LogTemp, Warning, TEXT("콘솔 변수 변경됨. GMYTestCoin 값: %d"), GMyTestCoin);
			if (GWorld) {
				if (GWorld->GetFirstPlayerController())
				{
					GWorld->GetFirstPlayerController()->GetPlayerState<ACXPlayerState>()->Coins += GMyTestCoin;
				}
			}
			GMyTestCoin = 0;

		}),
	ECVF_Cheat//플래그(치트 기능 등으로 분류
);

void ACXPlayerState::ClientToServerAddCoin_Implementation(const int32 Amount)
{
	//서버 시점
	AddCoins(Amount);
}

void ACXPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ThisClass, Coins, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ThisClass, BonusMaxSpeed, COND_OwnerOnly);
}

void ACXPlayerState::AddCoins(int32 Amount)
{
	if (HasAuthority() == false || Amount<=0)
	{
		return;
	}
	FString PlayerName = GetPlayerName();
	Coins += Amount;
	UE_LOG(LogTemp, Warning, TEXT("%s - AddCoins 값: %d(+%d)"), *PlayerName, Coins, Amount);
}

bool ACXPlayerState::TryRemoveCoins(int32 Amount)
{
	if (HasAuthority() == false || Amount <= 0 || Coins < 5)
	{
		return false;
	}

	Coins -= Amount;
	return true;
}

void ACXPlayerState::AddBonusMaxSpeed(float Amount)
{
	if (HasAuthority() == false || Amount <= 0.f)
	{
		return;
	}
	BonusMaxSpeed += Amount;
}

FString ACXPlayerState::GetPlayerStateInfo() const
{
	return FString::Printf(TEXT("Coins: %d / BonusMaxSpeed: %.2f"), Coins, BonusMaxSpeed);
}
