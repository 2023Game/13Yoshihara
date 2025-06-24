#pragma once

#define ELEMENTAL_POWER_RATIO	2.0f	// 弱点属性の倍率

// プレイヤーの色
#define PLAYER_COLOR	CColor(0.15f,0.2f,0.6f)
// 敵の色
#define ENEMY_COLOR		CColor(0.6f,0.1f,0.1f)

// 炎の色
#define FIRE_COLOR		CColor(1.0f,0.3f,0.1f)
// 風の色
#define WIND_COLOR		CColor(0.6f,0.9f,0.6f)
// 地の色
#define EARTH_COLOR		CColor(0.647f, 0.165f, 0.165f)
// 雷の色
#define THUNDER_COLOR	CColor(0.9f,0.75f,0.2f)
// 水の色
#define WATER_COLOR		CColor(0.4f,0.6f,1.0f)
// 無の色
#define NEUTRAL_COLOR	CColor(0.9f,0.9f,0.9f)

// 炎
#define FIRE_POWER		1			// 攻撃力
#define FIRE_SPEED		10.0f		// 速度
#define FIRE_HP			1			// 耐久
#define FIRE_IDLE_TIME	0.1f		// 待機時間
#define FIRE_GENERATE_INTERVAL 0.1f	// 生成間隔
#define FIRE_USE_MP		1			// 消費MP

// 風
#define WIND_POWER		1
#define WIND_SPEED		10.0f
#define WIND_HP			1
#define WIND_IDLE_TIME	0.1f
#define WIND_GENERATE_INTERVAL 0.1f
#define WIND_USE_MP		1

// 地
#define EARTH_POWER		1
#define EARTH_SPEED		10.0f
#define EARTH_HP		1
#define EARTH_IDLE_TIME	0.1f
#define EARTH_GENERATE_INTERVAL 0.1f
#define EARTH_USE_MP		1

// 雷
#define THUNDER_POWER		1
#define THUNDER_SPEED		10.0f
#define THUNDER_HP			1
#define THUNDER_IDLE_TIME	0.1f
#define THUNDER_GENERATE_INTERVAL 0.1f
#define THUNDER_USE_MP		1

// 水
#define WATER_POWER		1
#define WATER_SPEED		10.0f
#define WATER_HP		1
#define WATER_IDLE_TIME	0.1f
#define WATER_GENERATE_INTERVAL 0.1f
#define WATER_USE_MP		1

// 無
#define NEUTRAL_POWER		1
#define NEUTRAL_SPEED		10.0f
#define NEUTRAL_HP			1
#define NEUTRAL_IDLE_TIME	0.1f
#define NEUTRAL_GENERATE_INTERVAL 0.1f
#define NEUTRAL_USE_MP		1

// ボール型
#define BALL_POWER		10
#define BALL_SPEED		200.0f
#define BALL_HP			10
#define BALL_IDLE_TIME	0.1f
#define BALL_GENERATE_INTERVAL 0.1f
#define BALL_USE_MP		10
// オフセット座標
#define BALL_OFFSET_POS_1 CVector(  0.0f,15.0f,7.5f)
#define BALL_OFFSET_POS_2 CVector(  5.0f,10.0f,7.5f)
#define BALL_OFFSET_POS_3 CVector(- 5.0f,10.0f,7.5f)
#define BALL_OFFSET_POS_4 CVector(  0.0f, 5.0f,7.5f)
#define BALL_OFFSET_POS_5 CVector(  0.0f,10.0f,7.5f)
// 最大生成数
#define BALL_MAX_GENERATE_NUM 5
// 追跡終了する距離
#define BALL_CHASE_END_DIST 17.5f
// 消滅するまでの時間
#define BALL_DELETE_TIME 2.0f
// 追跡終了後の消滅までの時間
#define BALL_AFTER_CHASE_END_DELETE_TIME 2.0f

// ボルト型
#define BOLT_POWER		1
#define BOLT_SPEED		400.0f
#define BOLT_HP			1
#define BOLT_IDLE_TIME	0.5f
#define BOLT_GENERATE_INTERVAL 0.1f
#define BOLT_USE_MP		1
// オフセット座標
#define BOLT_OFFSET_POS CVector(0.0f,10.0f,7.5f)
// 消滅するまでの時間
#define BOLT_DELETE_TIME 2.0f

// ブレス型
#define BREATH_POWER		2
#define BREATH_SPEED		200.0f
#define BREATH_HP			1
#define BREATH_IDLE_TIME	0.0f		// ブレスでは使用しない
#define BREATH_GENERATE_INTERVAL 0.05f	// ブレスの生成間隔はこの値以外の影響は受けない
#define BREATH_USE_MP		1
// オフセット座標
#define BREATH_OFFSET_POS CVector(0.0f,0.0f,7.5f)
// 最大生成数
#define BREATH_MAX_GENERATE_NUM 25
// ぶれ幅
#define BREATH_DIR_RAND 0.1f
// 大きくなる数値
#define BREATH_SCALE_UP_NUM CVector::one * 10.0f * Times::DeltaTime()
// 消滅するまでの時間
#define BREATH_DELETE_TIME 0.5f

// テレポート型
#define TELEPORT_POWER		1
#define TELEPORT_SPEED		10.0f
#define TELEPORT_HP			0			// テレポート呪文は壊れない
#define TELEPORT_IDLE_TIME	0.1f		// テレポートの待機時間はこの値以外の影響は受けない
#define TELEPORT_GENERATE_INTERVAL 0.1f
#define TELEPORT_USE_MP		1
// 最小のスケール
#define TELEPORT_SCALE_MIN CVector::zero
// 最大のスケール
#define TELEPORT_SCALE_MAX CVector::one
// スケールの増加量
#define TELEPORT_SCALE_UP_NUM CVector::one * 5.0f * Times::DeltaTime()
// 移動距離
#define TELEPORT_MOVE_DIST 100.0f
// 消滅するまでの時間
#define TELEPORT_DELETE_TIME 1.0f

// シールド型
#define SHIELD_POWER		0			// シールドの攻撃力は必ず0
#define SHIELD_SPEED		10.0f
#define SHIELD_HP			50
#define SHIELD_IDLE_TIME	0.0f		// シールドでは使用しない
#define SHIELD_USE_MP		1
// シールドの生成間隔は最大数と回転スピードから計算して求める
#define SHIELD_GENERATE_INTERVAL (360.0f / (SHIELD_MAX_GENERATE_NUM + 1)) / SHIELD_ROT_SPEED
// 最大生成数
#define SHIELD_MAX_GENERATE_NUM 8
// 最小のスケール
#define SHIELD_SCALE_MIN CVector::zero
// 最大のスケール
#define SHIELD_SCALE_MAX CVector::one
// スケールの増加量
#define SHIELD_SCALE_UP_NUM CVector::one * Times::DeltaTime()
// 回転の半径
#define SHIELD_ROT_RADIUS 20.0f
// 回転スピード
#define SHIELD_ROT_SPEED 180.0f
// 消滅するまでの時間
#define SHIELD_DELETE_TIME 10.0f

// リフレクター型
#define REFLECTOR_POWER		0				// リフレクターの攻撃力は必ず0
#define REFLECTOR_SPEED		10.0f
#define REFLECTOR_HP		50
#define REFLECTOR_IDLE_TIME	0.0f			// リフレクターでは使用しない
#define REFLECTOR_GENERATE_INTERVAL 0.1f
#define REFLECTOR_USE_MP		1
// オフセット座標
#define REFLECTOR_OFFSET_POS CVector(0.0f,0.0f,20.0f)
// 最小のスケール
#define REFLECTOR_SCALE_MIN CVector::zero
// 最大のスケール
#define REFLECTOR_SCALE_MAX CVector::one
// スケールの増加量
#define REFLECTOR_SCALE_UP_NUM CVector::one * 5.0f * Times::DeltaTime()
// 反射時の強化値
#define REFLECTOR_POWER_UP	10
#define REFLECTOR_HP_UP		50
// 消滅するまでの時間
#define REFLECTOR_DELETE_TIME 0.5f