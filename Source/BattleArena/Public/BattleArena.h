// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
// "CoreMinimal.h" ��� ���, ���� ��ü�� �ε�, �������� ���ɼ�
//#include "EngineMinimal.h"


// ĳ���� ������Ʈ ������
UENUM(BlueprintType)
enum class ECharacterState : uint8
{
        PREINIT,
        LOADING,
        READY,
        DEAD
};



// �α�� ī�װ� �߰� �����ũ�� ����
DECLARE_LOG_CATEGORY_EXTERN(BattleArena, Log, All)

// �ڵ����� BattleArena ī�װ��� ���� �α� ��ũ�� ����
#define BALOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))		// �Լ��� ���α��
#define BALOG_S(Verbosity) UE_LOG(BattleArena, Verbosity, TEXT("%s"), *BALOG_CALLINFO)					// �α� �ܰ� ��������
#define BALOG(Verbosity, Format, ...) UE_LOG(BattleArena, Verbosity, TEXT("%s %s"), *BALOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__)) // �α� �ܰ�� ������ġ��

// ��Ÿ�ӿ��� ������ ������� �����α׿� �Լ� ��ȯ
#define BACHECK(Expr, ...) {if(!(Expr)) { BALOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__; } }