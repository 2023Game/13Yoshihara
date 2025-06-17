#include "CCastSpellStr.h"
#include "CTextUI2D.h"
#include <algorithm>

// �R���X�g���N�^
CCastSpellStr::CCastSpellStr(CObjectBase* owner, ECastType castType, 
	std::vector<std::string> quickStr, CVector textOffsetPos, int textSize)
	: CSpellCaster(owner)
	, mCastType(castType)
	, mQuickSpellStr(quickStr)
{
	// �����̃e�L�X�gUI�𐶐�
	mpSpellText = new CTextUI2D();
	// �����̑����̊��ݒ�
	mpSpellText->SetFontAligment(FTGL::TextAlignment::ALIGN_CENTER);
	// �����T�C�Y��ݒ�
	mpSpellText->SetFontSize(textSize);
	// ���W��ݒ�
	mpSpellText->Position(textOffsetPos);
}

// �f�X�g���N�^
CCastSpellStr::~CCastSpellStr()
{
}

// �r�������񂩂�������r��
void CCastSpellStr::CastSpell()
{
	// ����
	ESpellElementalType elemental = ESpellElementalType::eError;
	// �`
	ESpellShapeType shape = ESpellShapeType::eError;

	// ���������܂�����
	bool isElemental = false;

	for (int i = 0; i < mSpellStr.size(); i++)
	{
		// �܂����������܂��ĂȂ��Ȃ�
		if (!isElemental)
		{
			// ���������߂�
			elemental = ToElemental(mSpellStr[i]);
			// �G���[�Ȃ�`���m�F
			if (elemental == ESpellElementalType::eError)
			{
				// �`�����߂�
				shape = ToShape(mSpellStr[i]);

				// �G���[�Ȃ�
				if (shape == ESpellShapeType::eError)
				{
					// �r�����s
					break;
				}
				// �`�����܂����Ȃ�
				else
				{
					// ������
					elemental = ESpellElementalType::eNeutral;
					// �������r��
					CastStart(elemental, shape);
				}
			}
			// ����������
			else
			{
				// ���������܂���
				isElemental = true;
			}
		}
		// ���܂��Ă�����
		else
		{
			// �`�����߂�
			shape = ToShape(mSpellStr[i]);

			// �G���[�Ȃ�
			if (shape == ESpellShapeType::eError)
			{
				// �r�����s
				break;
			}
			// �`������
			else
			{
				// �������r��
				CastStart(elemental, shape);
				//���������܂��Ă��Ȃ�
				isElemental = false;
			}
		}
	}

	// ���Z�b�g
	mSpellStr.clear();
}

// �w�肵�������̕�������擾
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

// �������ǉ�
void CCastSpellStr::AddStr(std::string str)
{
	switch (mCastType)
	{
		// ��{�r��
	case CCastSpellStr::ECastType::eBasic:
	{
		// ��Ȃ珈�����Ȃ�
		if (str.empty()) return;

		// �v�f���Ȃ��Ȃ�
		if (mSpellStr.size() == 0)
		{
			// �V�����ǉ�
			mSpellStr.push_back(str);
			return;
		}
		// �X�y�[�X�Ȃ�
		if (str == " ")
		{
			// �V�����ǉ�
			mSpellStr.push_back("");
			return;
		}
		// �Ō�̗v�f
		int last = mSpellStr.size() - 1;
		std::string s = mSpellStr[last];
		// �Ō�̗v�f�̕�����Ɉ����̕������A��
		mSpellStr[last] = s + str;
		break;
	}

		// �Z�k�r��
	case CCastSpellStr::ECastType::eQuick:
		// ���̂܂ܒǉ�
		mSpellStr.push_back(str);
		break;
	}
}

// ��������폜
void CCastSpellStr::DeleteStr()
{
	switch (mCastType)
	{
		// ��{�r��
	case CCastSpellStr::ECastType::eBasic:
		// ���X�g����łȂ��Ȃ�
		if (!mSpellStr.empty())
		{
			int last = mSpellStr.size() - 1;
			// �Ō�̗v�f���󂶂�Ȃ��Ȃ�
			if (!mSpellStr[last].empty())
			{
				// �Ō�̗v�f���폜
				mSpellStr[last].pop_back();
			}
			// �����łȂ��ꍇ
			else
			{
				// �Ō�̗v�f���폜
				mSpellStr.pop_back();
			}
		}
		break;
	case CCastSpellStr::ECastType::eQuick:
		// ���X�g����łȂ��Ȃ�
		if (!mSpellStr.empty())
		{
			// �Ō�̗v�f���폜
			mSpellStr.pop_back();
		}
		break;
	}
}

// �X�V
void CCastSpellStr::Update()
{
	std::string spellStr = "";
	for (int i = 0; i < mSpellStr.size(); i++)
	{
		// �ŏ��ȊO�̓X�y�[�X��t����
		if (i != 0)spellStr += "   ";

		spellStr += mSpellStr[i];
	}
	// �`�悷�镶�����ݒ�
	mpSpellText->SetStr(spellStr.c_str());

	// �����r���N���X�̍X�V
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

// ��{�r��
void CCastSpellStr::BasicCastSpell(std::string str)
{
	// �������ǉ�
	AddStr(str);
}

// �Z�k�r��
void CCastSpellStr::QuickCastSpell(int num)
{
	// �T�C�Y�ȏ�Ȃ珈�����Ȃ�
	if (mQuickSpellStr.size() <= num) return;
	// �w�肳�ꂽ�v�f�ԍ��̎����̕������ǉ�
	AddStr(mQuickSpellStr[num]);
}

// �������ɕϊ�
std::string CCastSpellStr::ToLower(const std::string& str)
{
	// ���ʂɃR�s�[
	std::string result = str;
	// �擪���疖���܂ł̊e������tolower��K�p���Č��ʂɏ㏑��
	std::transform(result.begin(), result.end(), result.begin(),
		[](unsigned char c) {return std::tolower(c); });
	// ���ʂ�Ԃ�
	return result;
}

// �����񂩂瑮�������߂�
ESpellElementalType CCastSpellStr::ToElemental(const std::string& str)
{
	// �������ɕϊ�
	std::string s = ToLower(str);
	if (s == "fire")	return ESpellElementalType::eFire;		// ��
	if (s == "wind")	return ESpellElementalType::eWind;		// ��
	if (s == "earth")	return ESpellElementalType::eEarth;		// �n
	if (s == "thunder")	return ESpellElementalType::eThunder;	// ��
	if (s == "water")	return ESpellElementalType::eWater;		// ��

	if (s == "neutral")	return ESpellElementalType::eNeutral;	// ��
	// �G���[
	return ESpellElementalType::eError;
}

// �����񂩂�`�����߂�
ESpellShapeType CCastSpellStr::ToShape(const std::string& str)
{
	// �������ɕϊ�
	std::string s = ToLower(str);
	if (s == "ball")		return ESpellShapeType::eBall;
	if (s == "bolt")		return ESpellShapeType::eBolt;
	if (s == "breath")		return ESpellShapeType::eBreath;

	if (s == "teleport")	return ESpellShapeType::eTeleport;
	if (s == "shield")		return ESpellShapeType::eShield;
	if (s == "reflector")	return ESpellShapeType::eReflector;
	// �G���[
	return ESpellShapeType::eError;
}
