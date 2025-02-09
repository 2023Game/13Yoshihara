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
	eInteractObj,	// ���ׂ�I�u�W�F�N�g
	eInteractSearch,// ���ׂ�I�u�W�F�N�g�T�m�p
	eGoal,			// �S�[��
	eObstruction,	// ��Q��
	ePlayer,		// �v���C���[
	eEnemy,			// �G
	eCollector,		// �����
	eResident,		// �Z�l
	eTrashBag,		// �S�~��
	eTrashBagSearch,// �S�~�ܒT�m�p
	eCharaSearch,	// �L�����N�^�[�T�m�p
	eVehicle,		// �ԗ�
	eVehicleSearch,	// �ԗ��̒T�m�p
	eSpawnZone,		// �����ꏊ

	eAttackCol,		// �U������
};
// �Փ˔��背�C���[�̏��������X�g
using Layers = std::initializer_list<ELayer>;
