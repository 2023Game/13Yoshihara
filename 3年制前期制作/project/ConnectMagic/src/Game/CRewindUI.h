#pragma once

class CImage;
class CTextUI2D;

class CRewindUI : public CTask
{
public:
	// コンストラクタ
	CRewindUI();
	// デストラクタ
	~CRewindUI();

	// 更新
	void Update() override;
	// 描画
	void Render() override;

	// Dangerの文字の有効無効を設定
	void SetTextEnable(bool enable);

	// 画像の色を設定
	void SetImgColor(CColor col);

private:
	// 画面全体に色を付ける画像
	CImage* mpImg;
	// 「DANGER」の文字
	CTextUI2D* mpDangerText;
};