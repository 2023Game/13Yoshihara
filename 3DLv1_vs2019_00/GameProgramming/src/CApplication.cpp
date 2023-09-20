#include "CApplication.h"
#include "CRectangle.h"
//OpenGL
#include "glut.h"

//�N���X��static�ϐ�
CTexture CApplication::mTexture;
CCharacterManager CApplication::mCharacterManager;

#define SOUND_BGM "res\\mario.wav" //BGM�����t�@�C��
#define SOUND_OVER "res\\mdai.wav" //�Q�[���I�[�o�[�����t�@�C��

CCharacterManager* CApplication::CharacterManager()
{
	return &mCharacterManager;
}

CTexture* CApplication::Texture()
{
	return &mTexture;
}

void CApplication::Start()
{

}

void CApplication::Update()
{
	//���_�̐ݒ�
	//gluLookAt(���_X, ���_Y, ���_z, ���SX, ���SY, ���Sz, ���X, ���Y, ���Z)
	gluLookAt(1.0f, 2.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	//�`��J�n
	//glBegin(�`)
	//GL_TRIANGLES:�O�p�`
	glBegin(GL_TRIANGLES);

	//�@��(�ʂ̌���)�̐ݒ�
	//glNormal3f(X���W, Y���W, Z���W)
	glNormal3f(0.0f, 1.0f, 0.0f);

	//���_���W�̐ݒ�
	//glVertex3f(X���W, Y���W, Z���W)
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, -0.5f);

	//�ʂ̌�����Z������
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-0.5f, 0.0f, 0.0f);

	//�ʂ̌�����X������
	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, -0.5f, 0.0f);

	//�`��I��
	glEnd();
}
