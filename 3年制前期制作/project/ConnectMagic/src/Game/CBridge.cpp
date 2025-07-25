#include "CBridge.h"
#include "CColliderMesh.h"
#include "CConnectPointManager.h"
#include "CPlayer.h"

// 重量
#define WEIGHT 1.0f

// 接続ターゲットのオフセット座標
#define TARGET_OFFSET_POS CVector(0.0f,50.0f,0.0f)

// 回転速度
#define ROTATE_SPEED 1.0f
#define GRAVITY_ROTATE_SPEED 180.0f

// 傾ける最大の角度
#define MAX_ANGLE 90.0f
// 傾ける最小の角度
#define MIN_ANGLE 0.0f

// 倒れ始める角度
#define FALL_ANGLE 30.0f

// コンストラクタ
CBridge::CBridge(float defaultAngle)
	: CConnectObject(WEIGHT, ETaskPriority::eBackground, 0, ETaskPauseType::eGame)
	, mCurrentAngle(defaultAngle)
{
	// 重力無効
	mIsGravity = false;

	mpModel = CResourceManager::Get<CModel>("Bridge");

	// コライダーを生成
	CreateCol();

	// 接続ターゲットの生成
	CreateTarget(TARGET_OFFSET_POS);
}

// デストラクタ
CBridge::~CBridge()
{
}

// 引っ張る処理
void CBridge::Connect(CVector wandPointPos, CVector targetPointPos)
{
	CConnectPointManager* pointMgr = CConnectPointManager::Instance();
	// カメラの方向
	CVector cameraDir = -CCamera::CurrentCamera()->VectorZ();
	// 新しい座標を求める
	CVector newPos = CPlayer::Instance()->Position() + cameraDir * pointMgr->GetConnectDistance();
	
	CVector pullDir = newPos - Position();

	// 傾く角度
	float angle = ROTATE_SPEED * Times::DeltaTime();

	// 橋を傾ける
	RotateBridge(angle, pullDir);
}

// 更新処理
void CBridge::Update()
{
#if _DEBUG
	CDebugPrint::Print("Bridge_Angle:%f\n", mCurrentAngle);
#endif
	// 傾く角度
	float angle = GRAVITY_ROTATE_SPEED * Times::DeltaTime();
	// 橋を重力で傾ける
	RotateBridge(angle, CVector::down);
}

// コライダーを生成
void CBridge::CreateCol()
{
	mpCol = new CColliderMesh
	(
		this, ELayer::eObject,
		CResourceManager::Get<CModel>("Bridge_Col"),
		true
	);
	// プレイヤーと敵とオブジェクト
	// と接続オブジェクトの探知用とだけ衝突判定する
	mpCol->SetCollisionLayers({ 
		ELayer::ePlayer,ELayer::eEnemy,
		ELayer::eObject,ELayer::eConnectSearch });
}

// 角度分橋を傾ける
void CBridge::RotateBridge(float angle, CVector dir)
{
	// 背面方向との関係
	float dot = -VectorZ().Dot(dir);
	// 力のかかり具合で傾きを調整
	angle *= dot;

	float direction = -VectorZ().Dot(CVector::forward);
	// 符号だけ取り出す
	direction = std::_Float_copysign(1.0f, direction);

	// 背面方向へ傾く(橋が降りる方向)
	if (dot > 0.0f)
	{
		// 変更後の傾きが
		// 傾ける最大の角度より小さいなら
		if (mCurrentAngle + angle < MAX_ANGLE)
		{
			// 現在の角度に加算
			mCurrentAngle += angle;
			Rotate(VectorX() * angle * direction);
		}
		// 傾ける最大の角度より大きいなら
		else if (mCurrentAngle + angle > MAX_ANGLE)
		{
			// 最大値になるように角度を設定
			angle = MAX_ANGLE - mCurrentAngle;
			mCurrentAngle = MAX_ANGLE;
			Rotate(VectorX() * angle * direction);
		}
	}
	// 正面方向へ傾く(橋が上がる方向)
	else if (dot < 0.0f)
	{
		// 変更後の傾きが
		// 傾ける最小の角度より大きいなら
		if (mCurrentAngle + angle > MIN_ANGLE)
		{
			// 現在の角度に加算
			mCurrentAngle += angle;
			Rotate(VectorX() * angle * direction);
		}
		// 傾ける最小の角度より小さいなら
		else if (mCurrentAngle + angle < MIN_ANGLE)
		{
			angle = mCurrentAngle - MIN_ANGLE;
			mCurrentAngle = MIN_ANGLE;
			Rotate(VectorX() * angle * direction);
		}
	}
}

// 指定角度に設定する
void CBridge::SetAngle(float angle)
{
	// 回転を取得
	CVector rot = EulerAngles();
	// x軸の角度を設定
	rot.X(angle);
	// 回転を設定
	Rotation(rot);
	// 回転した角度を設定
	mCurrentAngle = angle;
}
