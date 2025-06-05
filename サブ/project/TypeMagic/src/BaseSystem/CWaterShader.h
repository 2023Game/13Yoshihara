#pragma once
#include "CShader.h"

// ���ʐ�p�V�F�[�_�[�N���X
class CWaterShader : public CShader
{
private:
	GLint mTimeLocation;		// �o�ߎ���
	GLint mNormalMapLocation;	// �m�[�}���}�b�v�̃e�N�X�`�����j�b�g
	GLint mCameraPosLocation;	// �J�����̃��[���h���W
	GLint mModelLocation;		// ���f���ϊ��s��
	GLint mViewLocation;		// �r���[�ϊ��s��
	GLint mProjectionLocation;	// ���e�ϊ��s��

public:

	// �V�F�[�_�[�t�@�C����ǂݍ��݁A�v���O�������쐬���A
	// uniform�̃��P�[�V�������擾
	bool Load(const char* vertexPath, const char* flagPath);

	// �e��uniform�ɒl��ݒ肷��
	void SetUniforms(
		float time,
		int normalMapTextureUnit,
		const CVector& cameraPos,
		const CMatrix& model,
		const CMatrix& view,
		const CMatrix& projection);
};