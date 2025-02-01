#pragma once
#include "CObjectBase.h"
#include "CText.h"

class CFont;

// 2D空間に配置するテキストクラス
class CTextUI2D : public CObjectBase
{
public:
	// コンストラクタ
	CTextUI2D(bool addTaskList = true);
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

	// 文字の揃いの基準を設定
	void SetFontAligment(FTGL::TextAlignment aligment);

private:
	CFont* mpFont;	// フォント
	CText* mpText;	// 文字列描画用

	std::string mStr;	// 表示する文字列
};