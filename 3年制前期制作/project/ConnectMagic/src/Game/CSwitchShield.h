#pragma once
#pragma once
#include "CSwitchObject.h"

// 何も通さない
// スイッチでオフにできるシールド
// 一度オフにするとオンにならない
class CSwitchShield : public CSwitchObject
{
public:
	// CSavableの純粋仮想関数のオーバーライド
	std::vector<char> SaveState() const override;
	void LoadState(const std::vector<char>& data) override;
	size_t GetTypeID() const override;
	unsigned int GetUniqueInstanceID() const override;

	// コンストラクタ
	CSwitchShield(CVector scale = CVector(1.0f, 1.0f, 1.0f));
	// デストラクタ
	~CSwitchShield();

	// 描画
	void Render() override;

private:
	// データ保存に使用
	unsigned int mUniqueID;

	void UpdateOff() override;
	void UpdateOn() override;

	// コライダーを生成
	void CreateCol() override;
	// コライダー
	CCollider* mpCol1;
	CCollider* mpCol2;
};