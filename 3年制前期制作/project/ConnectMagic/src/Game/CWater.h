#pragma once
#include "CObjectBase.h"
#include "CWaterShader.h"

class CTexture;
class CMaterial;

// ��
class CWater : public CObjectBase
{
private:
	GLuint mVAO;
	GLuint mVBO;
	GLuint mNormalMapTex;
	CWaterShader mWaterShader;	// ���ʗp�V�F�[�_�[
	float mTime;				// �o�ߎ���

	CMaterial* mpMaterial;

public:
	// �R���X�g���N�^
	CWater();
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