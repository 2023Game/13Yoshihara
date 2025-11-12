#pragma once
#pragma once
#include "CObjectBase.h"

// 何も通さないスイッチでオンオフできるシールド
class CSwitchShield : public CObjectBase
{
public:
	// コンストラクタ
	CSwitchShield();
	// デストラクタ
	~CSwitchShield();

	// 描画
	void Render() override;

private:
	// コライダーを生成
	void CreateCol() override;
	// コライダー
	CCollider* mpCol1;
	CCollider* mpCol2;
};