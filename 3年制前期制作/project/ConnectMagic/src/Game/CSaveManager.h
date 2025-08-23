#pragma once
#include "SaveData.h"
#include <deque>

// 保存の管理クラス
class CSaveManager : CTask
{
public:
	// インスタンスを取得
	static CSaveManager* Instance();

	// コンストラクタ
	CSaveManager();
	// デストラクタ
	~CSaveManager();

	// 更新
	void Update() override;

	// 状態を保存する
	void Save();

	// 一番古い状態を削除する
	void DeleteOld();
	// 一番新しい状態を削除
	void DeleteNew();

	// 全て削除する
	void AllDelete();

	// データをロード
	void Load();

	// 保存する箱に追加
	void AddBox(CBox* box);
	// 保存する箱から削除
	void DeleteBox(CBox* box);

	// 保存する移動オブジェクトに追加
	void AddMoveObj(CMoveObj* obj);
	// 保存する移動オブジェクトから削除
	void DeleteMoveObj(CMoveObj* obj);

	// 保存するスイッチで移動する床に追加
	void AddMoveFloor(CSwitchMoveFloor* floor);
	// 保存するスイッチで移動する床から削除
	void DeleteMoveFloor(CSwitchMoveFloor* floor);

	// 保存管理クラスの状態
	enum class EState
	{
		eSave,	// セーブ
		eLoad,	// ロード
	};
	// 状態切り替え
	void ChangeState(EState state);

private:
	// インスタンス
	static CSaveManager* spInstance;

	EState mState;	// 状態


	// 保存データ
	// deque：前後への追加と削除が多いときに使う
	std::deque<GameData> mData;

	// 保存する箱
	std::vector<CBox*> mBox;
	// 保存する移動オブジェクト
	std::vector<CMoveObj*> mMoveObj;
	// 保存するスイッチで移動する床
	std::vector<CSwitchMoveFloor*> mMoveFloor;
};