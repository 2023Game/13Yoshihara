#pragma once
#include "CResultUIBase.h"

class CTextUI2D;

// �S�~�E���̃��U���gUI�N���X
class CTrashGameResultUI : public CResultUIBase
{
public:
	// �R���X�g���N�^
	CTrashGameResultUI();
	// �f�X�g���N�^
	~CTrashGameResultUI();

	// �X�V
	void Update() override;
	// �`��
	void Render() override;
private:
	// �S�~�܂̉摜
	CImage* mpTrashBagImg;
	// �S�[���h�̃S�~�܂̉摜
	CImage* mpGoldTrashBagImg;
};