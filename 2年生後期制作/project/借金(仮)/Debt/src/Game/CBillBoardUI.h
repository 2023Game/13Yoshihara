#pragma once
#include "CBillBoardImage.h"

class CBillBoardUI : public CBillBoardImage
{
public:
	// �R���X�g���N�^
	CBillBoardUI(std::string path, ETag tag, ETaskPauseType pause);
	// �f�X�g���N�^
	~CBillBoardUI();

	// �őO�ʂɃJ�����̕����ŕ`�悷��
	void Render(CMaterial* mpMaterial);
private:

};