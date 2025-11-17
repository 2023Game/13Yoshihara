#pragma once
#include "MoveState.h"

class CPlayer;
class CWeight;
class CMoveObj;
class CSwitchMoveFloor;
class CSwitchMoveWall;
class CConnectTarget;

// プレイヤーのデータ
struct PlayerData {
	CVector pos;		// 座標
	CVector vec;		// 向き
	CConnectTarget* target;	// 接続中のターゲット
	int animationType;		// アニメーションの番号
	float animationFrame;	// アニメーションの進行時間

	// コンストラクタ
	PlayerData(const CVector& p, const CVector& v, CConnectTarget* t, int type, float frame)
		: pos(p), vec(v), target(t), animationType(type), animationFrame(frame) {}
};

// 重りのデータ
struct weightData {
	CVector pos;	// 座標
	CWeight* weight;		// 重り

	// コンストラクタ
	weightData(const CVector& p, CWeight* b)
		: pos(p), weight(b) {}
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

// スイッチで移動する壁のデータ
struct MoveWallData {
	CVector pos;			// 座標
	EMoveState state;		// 状態
	float elapsedTime;		// 移動の経過時間
	CSwitchMoveWall* moveWall;	// 壁

	// コンストラクタ
	MoveWallData(const CVector& p, EMoveState s, float t,
		CSwitchMoveWall* o)
		: pos(p), state(s), elapsedTime(t), moveWall(o) {}
};

// ゲーム全体のデータ
struct GameData {
	PlayerData player = PlayerData(CVector::zero, CVector::zero, nullptr, 0, 0.0f);
	std::vector<weightData> weightes;
	std::vector<MoveObjData> moveObjs;
	std::vector<MoveFloorData> moveFloor;
	std::vector<MoveWallData> moveWall;
};