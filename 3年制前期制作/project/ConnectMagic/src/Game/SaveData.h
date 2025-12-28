#pragma once

class CConnectTarget;

// プレイヤーのデータ
struct PlayerData {
	CVector pos;			// 座標
	CQuaternion vec;		// 向き
	CConnectTarget* target;	// 接続中のターゲット
	int animationType;		// アニメーションの番号
	float animationFrame;	// アニメーションの進行時間

	// コンストラクタ
	PlayerData() = default;
	PlayerData(const CVector& p, const CQuaternion& v, CConnectTarget* t, int type, float frame)
		: pos(p), vec(v), target(t), animationType(type), animationFrame(frame) {}
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