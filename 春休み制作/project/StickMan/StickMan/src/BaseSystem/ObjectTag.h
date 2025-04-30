#pragma once
#include <initializer_list>

// �I�u�W�F�N�g���ʗp�̃^�O
enum class ETag
{
	eNone = -1,

	eField,	// �t�B�[���h���\������I�u�W�F�N�g
	eRideableObject,	// ��邱�Ƃ��ł���I�u�W�F�N�g
	eInteractObject,	// ���ׂ�I�u�W�F�N�g
	eConnectObject,		// �q�����I�u�W�F�N�g
	eItem,				// �A�C�e���̃I�u�W�F�N�g
	eObstruction,		// ��Q��
	eSwitch,			// �X�C�b�`
	eSwitchObject,		// �X�C�b�`�ō�p����I�u�W�F�N�g

	ePlayer,	// �v���C���[
	eEnemy,		// �G�l�~�[

	eBullet,// �e��
	eFlame,	// ��
	eSlash,	// �a��
	eWand,	// ��
	eConnectPoint,	// �ڑ���

	eUI,
	eInteractUI, //�C���^���N�gUI

	eCamera,// �J����

	eDebug,	// �f�o�b�O�p�̃I�u�W�F�N�g

	Num
};
// �I�u�W�F�N�g���ʗp�̃^�O�̏��������X�g
using Tags = std::initializer_list<ETag>;
