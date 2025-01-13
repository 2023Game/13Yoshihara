#pragma once
#include <initializer_list>

// �Փ˔��背�C���[
enum class ELayer
{
	eNone = -1,
	eTest,
	eGround,
	eWall,
	eObject,
	eInteractObj,	// ���ׂ�I�u�W�F�N�g
	eInteractSearch,// ���ׂ�I�u�W�F�N�g�T�m�p
	ePlayer,
	eEnemy,
	eVehicle,
	eSpawnZone,

	eAttackCol,
};
// �Փ˔��背�C���[�̏��������X�g
using Layers = std::initializer_list<ELayer>;
