#include "CPunisherTrashEnemy.h"
#include "CGaugeUI3D.h"
#include "CDebugFieldOfView.h"

// 大きさ
#define SCALE 0.1f

#define BODY_RADIUS 2.5f	// 本体のコライダ―の半径
#define BODY_HEIGHT 25.0f	// 本体のコライダ―の高さ
#define BODY_WIDTH 50.0f	// 本体のコライダ―の幅

// コンストラクタ
CPunisherTrashEnemy::CPunisherTrashEnemy()
	:CTrashEnemy(true, BODY_RADIUS, BODY_HEIGHT, BODY_WIDTH)
{
	// 最初は無効
	mpHpGauge->SetEnable(false);
	mpHpGauge->SetShow(false);
#if _DEBUG
	mpDebugFov->SetEnable(false);
	mpDebugFov->SetShow(false);
#endif
}

// デストラクタ
CPunisherTrashEnemy::~CPunisherTrashEnemy()
{
}
