#pragma once

class CRoomBase;

class CRoomManager
{
public:
	// インスタンスを取得
	static CRoomManager* Instance();

	// コンストラクタ
	CRoomManager();
	// デストラクタ
	~CRoomManager();

	// 更新
	void Update();

	// 部屋のリストを取得
	std::vector<CRoomBase*> GetRooms() const;

private:
	// インスタンス
	static CRoomManager* spInstance;

	// 部屋を作成
	void CreateRoom();

	// 部屋のリスト
	std::vector<CRoomBase*> mRooms;
};