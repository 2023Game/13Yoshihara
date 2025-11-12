#include "CRoomManager.h"
#include "CConnectRoom.h"
#include "CRoom1.h"
#include "CRoom2.h"
#include "CRoom3.h"
#include "CRoom4.h"
#include "CRoom5.h"
#include "CRoom6.h"
#include "CLastRoom.h"
#include "CSaveManager.h"

CRoomManager* CRoomManager::spInstance = nullptr;

// インスタンスを取得
CRoomManager* CRoomManager::Instance()
{
	return spInstance;
}

// コンストラクタ
CRoomManager::CRoomManager()
	: mOnNum(2)
	, mOffNum(0)
{
	spInstance = this;
	CreateRoom();
}

// デストラクタ
CRoomManager::~CRoomManager()
{
}

// 更新
void CRoomManager::Update()
{
}

std::vector<CRoomBase*> CRoomManager::GetRooms() const
{
	return mRooms;
}

// 部屋を有効
void CRoomManager::RoomOn()
{
	// 部屋を有効
	// 2回処理する
	for (int i = 0; i < 2; i++)
	{
		// サイズ以上なら処理を終了
		if (mOnNum >= mRooms.size()) return;

		mRooms[mOnNum]->SetEnableRoom(true);
		mOnNum++;
	}
}

// 部屋を無効
void CRoomManager::RoomOff()
{
	// 処理回数
	int num = 2;
	// 初回なら1回だけw
	if (mOffNum == 0) num = 1;

	// 部屋を無効
	// 2回処理する
	for (int i = 0; i < num; i++)
	{
		// サイズ以上なら処理を終了
		if (mOffNum >= mRooms.size()) return;

		mRooms[mOffNum]->SetEnableRoom(false);
		mOffNum++;
	}

	// 全てのデータを削除
	CSaveManager::Instance()->AllDelete();
}

// 部屋を作成
void CRoomManager::CreateRoom()
{
	int i = 1;
	CRoomBase* room = nullptr;					// 部屋
	CConnectRoom* connectRoom = nullptr;		// 接続部屋
	CVector pos = CVector::zero;		// 前の部屋を生成した座標
	CVector offsetPos = CVector::zero;	// 今の部屋のオフセット座標

	while (true)
	{
		// 部屋を生成
		switch (i)
		{
			// 部屋1
		case 1:
			room = new CRoom1(pos + offsetPos);
			// 有効
			room->SetEnableRoom(true);
			break;
		//	// 部屋2
		//case 2:
		//	room = new CRoom2(pos + offsetPos);
		//	break;
		//	// 部屋3
		//case 3:
		//	room = new CRoom3(pos + offsetPos);
		//	break;
		//	// 部屋4
		//case 4:
		//	room = new CRoom4(pos + offsetPos);
		//	break;
		//	// 部屋5
		//case 5:
		//	room = new CRoom5(pos + offsetPos);
		//	break;
		case 2:
			room = new CRoom6(pos + offsetPos);
			break;
			// 全ての部屋の生成が終了した
		default:
			// 最後の部屋を生成
			room = new CLastRoom(pos + offsetPos);
			// リストに追加
			mRooms.push_back(room);
			// 無効
			room->SetEnableRoom(false);

			// 接続部屋があるなら
			if (connectRoom != nullptr)
			{
				// 次の部屋を設定
				connectRoom->SetNextRoom(room);
			}
			return;
			break;
		}

		// 座標を更新
		pos = room->Position();
		// オフセット座標を更新
		offsetPos = CVector(0.0f, 0.0f, -room->GetRoomLength());
		// リストに追加
		mRooms.push_back(room);
		// 最初以外は無効
		if (i != 1)
		{
			room->SetEnableRoom(false);
		}

		// 接続部屋があるなら
		if (connectRoom != nullptr)
		{
			// 次の部屋を設定
			connectRoom->SetNextRoom(room);
		}
		// 部屋と部屋の接続用の部屋を生成
		connectRoom = new CConnectRoom(pos + offsetPos);
		// 前の部屋を設定
		connectRoom->SetPreRoom(room);
		// 座標を更新
		pos = connectRoom->Position();
		// オフセット座標を更新
		offsetPos = CVector(0.0f, 0.0f, -connectRoom->GetRoomLength());
		// リストに追加
		mRooms.push_back(connectRoom);
		// 最初以外は無効
		if (i != 1)
		{
			connectRoom->SetEnableRoom(false);
		}

		// 次へ
		i++;
	}
}
