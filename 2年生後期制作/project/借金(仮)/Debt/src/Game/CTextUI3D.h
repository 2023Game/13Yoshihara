#pragma once
#include "CTextUI2D.h"
#include "CText.h"

class CFont;

// 3D��Ԃɔz�u����e�L�X�g�N���X
class CTextUI3D : public CTextUI2D
{
public:
	// �R���X�g���N�^
	CTextUI3D(ETaskPauseType pauseType = ETaskPauseType::eGame,
		bool addTaskList = true,
		char* fontPath = "res\\Font\\toroman.ttf");
	// �f�X�g���N�^
	~CTextUI3D();

	// �X�V
	void Update();
	// �`��
	void Render();

	// 2D��Ԃł̃I�t�Z�b�g���W���擾
	const CVector2& GetOffsetPos() const;
	// 2D��Ԃł̃I�t�Z�b�g���W��ݒ�
	void SetOffsetPos(const float& x, const float& y);
	// 2D��Ԃł̃I�t�Z�b�g���W��ݒ�iCVector2�Łj
	void SetOffsetPos(const CVector2& pos);

private:
	// 2D��Ԃł̃I�t�Z�b�g���W
	CVector2 mOffsetPos;
};