#pragma once
#include "CObjectBase.h"

class CTextUI2D;
class CImage;

// �����̕ԍϊz��\������
class CDebtMoneyUI : public CObjectBase
{
public:
	// �R���X�g���N�^
	CDebtMoneyUI();
	// �f�X�g���N�^
	~CDebtMoneyUI();

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:
	// �ҋ@
	void UpdateIdle();
	// �������̌���
	void UpdateDecrease();
	// �t�F�[�h�A�E�g
	void UpdateFadeOut();

	// ���
	enum class EState
	{
		eIdle,			// �ҋ@
		eDecrease,		// �������̌���
		eFadeOut,		// �t�F�[�h�A�E�g
	};
	// ��Ԑ؂�ւ�
	void ChangeState(EState state);
	EState mState;	// �v���C���[�̏��
	int mStateStep;				// ��ԓ��̃X�e�b�v�Ǘ��p
	// �o�ߎ���
	float mElapsedTime;

	// �w�i
	CImage* mpBackGround;

	// ����
	CTextUI2D* mpDay;
	// �ԍϊz
	CTextUI2D* mpDebtMoney;
	// ������
	CTextUI2D* mpMoney;

	// �ԍϊz�̒l
	int mDebtMoneyAmount;
	// �������̒l
	int mMoneyAmount;
	// �t�H���g�̃T�C�Y
	int mFontSize;

	// ���������l
	int mDecreaseAmount;
};