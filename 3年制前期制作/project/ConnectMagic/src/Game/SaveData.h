#pragma once
#include "CPlayer.h"

class CConnectTarget;

// プレイヤーのデータ
struct PlayerData {
	CVector pos;			// 座標
	CQuaternion rot;		// 向き
	CConnectTarget* target;	// 接続中のターゲット
	int animationType;		// アニメーションの番号
	float animationFrame;	// アニメーションの進行時間
	CPlayer::EState state;	// プレイヤーの状態
	CVector linearVelocity;	// 移動速度
	CVector angularVelocity;// 回転速度
	float connectDist;

	// コンストラクタ
	PlayerData() = default;
	PlayerData(const CVector& p, const CQuaternion& r, CConnectTarget* t, int type, float frame,
		CPlayer::EState pState,
		const CVector& l, const CVector& a,
		float dist)
		: pos(p), rot(r), target(t), animationType(type), animationFrame(frame), state(pState),
	linearVelocity(l), angularVelocity(a), connectDist(dist){}
};

// オブジェクトのデータ
struct ObjectData {
	// 巻き戻し対象のオブジェクトID
	unsigned int instanceID;
	// オブジェクトタイプID
	size_t typeID;
	// オブジェクトの状態をシリアライズしたデータ
	std::vector<char> rawData;

	// コンストラクタ
	ObjectData(unsigned int id, size_t type_id, std::vector<char>&& data)
		: instanceID(id), typeID(type_id), rawData(std::move(data)) {}
};

// ゲーム全体のデータ
struct GameData {
	PlayerData playerData;
	// すべての保存可能なオブジェクトの状態を保存するリスト
	std::vector<ObjectData> objects;
};