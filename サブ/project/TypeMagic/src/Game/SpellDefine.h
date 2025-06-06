#pragma once

// 狙う高さ
#define TARGET_HEIGHT 10.0f

// 炎
#define FIRE_POWER		1			// 攻撃力
#define FIRE_SPEED		10.0f		// 速度
#define FIRE_HP			1			// 耐久
#define FIRE_IDLE_TIME	0.1f		// 待機時間
#define FIRE_GENERATE_SPEED 0.1f	// 生成速度

// 風
#define WIND_POWER		1
#define WIND_SPEED		10.0f
#define WIND_HP			1
#define WIND_IDLE_TIME	1.0f
#define WIND_GENERATE_SPEED 0.1f

// 地
#define EARTH_POWER		1
#define EARTH_SPEED		10.0f
#define EARTH_HP		1
#define EARTH_IDLE_TIME	1.0f
#define EARTH_GENERATE_SPEED 0.1f

// 雷
#define THUNDER_POWER		1
#define THUNDER_SPEED		10.0f
#define THUNDER_HP			1
#define THUNDER_IDLE_TIME	1.0f
#define THUNDER_GENERATE_SPEED 0.1f

// 水
#define WATER_POWER		1
#define WATER_SPEED		10.0f
#define WATER_HP		1
#define WATER_IDLE_TIME	1.0f
#define WATER_GENERATE_SPEED 0.1f

// 無
#define NEUTRAL_POWER		1
#define NEUTRAL_SPEED		10.0f
#define NEUTRAL_HP			1
#define NEUTRAL_IDLE_TIME	1.0f
#define NEUTRAL_GENERATE_SPEED 0.1f

// ボール型
#define BALL_POWER		1
#define BALL_SPEED		100.0f
#define BALL_HP			1
#define BALL_IDLE_TIME	0.1f
#define BALL_GENERATE_SPEED 0.1f
// オフセット座標
#define BALL_OFFSET_POS_1 CVector(  0.0f,25.0f,5.0f)
#define BALL_OFFSET_POS_2 CVector(  5.0f,20.0f,5.0f)
#define BALL_OFFSET_POS_3 CVector(- 5.0f,20.0f,5.0f)
#define BALL_OFFSET_POS_4 CVector( 10.0f,15.0f,5.0f)
#define BALL_OFFSET_POS_5 CVector(-10.0f,15.0f,5.0f)
// 最大生成数
#define BALL_MAX_GENERATE_NUM 5
// 追跡終了する距離
#define BALL_CHASE_END_DIST 12.0f
// 加速
#define BALL_ACCELERATE 1.0f
// 消滅するまでの時間
#define BALL_DELETE_TIME 2.0f

// ボルト型
#define BOLT_POWER		1
#define BOLT_SPEED		400.0f
#define BOLT_HP			1
#define BOLT_IDLE_TIME	0.5f
#define BOLT_GENERATE_SPEED 0.1f
// オフセット座標
#define BOLT_OFFSET_POS CVector(0.0f,TARGET_HEIGHT,7.5f)
// 加速
#define BOLT_ACCELERATE 5.0f
// 消滅するまでの時間
#define BOLT_DELETE_TIME 2.0f

// ブレス型
#define BREATH_POWER		1
#define BREATH_SPEED		10.0f
#define BREATH_HP			1
#define BREATH_IDLE_TIME	1.0f
#define BREATH_GENERATE_SPEED 0.1f
// 消滅するまでの時間
#define BREATH_DELETE_TIME 2.0f

// テレポート型
#define TELEPORT_POWER		1
#define TELEPORT_SPEED		10.0f
#define TELEPORT_HP			1
#define TELEPORT_IDLE_TIME	1.0f
#define TELEPORT_GENERATE_SPEED 0.1f
// 消滅するまでの時間
#define TELEPORT_DELETE_TIME 2.0f

// シールド型
#define SHIELD_POWER		1
#define SHIELD_SPEED		10.0f
#define SHIELD_HP			1
#define SHIELD_IDLE_TIME	1.0f
#define SHIELD_GENERATE_SPEED 0.1f
// 消滅するまでの時間
#define SHIELD_DELETE_TIME 2.0f

// リフレクター型
#define REFLECTOR_POWER		1
#define REFLECTOR_SPEED		10.0f
#define REFLECTOR_HP			1
#define REFLECTOR_IDLE_TIME	1.0f
#define REFLECTOR_GENERATE_SPEED 0.1f
// 消滅するまでの時間
#define REFLECTOR_DELETE_TIME 2.0f