#pragma once

class CDeliveryField;
class CDeliveryHome;
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

	// フィールド管理クラスのインスタンス
	static CDeliveryFieldManager* spInstance;

	// フィールド
	std::vector<CDeliveryField*> mFields;
	// 家
	std::vector<CDeliveryHome*> mHomes;

	// 一番遠いフィールド
	CDeliveryField* mpFarField;
	// 最後に家の生成判定をしたZ座標
	float mLastPopZ;
};