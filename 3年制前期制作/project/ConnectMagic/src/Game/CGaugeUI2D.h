#pragma once
#include "CObjectBase.h"

class CImage;

// 2D空間に配置するイメージクラス
class CGaugeUI2D : public CObjectBase
{
public:
	// コンストラクタ
	CGaugeUI2D(CObjectBase* owner, std::string gaugePath, bool addTaskList = true);
	// デストラクタ
	virtual ~CGaugeUI2D();

	// 持ち主を設定
	void SetOwner(CObjectBase* owner);

	// 最大値を設定
	void SetMaxPoint(int point);
	// 現在値を設定
	void SetCurrPoint(int point);
	// ポイント残量の割合を設定（0.0～1.0）
	void SetPercent(float per);

	// 更新
	void Update() override;
	// 描画
	void Render() override;
	// サイズを設定
	void Size(float rate);

protected:
	// ポイント残量を反映
	void ApplyPoint();

	CObjectBase* mpOwner;	// HPゲージの持ち主

	CImage* mpGaugeImg;	// ゲージのイメージ
	CImage* mpWhiteImg;	// 白イメージ

	CVector2 mGaugeSize;	// ゲージのイメージのサイズ
	int mMaxPoint;			// 最大値
	int mCurrPoint;			// 現在値
	float mPercent;			// ポイント残量の割合
};