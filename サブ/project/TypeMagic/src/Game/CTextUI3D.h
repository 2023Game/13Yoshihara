#pragma once
#include "CTextUI2D.h"
#include "CTextUI2D.h"

class CFont;

// 3D��Ԃɔz�u����e�L�X�g�N���X
class CTextUI3D : public CTextUI2D
{
public:
	// �R���X�g���N�^
	CTextUI3D(CVector pos,
		ETaskPauseType pauseType = ETaskPauseType::eGame,
		bool addTaskList = true,
		char* fontPath = "res\\Font\\toroman.ttf");
	// �f�X�g���N�^
	~CTextUI3D();

	// �X�V
	void Update() override;

	// ���[���h���W����X�N���[�����W�����߂č��W��ݒ�
	void Position(const CVector& pos) override;

	// ���[���h���W��ݒ�
	void SetWorldPos(const CVector& pos);

protected:
	// ���[���h���W
	CVector mWorldPos;
};