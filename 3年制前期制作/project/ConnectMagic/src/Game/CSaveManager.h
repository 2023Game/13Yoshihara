#pragma once
#include "SaveData.h"
#include <deque>
#include "CRewindUI.h"

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

	// 保存する重りに追加
	void AddWeight(CWeight* weight);
	// 保存する重りから削除
	void DeleteWeight(CWeight* weight);

	// 保存する移動オブジェクトに追加
	void AddMoveObj(CMoveObj* obj);
	// 保存する移動オブジェクトから削除
	void DeleteMoveObj(CMoveObj* obj);

	// 保存するスイッチで移動する床に追加
	void AddMoveFloor(CSwitchMoveFloor* floor);
	// 保存するスイッチで移動する床から削除
	void DeleteMoveFloor(CSwitchMoveFloor* floor);

	// 保存するスイッチで移動する壁に追加
	void AddMoveWall(CSwitchMoveWall* wall);
	// 保存するスイッチで移動する壁から削除
	void DeleteMoveFloor(CSwitchMoveWall* wall);

	// 保存するスイッチで移動する空中オブジェクトに追加
	void AddMoveAirObj(CSwitchMoveAirObj* air);
	// 保存するスイッチで移動する空中オブジェクトから削除
	void DeleteMoveAirObj(CSwitchMoveAirObj* air);

	// 保存するスイッチに追加
	void AddSwitch(CSwitch* switchObj);
	// 保存するスイッチから削除
	void DeleteSwitch(CSwitch* switchObj);

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

	// 巻き戻しUI
	CRewindUI* mpRewindUI;

	// 保存データ
	// deque：前後への追加と削除が多いときに使う
	std::deque<GameData> mData;

	// 保存する重り
	std::vector<CWeight*> mWeights;
	// 保存する移動オブジェクト
	std::vector<CMoveObj*> mMoveObjs;
	// 保存するスイッチで移動する床
	std::vector<CSwitchMoveFloor*> mMoveFloors;
	// 保存するスイッチで移動する壁
	std::vector<CSwitchMoveWall*> mMoveWalls;
	// 保存するスイッチで移動する空中オブジェクト
	std::vector<CSwitchMoveAirObj*> mMoveAirObjs;
	// 保存するスイッチ
	std::vector<CSwitch*> mSwitchs;
};