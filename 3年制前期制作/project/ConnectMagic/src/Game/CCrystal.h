#pragma once
#include "CConnectObject.h"

class CSwitch;
class CModel;

// クリスタル
class CCrystal : public CConnectObject
{
public:
	// コンストラクタ
	CCrystal(CVector pos, CSwitch* owner);
	// デストラクタ
	~CCrystal();

	// 更新
	void Update() override;
	// 描画
	void Render() override;

	// 繋がった時の処理
	void Connect(CConnectPoint* otherPoint, bool isWand) override;

	// オンかオフかを設定
	void SetOnOff(bool enable);
	// オンかオフかを取得
	bool GetOnOff();

private:
	// コライダーを生成
	void CreateCol();

	// 当たり判定
	CCollider* mpCol;

	CSwitch* mpOwner;	// 持ち主

	CModel* mpModelOn;
	CModel* mpModelOff;
};