//�v���C���[�N���X�̃C���N���[�h
#include "CPlayer.h"

#define ROTATION_XV CVector(1.0f,0.0f,0.0f)//��]���x
#define ROTATION_YV CVector(0.0f,1.0f,0.0f)//��]���x
#define VELOCITY CVector(0.0f,0.0f,0.1f)//�ړ����x


//CPlayer(�ʒu,��],�X�P�[��)
CPlayer::CPlayer(const CVector& pos, const CVector& rot, const CVector& scale)
{
	CTransform::Update(pos, rot, scale);//�s��̍X�V
}

//�X�V����
void CPlayer::Update()
{
	//�X�y�[�X�L�[���͂Œe����
	if (mInput.Key(VK_SPACE))
	{
		bullet.Set(0.1f, 1.5f);
		bullet.Position(CVector(0.0f, 0.0f, 10.0f) * mMatrix);
		bullet.Rotation(mRotation);
	}         
	//S�L�[���͂ŉ�����
	if (mInput.Key('S'))
	{
		//X���̉�]�l�����Z
		mRotation = mRotation - ROTATION_XV;
	}
	//W�L�[���͂ŏ����
	if (mInput.Key('W'))
	{
		//X���̉�]�l�����Z
		mRotation = mRotation + ROTATION_XV;
	}
	//A�L�[���͂ŉ�]
	if (mInput.Key('A'))
	{
		//Y���̉�]�l�𑝉�
		mRotation = mRotation + ROTATION_YV;
	}

	//D�L�[���͂ŉ�]
	if (mInput.Key('D'))
	{
		//Y���̉�]�l������
		mRotation = mRotation - ROTATION_YV;
	}

	//��L�[���͂őO�i
	if (mInput.Key(VK_UP))
	{
		//Z�������̒l����]�����ړ�������
		mPosition = mPosition + VELOCITY * mMatrixRotate;
	}

	//�ϊ��s��̍X�V
	CTransform::Update();

}