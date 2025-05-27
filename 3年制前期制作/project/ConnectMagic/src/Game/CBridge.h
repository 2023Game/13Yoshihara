#pragma once
#include "CConnectObject.h"

// 橋
class CBridge : public CConnectObject
{
public:
	// コンストラクタ
	CBridge();
	// デストラクタ
	~CBridge();

	// 引っ張る処理
	void Pull(CVector pullDir, float opponentWeight) override;

	// 更新処理
	void Update() override;

private:
	// コライダーを生成
	void CreateCol() override;

	
	
	/// <summary>
	/// 角度分橋を傾ける
	/// </summary>
	/// <param name="angle">傾ける角度</param>
	/// <param name="dir">力の加わっている方向</param>
	void RotateBridge(float angle, CVector dir);

	// 現在の傾き
	float mCurrentAngle;
};