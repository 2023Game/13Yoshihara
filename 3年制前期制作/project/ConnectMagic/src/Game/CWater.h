#pragma once
#include "CObjectBase.h"
#include "CWaterShader.h"

class CMaterial;

// ��
class CWater : public CObjectBase
{
private:
	GLuint mVAO;				// �ݒ���
	GLuint mVBO;				// ���_���
	GLuint mNormalMapTex;		// �m�[�}���}�b�v�̃e�N�X�`��ID
	CWaterShader mWaterShader;	// ���ʗp�V�F�[�_�[
	float mTime;				// �o�ߎ���

	// �}�e���A��
	CMaterial* mpMaterial;

	// �R���C�_�[
	CCollider* mpCol;

	// �R���C�_�[�𐶐�
	void CreateCol() override;

public:
	// �R���X�g���N�^
	CWater(CVector scale);
	// �f�X�g���N�^
	~CWater();

	// �����ݒ�
	bool Init(const char* vertexPath,
		const char* flagPath);

	// �X�V
	void Update() override;
	// �`��
	void Render() override;
};