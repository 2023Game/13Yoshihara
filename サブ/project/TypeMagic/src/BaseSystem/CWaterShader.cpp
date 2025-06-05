#include  "glew.h"
#include "CWaterShader.h"

// �V�F�[�_�[�t�@�C����ǂݍ��݁A�v���O�������쐬���A
// uniform�̃��P�[�V�������擾
bool CWaterShader::Load(const char* vertexPath, const char* flagPath)
{
	// ���N���X��Load�ŃV�F�[�_�[�v���O�������쐬
	bool result = CShader::Load(vertexPath, flagPath);
	if (!result) return false;

	// �V�F�[�_�[����uniform�ϐ��̏ꏊ���擾
	mTimeLocation = glGetUniformLocation(GetProgram(), "u_Time");
	mNormalMapLocation = glGetUniformLocation(GetProgram(), "u_NormalMap");
	mCameraPosLocation = glGetUniformLocation(GetProgram(), "u_CameraPos");
	mModelLocation = glGetUniformLocation(GetProgram(), "u_Model");
	mViewLocation = glGetUniformLocation(GetProgram(), "u_View");
	mProjectionLocation = glGetUniformLocation(GetProgram(), "u_Projection");

	return true;
}

// �e��uniform�ɒl��ݒ肷��
void CWaterShader::SetUniforms(
	float time,
	int normalMapTextureUnit,
	const CVector& cameraPos,
	const CMatrix& model,
	const CMatrix& view,
	const CMatrix& projection)
{
	// �o�ߎ��Ԃ�ݒ�
	glUniform1f(mTimeLocation, time);

	// �m�[�}���}�b�v�p�̃e�N�X�`�����j�b�g��ݒ�
	glUniform1i(mNormalMapLocation, normalMapTextureUnit);

	// �J�����̃��[���h���W��ݒ�
	glUniform3fv(mCameraPosLocation, 1, (float*)&cameraPos);

	// ���f���ϊ��s���ݒ�
	glUniformMatrix4fv(mModelLocation, 1, GL_FALSE, model.M());

	// �r���[�ϊ��s���ݒ�
	glUniformMatrix4fv(mViewLocation, 1, GL_FALSE, view.M());

	// ���e�ϊ��s��
	glUniformMatrix4fv(mProjectionLocation, 1, GL_FALSE, projection.M());
}
