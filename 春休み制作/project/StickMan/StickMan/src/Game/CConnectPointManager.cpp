#include "CConnectPointManager.h"
#include "CPlayer.h"
#include "CWand.h"
#include "CConnectPoint.h"
#include "Primitive.h"

// レイを伸ばせる距離
#define RAY_DISTANCE 1000.0f

// 接続部のオフセット座標
#define POINT_OFFSET_POSX 0.0f
#define POINT_OFFSET_POSY 45.0f
#define POINT_OFFSET_POSZ 0.0f

// 接続部のスケール
#define POINT_SCALE 0.5f

// 接続できる最大数の初期値
#define DEFAULT_CONNECT_NUM 1

// インスタンス
CConnectPointManager* CConnectPointManager::spInstance = nullptr;

// インスタンスを取得
CConnectPointManager* CConnectPointManager::Instance()
{
	return spInstance;
}

// コンストラクタ
CConnectPointManager::CConnectPointManager()
	: CTask(ETaskPriority::eNone,0,ETaskPauseType::eGame)
	, mConnectMaxNum(DEFAULT_CONNECT_NUM)
{
	spInstance = this;

	// 杖用の接続部分のビルボード
	mpPoint = new CConnectPoint();
	// 最初は非表示
	mpPoint->SetEnable(false);
	mpPoint->SetShow(false);
	// スケール調整
	mpPoint->Scale(mpPoint->Scale() * POINT_SCALE);
}

// デストラクタ
CConnectPointManager::~CConnectPointManager()
{
	spInstance = nullptr;
}

// 更新
void CConnectPointManager::Update()
{
	// プレイヤーを取得
	CPlayer* player = dynamic_cast<CPlayer*>(CPlayer::Instance());
	if (player != nullptr)
	{
		CWand* wand = player->GetWand();
		// 杖の先に接続部の位置を設定
		CVector offsetPos = wand->Matrix().Position();
		offsetPos += wand->Matrix().VectorX() * POINT_OFFSET_POSX;
		offsetPos += wand->Matrix().VectorY() * POINT_OFFSET_POSY;
		offsetPos += wand->Matrix().VectorZ() * POINT_OFFSET_POSZ;
		mpPoint->Position(offsetPos);
	}
}

// 描画
void CConnectPointManager::Render()
{
	for (int i = 0; i < mPoints.size(); i += 2)
	{
		CVector rayStart;
		CVector rayEnd;
		// iの次の要素番号が要素範囲内なら
		// 接続部同士を線で繋げる
		if (i + 1 < mPoints.size())
		{
			rayStart = mPoints[i]->Position();
			rayEnd = mPoints[i + 1]->Position();
		}
		// セットになっていない接続部を杖につなげる
		else
		{
			// 杖の接続部の座標
			rayStart = mpPoint->Position();
			// 接続部の座標
			rayEnd = mPoints[i]->Position();

		}

		// 黄線を描画
		Primitive::DrawLine
		(
			rayStart, rayEnd,
			CColor::yellow,
			5.0f
		);
	}
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
	CVector rayEnd = rayStart + dir * RAY_DISTANCE;
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

// 接続部を生成
void CConnectPointManager::CreateConnectPoint(CVector createPos)
{
	// 接続している数が最大値と同じ場合
	// 最古の接続部を削除する
	if (mConnectMaxNum == mPoints.size())
	{
		// 最大値が１の場合だけは一つだけ消去
		if (mConnectMaxNum == 1)
		{
			DeleteConnectPoint(0);
		}
		// それ以外は2つ消去
		else
		{
			DeleteConnectPoint(0);
			DeleteConnectPoint(0);
		}
	}
	// 接続部を生成
	CConnectPoint* point = new CConnectPoint();
	// 座標を設定
	point->Position(createPos);
	// リストに追加
	mPoints.push_back(point);

	// 接続数が奇数なら
	if (mPoints.size() % 2 == 1)
	{
		// 杖に接続
		SetWandConnect(true);
	}
	// 偶数なら
	else
	{
		// 杖の接続解除
		SetWandConnect(false);
	}
}

// 接続部を消去
void CConnectPointManager::DeleteConnectPoint(int num)
{
	// 一時保存
	CConnectPoint* point = mPoints[num];
	// 要素から除外
	mPoints.erase(mPoints.begin() + num);
	// 削除
	point->Kill();
}

// 最後の要素を消去する
void CConnectPointManager::DeleteLastConnectPoint()
{
	// 一時保存
	CConnectPoint* point = mPoints[mPoints.size() - 1];
	// 要素から除外
	mPoints.pop_back();
	// 削除
	point->Kill();
}

// 杖の接続部の有効無効を設定
void CConnectPointManager::SetWandConnect(bool isOnOff)
{
	mpPoint->SetEnable(isOnOff);
	mpPoint->SetShow(isOnOff);
}

// 杖が接続されているか
bool CConnectPointManager::GetWandConnect()
{
	return mpPoint->IsShow();
}
