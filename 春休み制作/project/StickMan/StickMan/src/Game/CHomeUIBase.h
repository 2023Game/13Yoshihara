#pragma once
#include "CTask.h"

class CTextUI2D;
class CImage;

// 所持金表示UI
class CHomeUIBase : public CTask
{
public:
	// コンストラクタ
	CHomeUIBase();
	// デストラクタ
	~CHomeUIBase();

	// 更新
	void Update() override;
	// 描画
	void Render() override;

	// 所持金の量によって背景を設定
	void SetBgPath();

protected:
	// テキストUI
	CTextUI2D* mpTextUI;
	// テキストの背景
	CImage* mpBackground;
	// 背景のパス
	std::string mBgPath;
};