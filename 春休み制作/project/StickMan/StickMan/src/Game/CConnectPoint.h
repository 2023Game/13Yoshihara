#pragma once
#include "CBillBoardImage.h"

// �ڑ������̃G�t�F�N�g
class CConnectPoint : public CBillBoardImage
{
public:
	// �R���X�g���N�^
	CConnectPoint();
	// �f�X�g���N�^
	~CConnectPoint();

	// �X�V
	void Update() override;

private:
};