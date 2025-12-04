#pragma once
#pragma once
#include "CSwitchObject.h"

// 何も通さない
// スイッチでオフにできるシールド
// 一度オフにするとオンにならない
class CSwitchShield : public CSwitchObject
{
public:
	// コンストラクタ
	CSwitchShield(CVector scale = CVector(1.0f, 1.0f, 1.0f));
	// デストラクタ
	~CSwitchShield();

	// 描画
	void Render() override;

private:
	void UpdateOff() override;
	void UpdateOn() override;

	// コライダーを生成
	void CreateCol() override;
	// コライダー
	CCollider* mpCol1;
	CCollider* mpCol2;
};