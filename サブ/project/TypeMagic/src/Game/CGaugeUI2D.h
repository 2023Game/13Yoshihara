#pragma once
#include "CObjectBase.h"

class CImage;

// 2D空間に配置するイメージクラス
class CGaugeUI2D : public CObjectBase
{
public:
	enum class EGaugeType
	{
		eHpGauge,	// HPゲージ
		eMpGauge,	// MPゲージ
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">持ち主</param>
	/// <param name="gaugePath">ゲージの画像のパス</param>
	/// <param name="isRight">右からゲージの減少が始まるか</param>
	/// <param name="gaugeType">ゲージのタイプ</param>
	CGaugeUI2D(CObjectBase* owner, std::string gaugePath, bool isRight, EGaugeType gaugeType);
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
	// サイズを取得
	CVector Size() const;

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

	bool mIsRight;	// 右から減っていく
	EGaugeType mGaugeType;	// ゲージのタイプ
};