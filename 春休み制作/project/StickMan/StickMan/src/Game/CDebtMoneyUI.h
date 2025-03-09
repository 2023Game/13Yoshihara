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
	// �Q�[���I�[�o�[
	void UpdateGameOver();

	// ���
	enum class EState
	{
		eIdle,			// �ҋ@
		eDecrease,		// �������̌���
		eFadeOut,		// �t�F�[�h�A�E�g
		eGameOver,		// �Q�[���I�[�o�[
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
	CTextUI2D* mpDayText;
	// �ԍϊz
	CTextUI2D* mpDebtMoneyText;
	// ������
	CTextUI2D* mpMoneyText;
	// �u�N���b�N�Ŏ��ցv�̃e�L�X�g
	CTextUI2D* mpNextText;

	// �����������炷
	void DecreaseMoney();

	// �ԍϊz�̒l
	int mDebtMoneyAmount;
	// �������̒l
	int mMoneyAmount;
	// �t�H���g�̃T�C�Y
	int mFontSize;

	// ����
	int mResultAmount;
};