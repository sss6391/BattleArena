// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
// "CoreMinimal.h" 대신 사용, 엔진 전체를 로딩, 성능저하 가능성
//#include "EngineMinimal.h"


// 캐릭터 스테이트 열거형
UENUM(BlueprintType)
enum class ECharacterState : uint8
{
        PREINIT,
        LOADING,
        READY,
        DEAD
};



// 로깅용 카테고리 추가 공용매크로 설정
DECLARE_LOG_CATEGORY_EXTERN(BattleArena, Log, All)

// 자동으로 BattleArena 카테고리를 쓰는 로그 매크로 정의
#define BALOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))		// 함수와 라인기록
#define BALOG_S(Verbosity) UE_LOG(BattleArena, Verbosity, TEXT("%s"), *BALOG_CALLINFO)					// 로그 단계 조절가능
#define BALOG(Verbosity, Format, ...) UE_LOG(BattleArena, Verbosity, TEXT("%s %s"), *BALOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__)) // 로그 단계와 포멧합치기

// 런타임에서 문제가 있을경우 에러로그와 함수 반환
#define BACHECK(Expr, ...) {if(!(Expr)) { BALOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__; } }