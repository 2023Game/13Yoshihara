#pragma once
#include "CObjectBase.h"

class CImage3D;

// 3D空間に配置するゲージクラス
class CGaugeUI3D : public CObjectBase
{
public:
	enum class EGaugeType
	{
		eHpGauge,	// HPゲージ
		eMpGauge,	// MPゲージ
	};

	// コンストラクタ
	CGaugeUI3D(CObjectBase* owner,std::string gaugePath, bool isRight, EGaugeType gaugeType);
	// デストラクタ
	virtual ~CGaugeUI3D();

	// 持ち主を設定
	void SetOwner(CObjectBase* owner);

	// 最大値を設定
	void SetMaxPoint(int point);
	// 現在値を設定
	void SetCurrPoint(int point);
	// ポイント残量の割合を設定（0.0〜1.0）
	void SetPercent(float per);

	// 更新
	void Update() override;
	// 描画
	void Render() override;

protected:
	// ポイント残量を反映
	void ApplyPoint();

	CObjectBase* mpOwner;	// HPゲージの持ち主

	CImage3D* mpGaugeImg;	// ゲージのイメージ
	CImage3D* mpWhiteImg;	// 白イメージ

	CVector2 mGaugeSize;	// ゲージのイメージのサイズ
	int mMaxPoint;			// 最大値
	int mCurrPoint;			// 現在値
	float mPercent;			// ポイント残量の割合

	bool mIsRight;	// 右から減っていく
	EGaugeType mGaugeType;	// ゲージのタイプ
};