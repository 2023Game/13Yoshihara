#pragma once
#include "CObjectBase.h"

class CImage;
class CTextUI2D;

// 2D空間に配置する配達のHPUIクラス
class CDeliveryHpUI2D : public CObjectBase
{
public:
	// コンストラクタ
	CDeliveryHpUI2D(CObjectBase* owner);
	// デストラクタ
	~CDeliveryHpUI2D();

	// 持ち主を設定
	void SetOwner(CObjectBase* owner);

	// 現在値を設定
	void SetCurrPoint(int point);

	// 更新
	void Update() override;
	// 描画
	void Render() override;

private:
	CObjectBase* mpOwner;	// HPUIの持ち主

	CImage* mpDeliveryItemImg;		// 配達物のイメージ
	CTextUI2D* mpNumText;			// 持っている配達物の数のテキスト

	int mCurrPoint;	// 現在値
};