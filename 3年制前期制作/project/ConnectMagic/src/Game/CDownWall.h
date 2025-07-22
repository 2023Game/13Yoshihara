#pragma once
#include "CSwitchObject.h"

class CSwitch;

// 下がっていく壁
class CDownWall : public CSwitchObject
{
public:
	// コンストラクタ
	CDownWall(CVector defaultPos, std::vector<CSwitch*> switchs);
	// デストラクタ
	~CDownWall();

	// オンオフを切り替える
	void SetOnOff(bool isOnOff) override;

private:
	// コライダーを生成
	void CreateCol() override;

	// 作用していない時の処理
	void UpdateOff() override;
	// 作用している時の処理
	void UpdateOn() override;

	// デフォルトのY座標
	float mDefaultPosY;

	// 作用するスイッチ
	std::vector<CSwitch*> mSwitchs;
	// オンのスイッチの数
	int mOnSwitchNum;
};