#pragma once
#include <initializer_list>

// �Փ˔��背�C���[
enum class ELayer
{
	eNone = -1,
	eTest,			// �e�X�g�p
	eGround,		// �n��
	eWall,			// ��
	eObject,		// �I�u�W�F�N�g
	eItem,			// �A�C�e��
	
	eInteractObj,	// ���ׂ�I�u�W�F�N�g
	eInteractSearch,// ���ׂ�I�u�W�F�N�g�T�m�p
	
	eTrashBag,		// �S�~��
	eTrashBagSearch,// �S�~�ܒT�m�p

	eObstruction,		// ��Q��

	eGoal,			// �S�[��
	
	ePlayer,		// �v���C���[
	eEnemy,			// �G
	eCollector,		// �����
	eResident,		// �Z�l
	
	eCharaSearch,	// �L�����N�^�[�T�m�p
	
	eVehicle,		// �ԗ�
	eVehicleSearch,	// �ԗ��̒T�m�p

	eSearch,		// �Փ˃��C���[�̃I�u�W�F�N�g�T�m�p

	eAttackCol,		// �U������
};
// �Փ˔��背�C���[�̏��������X�g
using Layers = std::initializer_list<ELayer>;
