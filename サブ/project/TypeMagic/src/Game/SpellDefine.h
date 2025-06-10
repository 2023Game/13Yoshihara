#pragma once

// ��
#define FIRE_POWER		1			// �U����
#define FIRE_SPEED		10.0f		// ���x
#define FIRE_HP			1			// �ϋv
#define FIRE_IDLE_TIME	0.1f		// �ҋ@����
#define FIRE_GENERATE_INTERVAL 0.1f	// �����Ԋu

// ��
#define WIND_POWER		1
#define WIND_SPEED		10.0f
#define WIND_HP			1
#define WIND_IDLE_TIME	1.0f
#define WIND_GENERATE_INTERVAL 0.1f

// �n
#define EARTH_POWER		1
#define EARTH_SPEED		10.0f
#define EARTH_HP		1
#define EARTH_IDLE_TIME	1.0f
#define EARTH_GENERATE_INTERVAL 0.1f

// ��
#define THUNDER_POWER		1
#define THUNDER_SPEED		10.0f
#define THUNDER_HP			1
#define THUNDER_IDLE_TIME	1.0f
#define THUNDER_GENERATE_INTERVAL 0.1f

// ��
#define WATER_POWER		1
#define WATER_SPEED		10.0f
#define WATER_HP		1
#define WATER_IDLE_TIME	1.0f
#define WATER_GENERATE_INTERVAL 0.1f

// ��
#define NEUTRAL_POWER		1
#define NEUTRAL_SPEED		10.0f
#define NEUTRAL_HP			1
#define NEUTRAL_IDLE_TIME	1.0f
#define NEUTRAL_GENERATE_INTERVAL 0.1f

// �{�[���^
#define BALL_POWER		1
#define BALL_SPEED		100.0f
#define BALL_HP			1
#define BALL_IDLE_TIME	0.1f
#define BALL_GENERATE_INTERVAL 0.1f
// �I�t�Z�b�g���W
#define BALL_OFFSET_POS_1 CVector(  0.0f,15.0f,7.5f)
#define BALL_OFFSET_POS_2 CVector(  5.0f,10.0f,7.5f)
#define BALL_OFFSET_POS_3 CVector(- 5.0f,10.0f,7.5f)
#define BALL_OFFSET_POS_4 CVector(  0.0f, 5.0f,7.5f)
#define BALL_OFFSET_POS_5 CVector(  0.0f,10.0f,7.5f)
// �ő吶����
#define BALL_MAX_GENERATE_NUM 5
// �ǐՏI�����鋗��
#define BALL_CHASE_END_DIST 12.0f
// ����
#define BALL_ACCELERATE 60.0f * Times::DeltaTime()
// ���ł���܂ł̎���
#define BALL_DELETE_TIME 20.0f
// �ǐՏI����̏��ł܂ł̎���
#define BALL_AFTER_CHASE_END_DELETE_TIME 2.0f

// �{���g�^
#define BOLT_POWER		1
#define BOLT_SPEED		400.0f
#define BOLT_HP			1
#define BOLT_IDLE_TIME	0.5f
#define BOLT_GENERATE_INTERVAL 0.1f
// �I�t�Z�b�g���W
#define BOLT_OFFSET_POS CVector(0.0f,10.0f,7.5f)
// ����
#define BOLT_ACCELERATE 300.0f * Times::DeltaTime()
// ���ł���܂ł̎���
#define BOLT_DELETE_TIME 2.0f

// �u���X�^
#define BREATH_POWER		1
#define BREATH_SPEED		200.0f
#define BREATH_HP			1
#define BREATH_IDLE_TIME	1.0f		// �u���X�ł͎g�p���Ȃ�
#define BREATH_GENERATE_INTERVAL 0.01f	// �u���X�̐����Ԋu�͂��̒l�ȊO�̉e���͎󂯂Ȃ�
// �I�t�Z�b�g���W
#define BREATH_OFFSET_POS CVector(0.0f,0.0f,7.5f)
// �ő吶����
#define BREATH_MAX_GENERATE_NUM 50
// �Ԃꕝ
#define BREATH_DIR_RAND 0.1f
// �傫���Ȃ鐔�l
#define BREATH_SCALE_UP_NUM CVector::one * 10.0f * Times::DeltaTime()
// ���ł���܂ł̎���
#define BREATH_DELETE_TIME 0.5f

// �e���|�[�g�^
#define TELEPORT_POWER		1
#define TELEPORT_SPEED		10.0f
#define TELEPORT_HP			1
#define TELEPORT_IDLE_TIME	0.1f		// �e���|�[�g�̑ҋ@���Ԃ͂��̒l�ȊO�̉e���͎󂯂Ȃ�
#define TELEPORT_GENERATE_INTERVAL 0.1f
// �ŏ��̃X�P�[��
#define TELEPORT_SCALE_MIN CVector::zero
// �ő�̃X�P�[��
#define TELEPORT_SCALE_MAX CVector::one
// �X�P�[���̑�����
#define TELEPORT_SCALE_UP_NUM CVector::one * 10.0f * Times::DeltaTime()
// �ړ�����
#define TELEPORT_MOVE_DIST 100.0f
// ���ł���܂ł̎���
#define TELEPORT_DELETE_TIME 1.0f

// �V�[���h�^
#define SHIELD_POWER		1
#define SHIELD_SPEED		10.0f
#define SHIELD_HP			1
#define SHIELD_IDLE_TIME	1.0f		// �V�[���h�ł͎g�p���Ȃ�
// �V�[���h�̐����Ԋu�͍ő吔�Ɖ�]�X�s�[�h����v�Z���ċ��߂�
#define SHIELD_GENERATE_INTERVAL (360.0f / SHIELD_MAX_GENERATE_NUM) / SHIELD_ROT_SPEED
// �I�t�Z�b�g���W
#define SHIELD_OFFSET_POS CVector(0.0f,0.0f,17.5f)
// �ő吶����
#define SHIELD_MAX_GENERATE_NUM 5
// �ŏ��̃X�P�[��
#define SHIELD_SCALE_MIN CVector::zero
// �ő�̃X�P�[��
#define SHIELD_SCALE_MAX CVector::one
// �X�P�[���̑�����
#define SHIELD_SCALE_UP_NUM CVector::one * Times::DeltaTime()
// ��]�̔��a
#define SHIELD_ROT_RADIUS 0.5f
// ��]�X�s�[�h
#define SHIELD_ROT_SPEED 90.0f
// ���ł���܂ł̎���
#define SHIELD_DELETE_TIME 2.0f

// ���t���N�^�[�^
#define REFLECTOR_POWER		1
#define REFLECTOR_SPEED		10.0f
#define REFLECTOR_HP			1
#define REFLECTOR_IDLE_TIME	1.0f
#define REFLECTOR_GENERATE_INTERVAL 0.1f
// �ŏ��̃X�P�[��
#define REFLECTOR_SCALE_MIN CVector::zero
// �ő�̃X�P�[��
#define REFLECTOR_SCALE_MAX CVector::one
// �X�P�[���̑�����
#define REFLECTOR_SCALE_UP_NUM CVector::one * Times::DeltaTime()
// ���ł���܂ł̎���
#define REFLECTOR_DELETE_TIME 2.0f