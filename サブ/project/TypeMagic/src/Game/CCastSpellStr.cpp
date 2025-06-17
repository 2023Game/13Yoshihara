#include "CCastSpellStr.h"
#include "CTextUI2D.h"
#include <algorithm>

// コンストラクタ
CCastSpellStr::CCastSpellStr(CObjectBase* owner, ECastType castType, 
	std::vector<std::string> quickStr, CVector textOffsetPos, int textSize)
	: CSpellCaster(owner)
	, mCastType(castType)
	, mQuickSpellStr(quickStr)
{
	// 呪文のテキストUIを生成
	mpSpellText = new CTextUI2D();
	// 文字の揃いの基準を設定
	mpSpellText->SetFontAligment(FTGL::TextAlignment::ALIGN_CENTER);
	// 文字サイズを設定
	mpSpellText->SetFontSize(textSize);
	// 座標を設定
	mpSpellText->Position(textOffsetPos);
}

// デストラクタ
CCastSpellStr::~CCastSpellStr()
{
}

// 詠唱文字列から呪文を詠唱
void CCastSpellStr::CastSpell()
{
	// 属性
	ESpellElementalType elemental = ESpellElementalType::eError;
	// 形
	ESpellShapeType shape = ESpellShapeType::eError;

	// 属性が決まったか
	bool isElemental = false;

	for (int i = 0; i < mSpellStr.size(); i++)
	{
		// まだ属性が決まってないなら
		if (!isElemental)
		{
			// 属性を求める
			elemental = ToElemental(mSpellStr[i]);
			// エラーなら形も確認
			if (elemental == ESpellElementalType::eError)
			{
				// 形を求める
				shape = ToShape(mSpellStr[i]);

				// エラーなら
				if (shape == ESpellShapeType::eError)
				{
					// 詠唱失敗
					break;
				}
				// 形が決まったなら
				else
				{
					// 無属性
					elemental = ESpellElementalType::eNeutral;
					// 呪文を詠唱
					CastStart(elemental, shape);
				}
			}
			// 属性が決定
			else
			{
				// 属性が決まった
				isElemental = true;
			}
		}
		// 決まっていたら
		else
		{
			// 形を求める
			shape = ToShape(mSpellStr[i]);

			// エラーなら
			if (shape == ESpellShapeType::eError)
			{
				// 詠唱失敗
				break;
			}
			// 形が決定
			else
			{
				// 呪文を詠唱
				CastStart(elemental, shape);
				//属性が決まっていない
				isElemental = false;
			}
		}
	}

	// リセット
	mSpellStr.clear();
}

// 指定した属性の文字列を取得
std::string CCastSpellStr::GetElementStr(ESpellElementalType elemental)
{
	switch (elemental)
	{
	case ESpellElementalType::eFire:
		return "fire";
		break;
	case ESpellElementalType::eWind:
		return "wind";
		break;
	case ESpellElementalType::eEarth:
		return "earth";
		break;
	case ESpellElementalType::eThunder:
		return "thunder";
		break;
	case ESpellElementalType::eWater:
		return "water";
		break;
	case ESpellElementalType::eNeutral:
		return "neutral";
		break;
	}
	return "error";
}

// 文字列を追加
void CCastSpellStr::AddStr(std::string str)
{
	switch (mCastType)
	{
		// 基本詠唱
	case CCastSpellStr::ECastType::eBasic:
	{
		// 空なら処理しない
		if (str.empty()) return;

		// 要素がないなら
		if (mSpellStr.size() == 0)
		{
			// 新しく追加
			mSpellStr.push_back(str);
			return;
		}
		// スペースなら
		if (str == " ")
		{
			// 新しく追加
			mSpellStr.push_back("");
			return;
		}
		// 最後の要素
		int last = mSpellStr.size() - 1;
		std::string s = mSpellStr[last];
		// 最後の要素の文字列に引数の文字列を連結
		mSpellStr[last] = s + str;
		break;
	}

		// 短縮詠唱
	case CCastSpellStr::ECastType::eQuick:
		// そのまま追加
		mSpellStr.push_back(str);
		break;
	}
}

// 文字列を削除
void CCastSpellStr::DeleteStr()
{
	switch (mCastType)
	{
		// 基本詠唱
	case CCastSpellStr::ECastType::eBasic:
		// リストが空でないなら
		if (!mSpellStr.empty())
		{
			int last = mSpellStr.size() - 1;
			// 最後の要素が空じゃないなら
			if (!mSpellStr[last].empty())
			{
				// 最後の要素を削除
				mSpellStr[last].pop_back();
			}
			// そうでない場合
			else
			{
				// 最後の要素を削除
				mSpellStr.pop_back();
			}
		}
		break;
	case CCastSpellStr::ECastType::eQuick:
		// リストが空でないなら
		if (!mSpellStr.empty())
		{
			// 最後の要素を削除
			mSpellStr.pop_back();
		}
		break;
	}
}

// 更新
void CCastSpellStr::Update()
{
	std::string spellStr = "";
	for (int i = 0; i < mSpellStr.size(); i++)
	{
		// 最初以外はスペースを付ける
		if (i != 0)spellStr += "   ";

		spellStr += mSpellStr[i];
	}
	// 描画する文字列を設定
	mpSpellText->SetStr(spellStr.c_str());

	// 呪文詠唱クラスの更新
	CSpellCaster::Update();
#if _DEBUG
	for (int i = 0; i < mSpellStr.size(); i++)
	{
		if (i == 0 &&
			mSpellStr[i].empty()) break;
		if (i == mSpellStr.size() - 1)
		{
			CDebugPrint::Print("%s\n", mSpellStr[i].c_str());
		}
		else
		{
			CDebugPrint::Print("%s ", mSpellStr[i].c_str());
		}
	}
#endif
}

// 基本詠唱
void CCastSpellStr::BasicCastSpell(std::string str)
{
	// 文字列を追加
	AddStr(str);
}

// 短縮詠唱
void CCastSpellStr::QuickCastSpell(int num)
{
	// サイズ以上なら処理しない
	if (mQuickSpellStr.size() <= num) return;
	// 指定された要素番号の呪文の文字列を追加
	AddStr(mQuickSpellStr[num]);
}

// 小文字に変換
std::string CCastSpellStr::ToLower(const std::string& str)
{
	// 結果にコピー
	std::string result = str;
	// 先頭から末尾までの各文字にtolowerを適用して結果に上書き
	std::transform(result.begin(), result.end(), result.begin(),
		[](unsigned char c) {return std::tolower(c); });
	// 結果を返す
	return result;
}

// 文字列から属性を求める
ESpellElementalType CCastSpellStr::ToElemental(const std::string& str)
{
	// 小文字に変換
	std::string s = ToLower(str);
	if (s == "fire")	return ESpellElementalType::eFire;		// 炎
	if (s == "wind")	return ESpellElementalType::eWind;		// 風
	if (s == "earth")	return ESpellElementalType::eEarth;		// 地
	if (s == "thunder")	return ESpellElementalType::eThunder;	// 雷
	if (s == "water")	return ESpellElementalType::eWater;		// 水

	if (s == "neutral")	return ESpellElementalType::eNeutral;	// 無
	// エラー
	return ESpellElementalType::eError;
}

// 文字列から形を求める
ESpellShapeType CCastSpellStr::ToShape(const std::string& str)
{
	// 小文字に変換
	std::string s = ToLower(str);
	if (s == "ball")		return ESpellShapeType::eBall;
	if (s == "bolt")		return ESpellShapeType::eBolt;
	if (s == "breath")		return ESpellShapeType::eBreath;

	if (s == "teleport")	return ESpellShapeType::eTeleport;
	if (s == "shield")		return ESpellShapeType::eShield;
	if (s == "reflector")	return ESpellShapeType::eReflector;
	// エラー
	return ESpellShapeType::eError;
}
