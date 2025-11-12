#pragma once
#include "CConnectObject.h"

// 橋
class CBridge : public CConnectObject
{
public:
	// コンストラクタ
	CBridge(float defaultAngle = 0.0f);
	// デストラクタ
	~CBridge();

	// 更新処理
	void Update() override;

	// 繋がったときの処理
	void Connect(CConnectPoint* otherPoint, bool isWand) override;

	// 指定角度に設定
	void SetAngle(float angle);

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