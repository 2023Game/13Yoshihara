#pragma once
#include "MoveState.h"

class CPlayer;
class CBox;
class CMoveObj;
class CSwitchMoveFloor;

// プレイヤーのデータ
struct PlayerData {
	CVector pos;		// 座標
	CVector vec;		// 向き
};

// 箱のデータ
struct BoxData {
	CVector pos;	// 座標
	CBox* box;		// 箱

	// コンストラクタ
	BoxData(const CVector& p, CBox* b)
		: pos(p), box(b) {}
};

// 移動するオブジェクトのデータ
struct MoveObjData {
	CVector pos;			// 座標
	EMoveState state;		// 状態
	EMoveState preState;	// 1つ前の状態
	float elapsedTime;	// 移動の経過時間
	CMoveObj* moveObj;	// 移動オブジェクト

	// コンストラクタ
	MoveObjData(const CVector& p, EMoveState s, EMoveState preS, float t,
		CMoveObj* o)
		: pos(p), state(s), preState(preS), elapsedTime(t), moveObj(o) {}
};

// スイッチで移動する床のデータ
struct MoveFloorData {
	CVector pos;		// 座標
	EMoveState state;		// 状態
	EMoveState preState;	// 1つ前の状態
	float elapsedTime;	// 移動の経過時間
	CSwitchMoveFloor* moveFloor;	// 床

	// コンストラクタ
	MoveFloorData(const CVector& p, EMoveState s, EMoveState preS, float t,
		CSwitchMoveFloor* o)
		: pos(p), state(s), preState(preS), elapsedTime(t), moveFloor(o) {}
};

// ゲーム全体のデータ
struct GameData {
	PlayerData player;
	std::vector<BoxData> boxes;
	std::vector<MoveObjData> moveObjs;
	std::vector<MoveFloorData> moveFloor;
};