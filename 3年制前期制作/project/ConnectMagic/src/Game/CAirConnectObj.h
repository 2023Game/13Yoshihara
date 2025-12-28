#pragma once
#include "CConnectObject.h"

// 空中に浮かんでいる接続オブジェクト
class CAirConnectObj : public CConnectObject
{
public:
	// コンストラクタ
	CAirConnectObj(const CVector& pos);
	// デストラクタ
	~CAirConnectObj();

	// つながった時の処理
	void Connect(CConnectPoint* otherPoint, bool isWand) override;

private:
	// コライダーを生成
	void CreateCol() override;
};