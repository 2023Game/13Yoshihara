#pragma once
#include "CCharacter3.h"
#include "CTransform.h"
#include "CTriangle.h"
#include "CMatrix.h"
#include "CModel.h"
#include "CVector.h"
#include "CTexture.h"
#include "CBullet.h"
#include "CEnemy.h"
#include "CEnemy3.h"
#include "CPlayer.h"
#include "CInput.h"
#include "CFont.h"
#include <vector>
#include "CTaskManager.h"
#include "CCollisionManager.h"
#include "CBillBoard.h"
#include "CColliderTriangle.h"
#include "CColliderMesh.h"
#include "CUi.h"
#include "CModelX.h"
#include "CXCharacter.h"
#include "CXPlayer.h"
#include "CXEnemy.h"
#include "CPaladin.h"
#include "CActionCamera.h"

#include "CSound.h"

class CApplication
{
private:
	CMatrix mMatrix;
	CModelX mModelX;
	static CUi* spUi;//Ui�N���X�̃|�C���^
	//���f������R���C�_�𐶐�
	CColliderMesh mColliderMesh;
	//���f���r���[�̋t�s��
	static CMatrix mModelViewInverse;
	CModel mModelC5;//C5���f��
	CPlayer mPlayer;
	CModel mBackGround;//�w�i���f��
	CModel mModel; //���f���N���X�̃C���X�^���X�쐬
	CVector mEye; //�J�����̎��_
	CSound mSoundBgm;	//BGM
	CSound mSoundOver;	//�Q�[���I�[�o�[
	//static CTaskManager mTaskManager;
	enum class EState
	{
		ESTART,	//�Q�[���J�n
		EPLAY,	//�Q�[����
		ECLEAR,	//�Q�[���N���A
		EOVER,	//�Q�[���I�[�o�[
	};
	EState mState;
	CPlayer* mpPlayer;
	static CTexture mTexture;
	CEnemy* mpEnemy;
	CInput mInput;
	CFont mFont;

	//�L�����N�^�̃C���X�^���X
	CXPlayer mXPlayer;
	//�G�̃C���X�^���X
	CXEnemy mXEnemy;
	CModelX mKnight;
	CPaladin* mpPaladin;
	//CActionCamera
	CActionCamera mActionCamera;
public:
	~CApplication();
	static CUi* Ui();//UI�N���X�̃C���X�^���X���擾
	//���f���r���[�s��̎擾
	static const CMatrix& ModelViewInverse();
	static CTexture* Texture();
	//�����ݒ�
	void Init();
	//�ŏ��Ɉ�x�������s����v���O����
	void Start();
	//�J��Ԃ����s����v���O����
	void Update();
};