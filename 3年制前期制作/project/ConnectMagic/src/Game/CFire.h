#pragma once
#include "CConnectObject.h"

class CFlamethrower;

// 炎のスケール
#define FIRE_SCALE 0.7f

// 炎の移動速度
#define MOVE_SPEED 2.0f

// 接続ターゲットのオフセット座標
#define TARGET_OFFSET_POS CVector(0.0f,2.0f,0.0f)

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
		float fireScale = FIRE_SCALE, float fireSpeed = MOVE_SPEED,
		CVector targetOffsetPos = TARGET_OFFSET_POS);
	// デストラクタ
	~CFire();

	// 繋がったときの処理
	void Connect(CVector wandPointPos, CVector targetPointPos) override;

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