#pragma once
#include "CSwitchObject.h"

class CModel;

// スイッチで作用するドア
class CDoor : public CSwitchObject
{
public:
	// コンストラクタ
	CDoor(const CVector& rot);
	// デストラクタ
	~CDoor();

private:
	// コライダーを生成
	void CreateCol() override;

	// 作用していない時の処理
	void UpdateOff() override;
	// 作用している時の処理
	void UpdateOn() override;

	CVector mDefaultRotation;	// 初期の回転
};