#pragma once
#include "CObjectBase.h"

class CModel;

class CSky : public CObjectBase
{
public:
	// �R���X�g���N�^
	CSky(CModel* model,
		const CVector& pos, const CVector& scale, const CVector& rotation);
	// �f�X�g���N�^
	~CSky();

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:
	// ��̃��f��
	CModel* mpModel;
};