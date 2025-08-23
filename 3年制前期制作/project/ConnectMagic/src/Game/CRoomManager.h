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

	// 部屋を有効
	void RoomOn();
	// 部屋を無効
	void RoomOff();

private:
	// インスタンス
	static CRoomManager* spInstance;

	// 部屋を作成
	void CreateRoom();

	// 部屋のリスト
	std::vector<CRoomBase*> mRooms;

	// 有効にする部屋の番号
	int mOnNum;
	// 無効にする部屋の番号
	int mOffNum;
};