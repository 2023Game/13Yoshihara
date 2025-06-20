#pragma once
#include "CSpellCaster.h"

class CTextUI2D;

// テキストのオフセット座標
#define TEXT_UI_OFFSET_POS CVector(0.0f,WINDOW_HEIGHT * 0.1f, 0.0f)

// 文字列から詠唱呪文を指定するクラス
// 実際に呪文を発動する呪文詠唱クラスを継承
class CCastSpellStr : public CSpellCaster
{
public:
	// 文字列を追加
	void AddStr(std::string str);
	// 文字列を削除
	void DeleteStr();

	// 基本詠唱
	void BasicCastSpell(std::string str);
	// 短縮詠唱
	void QuickCastSpell(int num);

	// 詠唱文字列から呪文を詠唱
	void CastSpell();

	// 指定した属性の文字列を取得
	std::string GetElementStr(ESpellElementalType elemental);

protected:
	// 詠唱の種類
	enum class ECastType
	{
		eBasic,	// 基本詠唱
		eQuick,	// 短縮詠唱
	};
	ECastType mCastType;

	// コンストラクタ
	CCastSpellStr(CObjectBase* owner, ECastType castType,
		std::vector<std::string> quickStr = {}, CVector textOffsetPos = TEXT_UI_OFFSET_POS, int textSize = 64);
	// デストラクタ
	~CCastSpellStr();

	// 更新
	void Update();

	// 小文字に変換
	std::string ToLower(const std::string& str);
	// 文字列から属性を求める
	ESpellElementalType ToElemental(const std::string& str);
	// 文字列から形を求める
	ESpellShapeType ToShape(const std::string& str);

	// 呪文の文字列
	std::vector<std::string> mSpellStr;

	// 短縮詠唱で書かれる文字列のリスト
	std::vector<std::string> mQuickSpellStr;

	// テキスト
	CTextUI2D* mpSpellText;
};