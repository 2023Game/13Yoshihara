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
	ePlayer,		// �v���C���[
	eEnemy,			// �G
	eVehicle,		// �ԗ�
	eVehicleSearch,	// �ԗ��̒T�m�p
	eSpawnZone,		// �����ꏊ

	eAttackCol,		// �U������
};
// �Փ˔��背�C���[�̏��������X�g
using Layers = std::initializer_list<ELayer>;
