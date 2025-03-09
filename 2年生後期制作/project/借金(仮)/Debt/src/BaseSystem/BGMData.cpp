#include "BGMData.h"

// BGM�f�[�^�̃e�[�u��
const BGMData BGM_DATA[] =
{
	//BGM�̎��					���\�[�X�p�X							�x�[�X����		���[�v	���[�v�J�n	���[�v�I��
	{ EBGMType::eTitle,			"Sound\\BGM\\title.wav",				0.0625f,		true,	0,			0		},
	{ EBGMType::eClear,			"Sound\\BGM\\game_clear.wav",			0.0625f,		false,	0,			0		},
	{ EBGMType::eGameOver,		"Sound\\BGM\\game_over.wav",			0.0625f,		true,	0,			0		},
	{ EBGMType::eResultSuccess,	"Sound\\BGM\\game_result_success.wav",	0.0625f,		false,	0,			0		},
	{ EBGMType::eResultFailed,	"Sound\\BGM\\game_result_failed.wav",	0.0625f,		false,	0,			0		},
	{ EBGMType::eHome,			"Sound\\BGM\\game.wav",					0.0625f,		true,	0,			2801203	},
	{ EBGMType::eTrashGame,		"Sound\\BGM\\streetCleaner.wav",		0.0625f,		true,	0,			0		},
	{ EBGMType::eMenu,			"Sound\\BGM\\menu.wav",					0.0625f,		true,	0,			0		},
};

// BGM�f�[�^��
const int BGM_DATA_COUNT = ARRAY_SIZE(BGM_DATA);