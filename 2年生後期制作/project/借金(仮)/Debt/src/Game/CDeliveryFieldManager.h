#pragma once

class CDeliveryField;
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

	// フィールド管理クラスのインスタンス
	static CDeliveryFieldManager* spInstance;

	// フィールド
	std::vector<CDeliveryField*> mFields;

	// 一番奥のフィールドのZ座標
	float mFieldPosZ;
};