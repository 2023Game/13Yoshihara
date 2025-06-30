#pragma once

#define ELEMENTAL_POWER_RATIO	2.0f	// ��_�����̔{��

// �v���C���[�̐F
#define PLAYER_COLOR	CColor(0.15f,0.2f,0.6f)
// �G�̐F
#define ENEMY_COLOR		CColor(0.6f,0.1f,0.1f)

// ���̐F
#define FIRE_COLOR		CColor(1.0f,0.3f,0.1f)
// ���̐F
#define WIND_COLOR		CColor(0.6f,0.9f,0.6f)
// �n�̐F
#define EARTH_COLOR		CColor(0.647f, 0.165f, 0.165f)
// ���̐F
#define THUNDER_COLOR	CColor(0.9f,0.75f,0.2f)
// ���̐F
#define WATER_COLOR		CColor(0.4f,0.6f,1.0f)
// ���̐F
#define NEUTRAL_COLOR	CColor(0.9f,0.9f,0.9f)

// ��
#define FIRE_POWER		500		// �U����
#define FIRE_SPEED		10.0f		// ���x
#define FIRE_HP			0			// �ϋv
#define FIRE_IDLE_TIME	0.1f		// �ҋ@����
#define FIRE_GENERATE_INTERVAL 0.1f	// �����Ԋu
#define FIRE_USE_MP		0			// ����MP

// ��
#define WIND_POWER		0
#define WIND_SPEED		50.0f
#define WIND_HP			0
#define WIND_IDLE_TIME	0.1f
#define WIND_GENERATE_INTERVAL 0.1f
#define WIND_USE_MP		0

// �n
#define EARTH_POWER		500
#define EARTH_SPEED		0.0f
#define EARTH_HP		500
#define EARTH_IDLE_TIME	0.1f
#define EARTH_GENERATE_INTERVAL 0.1f
#define EARTH_USE_MP		100

// ��
#define THUNDER_POWER		500
#define THUNDER_SPEED		50.0f
#define THUNDER_HP			0
#define THUNDER_IDLE_TIME	0.0f
#define THUNDER_GENERATE_INTERVAL 0.0f
#define THUNDER_USE_MP		500

// ��
#define WATER_POWER		100
#define WATER_SPEED		10.0f
#define WATER_HP		500
#define WATER_IDLE_TIME	0.1f
#define WATER_GENERATE_INTERVAL 0.1f
#define WATER_USE_MP		100

// ��
#define NEUTRAL_POWER		0
#define NEUTRAL_SPEED		0.0f
#define NEUTRAL_HP			0
#define NEUTRAL_IDLE_TIME	0.1f
#define NEUTRAL_GENERATE_INTERVAL 0.1f
#define NEUTRAL_USE_MP		0

// �{�[���^
#define BALL_POWER		200
#define BALL_SPEED		200.0f
#define BALL_HP			200
#define BALL_IDLE_TIME	0.1f
#define BALL_GENERATE_INTERVAL 0.1f
#define BALL_USE_MP		500
// �I�t�Z�b�g���W
#define BALL_OFFSET_POS_1 CVector(  0.0f,15.0f,7.5f)
#define BALL_OFFSET_POS_2 CVector(  5.0f,10.0f,7.5f)
#define BALL_OFFSET_POS_3 CVector(- 5.0f,10.0f,7.5f)
#define BALL_OFFSET_POS_4 CVector(  0.0f, 5.0f,7.5f)
#define BALL_OFFSET_POS_5 CVector(  0.0f,10.0f,7.5f)
// �ő吶����
#define BALL_MAX_GENERATE_NUM 5
// �ǐՏI�����鋗��
#define BALL_CHASE_END_DIST 17.5f
// ���ł���܂ł̎���
#define BALL_DELETE_TIME 3.0f
// �ǐՏI����̏��ł܂ł̎���
#define BALL_AFTER_CHASE_END_DELETE_TIME 1.0f

// �{���g�^
#define BOLT_POWER		1000
#define BOLT_SPEED		400.0f
#define BOLT_HP			1000
#define BOLT_IDLE_TIME	0.5f
#define BOLT_GENERATE_INTERVAL 0.1f
#define BOLT_USE_MP		800
// �I�t�Z�b�g���W
#define BOLT_OFFSET_POS CVector(0.0f,10.0f,7.5f)
// ���ł���܂ł̎���
#define BOLT_DELETE_TIME 2.0f

// �u���X�^
#define BREATH_POWER		50
#define BREATH_SPEED		300.0f
#define BREATH_HP			1
#define BREATH_IDLE_TIME	0.0f		// �u���X�ł͎g�p���Ȃ�
#define BREATH_GENERATE_INTERVAL 0.05f	// �u���X�̐����Ԋu�͂��̒l�ȊO�̉e���͎󂯂Ȃ�
#define BREATH_USE_MP		400
// �I�t�Z�b�g���W
#define BREATH_OFFSET_POS CVector(0.0f,0.0f,7.5f)
// �ő吶����
#define BREATH_MAX_GENERATE_NUM 25
// �Ԃꕝ
#define BREATH_DIR_RAND 0.1f
// �傫���Ȃ鐔�l
#define BREATH_SCALE_UP_NUM CVector::one * 10.0f * Times::DeltaTime()
// ���ł���܂ł̎���
#define BREATH_DELETE_TIME 0.5f

// �e���|�[�g�^
#define TELEPORT_POWER		0
#define TELEPORT_SPEED		0.0f
#define TELEPORT_HP			0			// �e���|�[�g�����͉��Ȃ�
#define TELEPORT_IDLE_TIME	0.1f		// �e���|�[�g�̑ҋ@���Ԃ͂��̒l�ȊO�̉e���͎󂯂Ȃ�
#define TELEPORT_GENERATE_INTERVAL 0.1f
#define TELEPORT_USE_MP		1000
// �ŏ��̃X�P�[��
#define TELEPORT_SCALE_MIN CVector::zero
// �ő�̃X�P�[��
#define TELEPORT_SCALE_MAX CVector::one
// �X�P�[���̑�����
#define TELEPORT_SCALE_UP_NUM CVector::one * 5.0f * Times::DeltaTime()
// �ړ�����
#define TELEPORT_MOVE_DIST 200.0f
// ���ł���܂ł̎���
#define TELEPORT_DELETE_TIME 1.0f

// �V�[���h�^
#define SHIELD_POWER		0			// �V�[���h�̍U���͕͂K��0
#define SHIELD_SPEED		0.0f
#define SHIELD_HP			100
#define SHIELD_IDLE_TIME	0.0f		// �V�[���h�ł͎g�p���Ȃ�
#define SHIELD_USE_MP		700
// �V�[���h�̐����Ԋu�͍ő吔�Ɖ�]�X�s�[�h����v�Z���ċ��߂�
#define SHIELD_GENERATE_INTERVAL (360.0f / (SHIELD_MAX_GENERATE_NUM + 1)) / SHIELD_ROT_SPEED
// �ő吶����
#define SHIELD_MAX_GENERATE_NUM 8
// �ŏ��̃X�P�[��
#define SHIELD_SCALE_MIN CVector::zero
// �ő�̃X�P�[��
#define SHIELD_SCALE_MAX CVector::one
// �X�P�[���̑�����
#define SHIELD_SCALE_UP_NUM CVector::one * Times::DeltaTime()
// ��]�̔��a
#define SHIELD_ROT_RADIUS 20.0f
// ��]�X�s�[�h
#define SHIELD_ROT_SPEED 180.0f
// ���ł���܂ł̎���
#define SHIELD_DELETE_TIME 10.0f

// ���t���N�^�[�^
#define REFLECTOR_POWER		0				// ���t���N�^�[�̍U���͕͂K��0
#define REFLECTOR_SPEED		0.0f
#define REFLECTOR_HP		1000
#define REFLECTOR_IDLE_TIME	0.0f			// ���t���N�^�[�ł͎g�p���Ȃ�
#define REFLECTOR_GENERATE_INTERVAL 0.1f
#define REFLECTOR_USE_MP		200
// �I�t�Z�b�g���W
#define REFLECTOR_OFFSET_POS CVector(0.0f,0.0f,20.0f)
// �ŏ��̃X�P�[��
#define REFLECTOR_SCALE_MIN CVector::zero
// �ő�̃X�P�[��
#define REFLECTOR_SCALE_MAX CVector::one
// �X�P�[���̑�����
#define REFLECTOR_SCALE_UP_NUM CVector::one * 5.0f * Times::DeltaTime()
// ���ˎ��̋����l
#define REFLECTOR_POWER_UP	100
#define REFLECTOR_HP_UP		500
// ���ł���܂ł̎���
#define REFLECTOR_DELETE_TIME 0.5f