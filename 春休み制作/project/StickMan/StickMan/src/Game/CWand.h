#pragma once
#include "CWeapon.h"

class CModel;
class CConnectPoint;

// 杖のクラス
class CWand : public CWeapon
{
public:
	// コンストラクタ
	CWand(CObjectBase* owner, ETag tag);
	// デストラクタ
	~CWand();

	// 更新
	void Update() override;

	// 描画
	void Render() override;

private:
	CModel* mpModel;		// 杖のモデル
};
