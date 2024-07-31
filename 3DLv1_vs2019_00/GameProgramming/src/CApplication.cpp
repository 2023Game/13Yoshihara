#include "CApplication.h"
#include "CCamera.h"
//OpenGL
#include "glut.h"


//�N���X��static�ϐ�
CTexture CApplication::mTexture;
CMatrix CApplication::mModelViewInverse;

//�w�i���f���f�[�^�̎w��
#define MODEL_BACKGROUND "res\\sky.obj","res\\sky.mtl"
#define MODEL_OBJ "res\\f14.obj","res\\f14.mtl" //���f���f�[�^�̎w��
#define MODEL_C5 "res\\c5.obj","res\\c5.mtl"//�G�A���@���f��
#define SOUND_BGM "res\\mario.wav" //BGM�����t�@�C��
#define SOUND_OVER "res\\mdai.wav" //�Q�[���I�[�o�[�����t�@�C��

CUi* CApplication::spUi = nullptr;

CUi* CApplication::Ui()
{
	return spUi;//�C���X�^���X�̃|�C���^��Ԃ�
}

CApplication::~CApplication()
{
	delete spUi;//�C���X�^���XUi�̍폜
}

const CMatrix& CApplication::ModelViewInverse()
{
	return mModelViewInverse;
}

CTexture* CApplication::Texture()
{
	return &mTexture;
}

void CApplication::Start()
{
	//�J�����̐ݒ�
	mActionCamera.Set(5.0f, -15.0f, 180.0f);
	//3D���f���t�@�C���̓ǂݍ���
	mModelX.Load(MODEL_FILE);
	mKnight.Load(KNIGHT_MODEL);
	//�A�j���[�V�����Z�b�g�̕���
	mKnight.SeparateAnimationSet(0, 10, 80, "walk");//1:�ړ�
	mKnight.SeparateAnimationSet(0, 1530, 1830, "idle1");//2:�ҋ@
	mKnight.SeparateAnimationSet(0, 10, 80, "walk");//3:�_�~�[
	mKnight.SeparateAnimationSet(0, 10, 80, "walk");//4:�_�~�[
	mKnight.SeparateAnimationSet(0, 10, 80, "walk");//5:�_�~�[
	mKnight.SeparateAnimationSet(0, 10, 80, "walk");//6:�_�~�[
	mKnight.SeparateAnimationSet(0, 440, 520, "attack1");//7:Attack1
	mKnight.SeparateAnimationSet(0, 520, 615, "attack2");//8:Attack2
	mKnight.SeparateAnimationSet(0, 10, 80, "walk");//9:�_�~�[
	mKnight.SeparateAnimationSet(0, 10, 80, "walk");//10:�_�~�[
	mKnight.SeparateAnimationSet(0, 1160, 1260, "death1");//11:�_�E��
	//�L�����N�^�[�Ƀ��f����ݒ�
	mXPlayer.Init(&mModelX);
	//�G�̏����ݒ�
	mXEnemy.Init(&mKnight);
	//�G�̔z�u
	mXEnemy.Position(CVector(7.0f, 0.0f, 0.0f));
	//�G�̃A�j���[�V������ҋ@�ɕύX
	mXEnemy.ChangeAnimation(2, true, 200);
	//CPaladin�̍쐬
	mpPaladin = new CPaladin();
	mpPaladin->Position(CVector(-1.0f, 0.0, 5.0f));
	mpPaladin->ChangeAnimation(1, true, 100);
	mFont.Load("FontG.png", 1, 4096 / 64);
}

void CApplication::Update()
{	
	//�L�����N�^�N���X�̍X�V
	mXPlayer.Update();
	//�G�̍X�V
	mXEnemy.Update();
	//�p���f�B���̍X�V
	mpPaladin->Update();

	//�J�����ݒ�
	mActionCamera.Position(mXPlayer.Position()
		+ CVector(0.0f, 2.0f, 0.0f));
	mActionCamera.Update();
	mActionCamera.Render();

	//���f���r���[�s��̎擾
	glGetFloatv(GL_MODELVIEW_MATRIX, mModelViewInverse.M());
	//�t�s��̎擾
	mModelViewInverse = mModelViewInverse.Transpose();
	mModelViewInverse.M(0, 3, 0);
	mModelViewInverse.M(1, 3, 0);
	mModelViewInverse.M(2, 3, 0);

	//���f���`��
	mXPlayer.Render();
	//�G�`��
	mXEnemy.Render();
	//�p���f�B���̍X�V
	mpPaladin->Render();

	//�R���C�_�̕`��
	CCollisionManager::Instance()->Render();
	//�Փˏ���
	CCollisionManager::Instance()->Collision();

	//2D�`��J�n
	CCamera::Start(0, 800, 0, 600);

	mFont.Draw(20, 20, 10, 12, "3D PROGRAMING");

	CVector screen;
	//Enemy�̍��W���X�N���[�����W�֕ϊ�
	if (CActionCamera::Instance()->WorldToScreen(&screen, mXEnemy.Position()))
	{
		//�ϊ���̍��W�ɕ�������o�͂���
		mFont.Draw(screen.X(), screen.Y(), 7, 14, "ENEMY");
	}

	//2D�̕`��I��
	CCamera::End();
}
