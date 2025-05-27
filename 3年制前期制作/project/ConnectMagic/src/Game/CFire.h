#pragma once
#include "CConnectObject.h"

class CFlamethrower;

// 炎のスケール
#define FIRE_SCALE 0.7f

// 炎がつく接続オブジェクト
class CFire : public CConnectObject
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="modelName">モデルの名前</param>
	/// <param name="fireOffsetPos">炎のオフセット座標</param>
	/// <param name="fireScale">炎のスケール</param>
	CFire(std::string modelName = "", CVector fireOffsetPos = CVector::zero,
		float fireScale = FIRE_SCALE);
	// デストラクタ
	~CFire();

	// 繋がったときの処理
	void Connect(CConnectObject* other) override;

	// 炎がついているかを設定
	void SetFire(bool isEnable);
	// 炎がついているかを取得
	bool GetFire() const;

protected:
	// コライダーを生成
	void CreateCol(std::string modelName);

	// 炎のエフェクト生成用
	CFlamethrower* mpFlamethrower;

	// 炎がついているか
	bool mIsFire;
};