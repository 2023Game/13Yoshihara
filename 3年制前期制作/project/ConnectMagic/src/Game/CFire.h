#pragma once
#include "CConnectObject.h"

class CFlamethrower;

// 炎のスケール
constexpr float DEFAULT_FIRE_SCALE =		0.7f;

// 炎の移動速度
constexpr float DEFAULT_FIRE_SPEED =		2.0f;

// 接続ターゲットのオフセット座標
const CVector DEFAULT_TARGET_OFFSET_POS =	CVector(0.0f, 2.0f, 0.0f);

// 炎がつく接続オブジェクト
class CFire : public CConnectObject
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="fireOffsetPos">炎のオフセット座標</param>
	/// <param name="fireScale">炎のスケール</param>
	/// <param name="fireSpeed">炎の移動速度</param>
	/// <param name="targetOffsetPos">接続ターゲットのオフセット座標</param>
	CFire(CVector fireOffsetPos = CVector::zero,
		float fireScale = DEFAULT_FIRE_SCALE, float fireSpeed = DEFAULT_FIRE_SPEED,
		CVector targetOffsetPos = DEFAULT_TARGET_OFFSET_POS);
	// デストラクタ
	~CFire();

	// 繋がったときの処理
	void Connect(CConnectPoint* otherPoint, bool isWand) override;

	// 炎がついているかを設定
	void SetFire(bool isEnable);
	// 炎がついているかを取得
	bool GetFire() const;

protected:
	// コライダーを生成
	void CreateCol() override;

	// 燃えた時の処理
	virtual void Burning();

	// 炎のエフェクト生成用
	CFlamethrower* mpFlamethrower;

	// 炎がついているか
	bool mIsFire;
};