#pragma once
#include "CSpellCaster.h"

class CTextUI2D;

// �e�L�X�g�̃I�t�Z�b�g���W
#define TEXT_UI_OFFSET_POS CVector(0.0f,WINDOW_HEIGHT * 0.1f, 0.0f)

// �����񂩂�r���������w�肷��N���X
// ���ۂɎ����𔭓���������r���N���X���p��
class CCastSpellStr : public CSpellCaster
{
public:
	// �������ǉ�
	void AddStr(std::string str);
	// ��������폜
	void DeleteStr();

	// ��{�r��
	void BasicCastSpell(std::string str);
	// �Z�k�r��
	void QuickCastSpell(int num);

	// �r�������񂩂�������r��
	void CastSpell();

	// �w�肵�������̕�������擾
	std::string GetElementStr(ESpellElementalType elemental);

protected:
	// �r���̎��
	enum class ECastType
	{
		eBasic,	// ��{�r��
		eQuick,	// �Z�k�r��
	};
	ECastType mCastType;

	// �R���X�g���N�^
	CCastSpellStr(CObjectBase* owner, ECastType castType,
		std::vector<std::string> quickStr = {}, CVector textOffsetPos = TEXT_UI_OFFSET_POS, int textSize = 64);
	// �f�X�g���N�^
	~CCastSpellStr();

	// �X�V
	void Update();

	// �������ɕϊ�
	std::string ToLower(const std::string& str);
	// �����񂩂瑮�������߂�
	ESpellElementalType ToElemental(const std::string& str);
	// �����񂩂�`�����߂�
	ESpellShapeType ToShape(const std::string& str);

	// �����̕�����
	std::vector<std::string> mSpellStr;

	// �Z�k�r���ŏ�����镶����̃��X�g
	std::vector<std::string> mQuickSpellStr;

	// �e�L�X�g
	CTextUI2D* mpSpellText;
};