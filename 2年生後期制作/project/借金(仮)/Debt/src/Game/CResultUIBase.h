#pragma once
#include "CTask.h"

class CFont;
class CImage;
class CExpandButton;
class CTextUI2D;

class CResultUIBase : public CTask
{
public:
	// �R���X�g���N�^
	CResultUIBase(std::string backGroundPath);
	// �f�X�g���N�^
	~CResultUIBase();

	// ���ʉ�ʏI����
	bool IsEnd() const;
	// ���_�֖߂邩
	bool IsReturnHome() const;

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

protected:
	// ���j���[���J��
	void UpdateOpen();
	// ���j���[�I��
	void UpdateSelect();
	// �t�F�[�h�A�E�g
	void UpdateFadeOut();

	// �^�C�g���̏��
	enum class EState
	{
		eOpen,		// ���j���[���J��
		eSelect,	// ���j���[�I��
		eFadeOut,	// �t�F�[�h�A�E�g
	};
	// ��Ԑ؂�ւ�
	void ChangeState(EState state);

	// [���_��]�N���b�N���̃R�[���o�b�N�֐�
	virtual void OnClickReturn();


	EState mState;		// ���݂̏��
	int mStateStep;		// ��ԓ��ł̃X�e�b�v�Ǘ��p
	float mElapsedTime;	// �o�ߎ��Ԍv���p
	bool mIsEnd;		// �^�C�g����ʏI���t���O
	int mSelectIndex;	// ���ݑI�����Ă��鍀��

	// �X�R�A�Ƃ��������̃e�L�X�g
	CTextUI2D* mpScoreText;
	// ���_�̃e�L�X�g
	CTextUI2D* mpScorePointText;
	// ���l�̃e�L�X�g1
	CTextUI2D* mpNumText1;
	// ���l�̃e�L�X�g2
	CTextUI2D* mpNumText2;
	// �v�Z���ʂ̃e�L�X�g
	CTextUI2D* mpTotalText;
	// �P�ʂ̃e�L�X�g
	CTextUI2D* mpUnitText;
	// �L���̃e�L�X�g
	CTextUI2D* mpSymbolText;
	// ��ԉ��̌v�Z�̐����e�L�X�g
	CTextUI2D* mpDescriptionText;
	// �{�[�i�X�̐����e�L�X�g
	CTextUI2D* mpBonusDescriptionText;
	// ���U���g�w�i�C���[�W
	CImage* mpResultBg;
	// ���_�֖߂�{�^��
	CExpandButton* mpReturnButton;
};