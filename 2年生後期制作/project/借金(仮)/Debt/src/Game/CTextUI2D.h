#pragma once
#include "CObjectBase.h"
#include "CText.h"

class CFont;

// 2D空間に配置するテキストクラス
class CTextUI2D : public CObjectBase
{
public:
	// コンストラクタ
	CTextUI2D(ETaskPauseType pauseType = ETaskPauseType::eGame,
		bool addTaskList = true,
		char * fontPath = "res\\Font\\toroman.ttf");
	// デストラクタ
	~CTextUI2D();

	// 更新
	void Update();
	// 描画
	void Render();

	/// <summary>
	/// 描画する文字列を変換して設定する
	/// </summary>
	/// <param name="format">書式文字列</param>
	/// <param name="第2引数以降">文字列に変換する引数</param>
	void ChangeToStr(const char* format, ...);

	// 描画する文字列を設定する
	void SetStr(std::string str);

	// 文字のサイズを設定する
	void SetFontSize(int fontSize);
	// 文字の色を設定する
	void SetFontColor(CColor color);
	// アウトラインの色を設定する
	void SetOutLineColor(CColor color);

	// 文字の揃いの基準を設定
	void SetFontAligment(FTGL::TextAlignment aligment);

	// 影を設定
	void SetShadow(bool enable, CColor color = CColor(0.0f, 0.0f, 0.0f));
	// アウトラインを設定
	void SetOutline(bool enable, CColor color = CColor(0.0f, 0.0f, 0.0f));

	// 点滅するかを設定
	void SetBlink(bool enable);

	// 点滅する処理
	void Blink();

private:
	CFont* mpFont;	// フォント
	CText* mpText;	// 文字列描画用

	std::string mStr;	// 表示する文字列

	// 点滅するか
	bool mIsBlink;

	int mStateStep;		// 状態内でのステップ管理用
	float mElapsedTime;	// 経過時間計測用
};