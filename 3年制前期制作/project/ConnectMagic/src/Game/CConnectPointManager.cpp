#include "CConnectPointManager.h"
#include "CPlayer.h"
#include "CWand.h"
#include "CConnectPoint.h"
#include "Primitive.h"
#include "CConnectObject.h"
#include "CConnectTarget.h"
#include "Maths.h"

// レイを伸ばせる距離の最大の初期値
#define RAY_MAX_DISTANCE 100.0f
// レイを縮めれる距離の最小
#define RAY_MIN_DISTANCE 15.0f

// ターザンの最短距離
#define TARZAN_MIN_DISTANCE 30.0f

// 接続部のオフセット座標
#define POINT_OFFSET_POSX 0.0f
#define POINT_OFFSET_POSY 45.0f
#define POINT_OFFSET_POSZ 0.0f

// 接続部のスケール
#define POINT_SCALE 0.5f

// 接続できる最大数の初期値
#define DEFAULT_CONNECT_NUM 1

// 杖の重量
#define WAND_WEIGHT 0.5f

// インスタンス
CConnectPointManager* CConnectPointManager::spInstance = nullptr;

// インスタンスを取得
CConnectPointManager* CConnectPointManager::Instance()
{
	if (spInstance == nullptr)
	{
		spInstance = new CConnectPointManager();
	}
	return spInstance;
}

// コンストラクタ
CConnectPointManager::CConnectPointManager()
	: CTask(ETaskPriority::eNone, 0, ETaskPauseType::eGame)
	, mpConnectWandTarget(nullptr)
	, mConnectDistance(0.0f)
{
	// 杖用の接続部分のビルボード
	mpWandPoint = new CConnectPoint(nullptr);
	// 最初は非表示
	mpWandPoint->SetEnable(false);
	mpWandPoint->SetShow(false);
	// スケール調整
	mpWandPoint->Scale(mpWandPoint->Scale() * POINT_SCALE);

	// 接続先の接続部のビルボード
	mpConnectPoint = new CConnectPoint(nullptr);
	// 最初は無効
	mpConnectPoint->SetEnable(false);
	mpConnectPoint->SetShow(false);
}

// デストラクタ
CConnectPointManager::~CConnectPointManager()
{
	if (spInstance == this)
		spInstance = nullptr;
}

// 更新
void CConnectPointManager::Update()
{
	// 杖の先の座標を求める
	WandPos();

	// 接続部と繋いだ線が何かに衝突したら無効
	RayPoint();

	// 接続部との距離が最大値より遠いか
	// 最小値より近ければ接続を無効にする
	FarOrNearDist();

	// 繋がっている処理
	Connect();

#if _DEBUG
	CDebugPrint::Print("ConnectDist:%f\n", mConnectDistance);
#endif
}

// 描画
void CConnectPointManager::Render()
{
	// 杖が接続されていなければ処理しない
	if (!GetWandConnect()) return;
	// 黄線を描画
	Primitive::DrawLine
	(
		mpWandPoint->Position(), mpConnectPoint->Position(),
		CColor::yellow,
		5.0f
	);
}

// 繋がっている処理を実行
void CConnectPointManager::Connect()
{
	// 杖が接続されていないなら処理しない
	if (!GetWandConnect()) return;
	
	mpConnectPoint->GetConnectObj()->Connect(mpWandPoint->Position(), mpConnectPoint->Position());
}

// 設定されているコライダーと衝突判定を行う
bool CConnectPointManager::Ray(CVector& hitPos)
{
	CHitInfo hit;
	// カメラを取得
	CCamera* camera = CCamera::CurrentCamera();
	// 視点から注視点への方向
	CVector dir = camera->GetEyeVec();
	dir.Normalize();
	// レイの開始と終了地点
	CVector rayStart = camera->GetEye();
	CVector rayEnd = rayStart + dir * RAY_MAX_DISTANCE;
	float nearDist = 0.0f;
	bool isHit = false;
	// 設定されているコライダーを順番に調べる
	for (CCollider* c : mColliders)
	{
		// レイとコライダーの衝突判定を行う
		if (CCollider::CollisionRay(c, rayStart, rayEnd, &hit))
		{
			// 交点が不整値でなければ、
			if (hit.cross.LengthSqr() != 0.0f)
			{
				// 衝突位置までの距離で一番近い距離を求める
				if (!isHit)
				{
					nearDist = hit.dist;
					hitPos = hit.cross;
				}
				else if (nearDist > hit.dist)
				{
					nearDist = hit.dist;
					hitPos = hit.cross;
				}
				isHit = true;
			}
		}
	}

	return isHit;
}

// 2点を繋いだレイと設定されているコライダーとの衝突判定を行う
bool CConnectPointManager::Ray(const CVector& start, const CVector& end, CHitInfo* hit)
{
	return false;
}

// 視点からターゲットまでのレイと設定されているコライダーとの衝突判定を行う
bool CConnectPointManager::RayTarget(CVector targetPos)
{
	CHitInfo hit;
	// プレイヤーを取得
	CPlayerBase* player = CPlayerBase::Instance();
	// プレイヤーの座標
	CVector playerPos = player->Position();
	// カメラを取得
	CCamera* camera = CCamera::CurrentCamera();
	// プレイヤーからターゲットへの方向
	CVector dir = targetPos - playerPos;
	dir.Y(0.0f);
	dir.Normalize();
	// レイの開始と終了地点
	CVector rayStart = camera->GetEye();
	CVector rayEnd = targetPos;
	// 設定されているコライダーを順番に調べる
	for (CCollider* c : mColliders)
	{
		// レイとコライダーの衝突判定を行う
		if (CCollider::CollisionRay(c, rayStart, rayEnd, &hit))
		{
			// プレイヤーから衝突点への方向
			CVector playerCrossDir = hit.cross - playerPos;
			playerCrossDir.Y(0.0f);
			playerCrossDir.Normalize();
			// 二つの方向の内積を求める
			float dot = playerCrossDir.Dot(dir);
			
			// 内積が負の値なら反対方向（プレイヤーの後ろ）にあるので衝突を無視
			if (dot < 0.0f)
				continue;

			// 衝突した
			return true;
		}
	}

	// 衝突していない
	return false;
}

// 接続部と繋いだレイと設定されているコライダーとの衝突判定を行い
// 衝突していたら接続解除
void CConnectPointManager::RayPoint()
{
	CHitInfo hit;
	// レイの開始と終了地点
	CVector rayStart = mpWandPoint->Position();
	CVector rayEnd = mpConnectPoint->Position();

	// 設定されているコライダーを順番に調べる
	for (CCollider* c : mColliders)
	{
		// レイとコライダーの衝突判定を行う
		if (CCollider::CollisionRay(c, rayStart, rayEnd, &hit))
		{
			// 接続を無効
			DisableConnect(GetConnectWandTarget());
		}
	}
}

// 接続部との距離が最大値より遠いか
// 最小値より近ければ接続を無効にする
void CConnectPointManager::FarOrNearDist()
{
	// 杖が接続されていないなら処理しない
	if (!GetWandConnect()) return;

	// 最大距離より遠いなら
	if (GetNowConnectDistance() > RAY_MAX_DISTANCE)
	{
		// 接続を無効
		DisableConnect(GetConnectWandTarget());
	}
	// 最小距離より近いなら
	else if (GetNowConnectDistance() < RAY_MIN_DISTANCE)
	{
		// 接続を無効
		DisableConnect(GetConnectWandTarget());
	}
}

// 衝突判定を行うコライダーをリストに追加
void CConnectPointManager::AddCollider(CCollider* col)
{
	mColliders.push_back(col);
}

// 衝突判定を行うコライダーをリストから取り除く
void CConnectPointManager::RemoveCollider(CCollider* col)
{
	mColliders.remove(col);
}

// 衝突判定を行うコライダーをリセット
void CConnectPointManager::ResetCollider()
{
	mColliders.clear();
}

// 接続部を有効
void CConnectPointManager::EnableConnect(CConnectTarget* connectTarget)
{
	// 座標を設定
	mpConnectPoint->Position(connectTarget->Position());
	// 親子設定
	mpConnectPoint->SetParent(connectTarget);
	// 接続オブジェクトを設定
	mpConnectPoint->SetConnectObj(connectTarget->GetConnectObj());

	// 杖に接続
	SetWandConnect(true, connectTarget);

	// 接続部を有効
	mpConnectPoint->SetEnable(true);
	mpConnectPoint->SetShow(true); 
	
	// 接続部との距離を設定
	SetConnectDistance();
}

// 接続を無効
void CConnectPointManager::DisableConnect(CConnectTarget* connectTarget)
{
	// 接続中のターゲットでないなら処理しない
	if (GetConnectWandTarget() != connectTarget) return;

	// 杖の接続解除
	SetWandConnect(false, nullptr);

	// 接続部を無効
	mpConnectPoint->SetEnable(false);
	mpConnectPoint->SetShow(false);
}

// 杖の接続部の有効無効を設定
void CConnectPointManager::SetWandConnect(bool isOnOff, CConnectTarget* target)
{
	mpWandPoint->SetEnable(isOnOff);
	mpWandPoint->SetShow(isOnOff);

	// 杖と接続中のターゲットを設定
	SetConnectWandTarget(target);
}

// 杖が接続されているか
bool CConnectPointManager::GetWandConnect()
{
	if (mpWandPoint == nullptr)
		return false;

	return mpWandPoint->IsShow();
}

// 杖と接続中のターゲットを設定
void CConnectPointManager::SetConnectWandTarget(CConnectTarget* connectTarget)
{
	mpConnectWandTarget = connectTarget;
}

// 杖と接続中のターゲットを取得
CConnectTarget* CConnectPointManager::GetConnectWandTarget()
{
	return mpConnectWandTarget;
}

// 杖が接続している接続部とプレイヤーの距離を設定
void CConnectPointManager::SetConnectDistance()
{
	// 杖が接続されているなら
	if (GetWandConnect())
	{
		// プレイヤーの座標
		CVector rayStart = CPlayer::Instance()->Position();
		// 杖と繋がっている接続部の座標
		CVector rayEnd = mpConnectPoint->Position();
		// 2点の距離
		float distance = (rayEnd - rayStart).Length();
		// 杖が接続している接続部とプレイヤーの距離を設定
		mConnectDistance = distance;
	}
	// いないなら
	else
	{
		// 不整値を設定
		mConnectDistance = -1.0f;
	}
}

// 杖が接続している接続部とプレイヤーの距離を取得
float CConnectPointManager::GetConnectDistance()
{				
	return mConnectDistance;
}

// 現在の杖が接続している接続部とプレイヤーの距離を取得
float CConnectPointManager::GetNowConnectDistance()
{
	// プレイヤーの座標
	CVector rayStart = CPlayer::Instance()->Position();
	// 杖と繋がっている接続部の座標
	CVector rayEnd = mpConnectPoint->Position();
	// 2点の距離
	float distance = (rayEnd - rayStart).Length();

	return distance;
}

// 杖と接続しているオブジェクトが空中の接続オブジェクトか
bool CConnectPointManager::IsWandConnectAirObject()
{
	// 杖が接続されているか
	if (GetWandConnect())
	{
		// 杖と接続中のオブジェクト
		CConnectObject* obj = GetConnectWandTarget()->GetConnectObj();
		if (obj != nullptr)
		{
			// 空中の接続オブジェクトなら
			if (obj->GetConnectObjTag() == EConnectObjTag::eAir)
			{
				return true;
			}
		}
	}

	return false;
}

// 杖の先の接続部の位置を特定
void CConnectPointManager::WandPos()
{
	// プレイヤーを取得
	CPlayer* player = dynamic_cast<CPlayer*>(CPlayer::Instance());

	if (player != nullptr)
	{
		CWand* wand = player->GetWand();
		if (wand != nullptr)
		{
			// 杖の先に接続部の位置を設定
			CVector offsetPos = wand->Matrix().Position();
			offsetPos += wand->Matrix().VectorX() * POINT_OFFSET_POSX;
			offsetPos += wand->Matrix().VectorY() * POINT_OFFSET_POSY;
			offsetPos += wand->Matrix().VectorZ() * POINT_OFFSET_POSZ;
			mpWandPoint->Position(offsetPos);
		}
	}
}
