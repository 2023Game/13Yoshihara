#pragma once
#include "RoadType.h"

class CDeliveryField;
class CDeliveryHome;
class CDeliveryObstruction;
class CDeliveryFieldItem;
class CSky;

/*
配達のフィールド管理クラス
*/
class CDeliveryFieldManager
{
public:
	static CDeliveryFieldManager* Instance();
	// コンストラクタ
	CDeliveryFieldManager();
	// デストラクタ
	~CDeliveryFieldManager();

	// 更新
	void Update();

	/// <summary>
	/// 指定のフィールドを取得する
	/// </summary>
	/// <param name="num">指定する番号</param>
	/// <param name="end">最後の要素ならtrue</param>
	/// <returns></returns>
	CDeliveryField* GetField(int num, bool& end);

private:
	// フィールドを生成
	void CreateField();
	// 家を生成するか判定する
	void CreateHome();
	// 障害物を生成するか判定する
	void CreateObstruction();
	// 障害物の座標をランダムで設定
	void ObstructionRandomPos(CDeliveryObstruction* obstruction);

	// アイテムを生成するか判定する
	void CreateFieldItem();
	// アイテムの座標をランダムで設定
	void ItemRandomPos(CDeliveryFieldItem* fieldItem);

	// フィールド管理クラスのインスタンス
	static CDeliveryFieldManager* spInstance;

	// フィールド
	std::vector<CDeliveryField*> mFields;
	// 家
	std::vector<CDeliveryHome*> mHomes;
	// 障害物
	std::vector<CDeliveryObstruction*> mObstructions;
	// アイテム
	std::vector<CDeliveryFieldItem*> mFieldItems;

	// 一番遠いフィールド
	CDeliveryField* mpFarField;
	
	// 最後に家の生成判定をしたZ座標
	float mLastHomePopZ;
	// 最後に障害物の生成判定をしたZ座標
	float mLastObstructionPopZ;
	// 最後に障害物の生成をした道
	ERoadType mLastObstructionRoad;
	// 最後にアイテムの生成判定をしたZ座標
	float mLastFieldItemPopZ;
	// 最後にアイテムの生成をした道
	ERoadType mLastItemRoad;
};