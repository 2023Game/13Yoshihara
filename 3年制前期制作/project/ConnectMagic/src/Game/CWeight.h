#pragma once
#include "CConnectObject.h"
#include "CSavable.h"

class CWeight : public CConnectObject, public CSavable
{
public:
	// CSavableの純粋仮想関数のオーバーライド
	std::vector<char> SaveState() const override;
	void LoadState(const std::vector<char>& data) override;
	size_t GetTypeID() const override;
	unsigned int GetUniqueInstanceID() const override;

	// コンストラクタ
	CWeight(CVector defaultPos, float scaleRatio = 1.0f);
	// デストラクタ
	~CWeight();

	// 更新
	void Update() override;

	// 重りがスイッチに張り付いているかを設定
	void SetIsAttach(bool enable);
	// 重りがスイッチに張り付いているか
	bool GetIsAttach() const;

	// 衝突処理
	void OnCollision(const CollisionData& data) override;

private:
	// データ保存で使用
	unsigned int mUniqueID;
	void SetElapsedTime(float time);
	float GetElapsedTime() const;


	// コライダーを生成
	void CreateCol() override;

	// 初期座標
	CVector mDefaultPos;
	// リスポーンする
	bool mIsRespawn;
	// 経過時間
	float mElapsedTime;

	// 重りがスイッチに張り付いているか
	bool mIsAttach;
};