#pragma once
#include "CObjectBase.h"

class CImage3D;
class CTextUI3D;

// 3D空間に配置する配達のHPUIクラス
class CDeliveryHpUI3D : public CObjectBase
{
public:
	// コンストラクタ
	CDeliveryHpUI3D(CObjectBase* owner);
	//デストラクタ
	~CDeliveryHpUI3D();

	// 持ち主を設定
	void SetOwner(CObjectBase* owner);

	// 現在値を設定
	void SetCurrPoint(int point);

	// 更新
	void Update() override;
	// 描画
	void Render() override;

private:
	CObjectBase* mpOwner;		// HPUIの持ち主

	CImage3D* mpDeliveryItemImg;// 配達物の画像
	CTextUI3D* mpNumText;		// 持っている配達物の数のテキスト 

	int mCurrPoint;		// 現在値
};