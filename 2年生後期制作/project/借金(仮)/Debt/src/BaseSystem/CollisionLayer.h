#pragma once
#include <initializer_list>

// �Փ˔��背�C���[
enum class ELayer
{
	eNone = -1,
	eTest,
	eField,
	eWall,
	eObject,
	eInteract,
	ePlayer,
	eEnemy,
	eVehicle,
	eSpawnZone,

	eAttackCol,
};
// �Փ˔��背�C���[�̏��������X�g
using Layers = std::initializer_list<ELayer>;
