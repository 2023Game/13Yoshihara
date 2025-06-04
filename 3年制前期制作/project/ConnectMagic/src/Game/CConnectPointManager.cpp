#include "CConnectPointManager.h"
#include "CPlayer.h"
#include "CWand.h"
#include "CConnectPoint.h"
#include "Primitive.h"
#include "CConnectObject.h"
#include "CConnectTarget.h"
#include "Maths.h"

// レイを伸ばせる距離の最大の初期値
#define DEFAULT_RAY_MAX_DISTANCE 100.0f
// レイを縮めれる距離の最小
#define RAY_MIN_DISTANCE 15.0f

// 伸ばせる距離の強化値
#define RAY_MAX_UPGRADE_SCALE 10.0f
// 接続できる最大数の増加に必要な強化アイテムの数
#define CONNECT_UPGRADE_NUM 10

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
	, mConnectMaxNum(DEFAULT_CONNECT_NUM)
	, mpConnectWandTarget(nullptr)
	, mWandConnectDistance(0.0f)
	, mConnectMaxDist(DEFAULT_RAY_MAX_DISTANCE)
	, mUpgradeItemNum(0)
{
	// 杖用の接続部分のビルボード
	mpPoint = new CConnectPoint(nullptr);
	// 最初は非表示
	mpPoint->SetEnable(false);
	mpPoint->SetShow(false);
	// スケール調整
	mpPoint->Scale(mpPoint->Scale() * POINT_SCALE);
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

	// 接続部同士を繋いだ線が何かに衝突したら削除
	RayPoint();

	// 繋がっている処理を実行
	Connect();
}

// 描画
void CConnectPointManager::Render()
{
	// レイの開始と終了地点
	CVector rayStart;
	CVector rayEnd;
	for (int i = 0; i < mPoints.size(); i += 2)
	{
		// 接続部同士で繋がっているか
		bool isPoints = false;
		// iの次の要素番号が要素範囲内なら
		// 接続部同士を線で繋げる
		if (i + 1 < mPoints.size())
		{
			// 接続部がついているオブジェクトが同じなら
			if (mPoints[i]->GetConnectObj() == mPoints[i + 1]->GetConnectObj())
			{
				// ペアで削除
				DeleteConnectPointPair(i);
				// 1セット戻す
				i -= 2;
				// 次へ
				continue;
			}
			rayStart = mPoints[i]->Position();
			rayEnd = mPoints[i + 1]->Position();
			isPoints = true;
		}
		// セットになっていない接続部を杖につなげる
		else
		{
			// 杖の接続部の座標
			rayStart = mpPoint->Position();
			// 接続部の座標
			rayEnd = mPoints[i]->Position();

		}

		// 接続部同士の距離
		float distance = (rayEnd - rayStart).Length();
		// 最大距離より遠いなら
		if (distance > mConnectMaxDist)
		{
			// 接続削除
			DeleteConnectPointPair(i);
			// 接続部同士なら
			if (isPoints)
			{
				// 1セット戻す
				i -= 2;
			}
			// 杖とつながっているなら
			else
			{
				// 杖の接続部を無効
				SetWandConnect(false);
			}
			// 次へ
			continue;
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

// 引っ張る処理を実行
void CConnectPointManager::Pull()
{
	for (int i = 0; i < mPoints.size(); i += 2)
	{
		// iの次の要素番号が要素範囲内なら
		// セットになっている接続部
		if (i + 1 < mPoints.size())
		{
			// 接続部がついているオブジェクト
			CConnectObject* connectObj1 = mPoints[i]->GetConnectObj();
			CConnectObject* connectObj2 = mPoints[i + 1]->GetConnectObj();

			// 引っ張られる方向を求める
			CVector pullDir = mPoints[i + 1]->Position() - mPoints[i]->Position();
			// 長さが最小未満なら次へ
			if (pullDir.Length() < RAY_MIN_DISTANCE) continue;
			// 正規化
			pullDir.Normalize();
			// 引っ張る
			connectObj1->Pull(pullDir, connectObj2->GetWeight());

			// 引っ張られる方向を求める
			pullDir = mPoints[i]->Position() - mPoints[i + 1]->Position();
			// 正規化
			pullDir.Normalize();
			// 引っ張る
			connectObj2->Pull(pullDir, connectObj1->GetWeight());
		}
		// セットになっていない接続部
		else
		{
			// 引っ張られる方向を求める
			// 接続部から杖の方向
			CVector pullDir = mpPoint->Position() - mPoints[i]->Position();
			// 長さが最小未満なら次へ
			if (pullDir.Length() < RAY_MIN_DISTANCE) continue;
			// 正規化
			pullDir.Normalize();
			// 接続部がついているオブジェクト
			CConnectObject* connectObj = mPoints[i]->GetConnectObj();
			// 引っ張る
			connectObj->Pull(pullDir, WAND_WEIGHT);
		}
	}
}

// 繋がっている処理を実行
void CConnectPointManager::Connect()
{
	for (int i = 0; i < mPoints.size(); i += 2)
	{
		// iの次の要素番号が要素範囲内なら
		// セットになっている接続部
		if (i + 1 < mPoints.size())
		{
			// 接続部がついているオブジェクト
			CConnectObject* connectObj1 = mPoints[i]->GetConnectObj();
			CConnectObject* connectObj2 = mPoints[i + 1]->GetConnectObj();

			// どちらかが空なら処理しない
			if (connectObj1 == nullptr || connectObj2 == nullptr) return;
			// 繋げる
			connectObj1->Connect(connectObj2);

			// 繋げる
			connectObj2->Connect(connectObj1);
		}
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
	CVector rayEnd = rayStart + dir * mConnectMaxDist;
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

// 接続部同士を繋いだレイと設定されているコライダーとの衝突判定を行い
// 衝突していたら削除する
void CConnectPointManager::RayPoint()
{

	CHitInfo hit;
	// レイの開始と終了地点
	CVector rayStart;
	CVector rayEnd;
	for (int i = 0; i < mPoints.size(); i += 2)
	{
		// 接続部同士で繋がっているか
		bool isPoints = false;
		// iの次の要素番号が要素範囲内なら
		// 接続部二つを開始と終了地点に設定
		if (i + 1 < mPoints.size())
		{
			rayStart = mPoints[i]->Position();
			rayEnd = mPoints[i + 1]->Position();
			// 接続部同士で繋がっている
			isPoints = true;
		}
		// セットになっていない場合、接続部と杖の接続部を設定
		else
		{
			// 杖の接続部の座標
			rayStart = mpPoint->Position();
			// 接続部の座標
			rayEnd = mPoints[i]->Position();

		}

		// 設定されているコライダーを順番に調べる
		for (CCollider* c : mColliders)
		{
			// レイとコライダーの衝突判定を行う
			if (CCollider::CollisionRay(c, rayStart, rayEnd, &hit))
			{
				// ペア削除
				DeleteConnectPointPair(i);
			}
		}
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

// 接続部を生成
void CConnectPointManager::CreateConnectPoint(CConnectTarget* connectTarget)
{
	// 接続している数が最大値と同じ場合
	// 最古の接続部を削除する
	if (mConnectMaxNum == mPoints.size())
	{
		// 削除
		DeleteConnectPointPair(0);
	}
	// 接続部を生成
	CConnectPoint* point = new CConnectPoint(connectTarget->GetConnectObj());
	// 座標を設定
	point->Position(connectTarget->Position());
	// 親子設定
	point->SetParent(connectTarget);
	// リストに追加
	mPoints.push_back(point);

	// 接続数が奇数なら
	if (mPoints.size() % 2 == 1)
	{
		// 杖に接続
		SetWandConnect(true);
		// 杖と接続中のターゲットを設定
		SetConnectWandTarget(connectTarget);
	}
	// 偶数なら
	else
	{
		// 杖の接続解除
		SetWandConnect(false);
		// 生成した接続部と一つ前の接続部をペアに設定
		int num = mPoints.size() - 2;
		point->SetPair(mPoints[num]);
		mPoints[num]->SetPair(point);
	}
}

// 指定のオブジェクトが親の接続部を削除
void CConnectPointManager::DeleteConnectPoint(CConnectObject* obj)
{
	// 削除する番号
	std::vector<int> deleteNum;
	// 後ろからチェック
	for (int i = mPoints.size() - 1; i >= 0; i--)
	{
		// 一致したら
		if (mPoints[i]->GetConnectObj() == obj)
		{
			// 削除する番号に追加
			deleteNum.push_back(i);
		}
	}

	// ペアで削除
	for (int num : deleteNum)
	{
		DeleteConnectPointPair(num);
	}
}

// 接続部を削除
void CConnectPointManager::DeleteConnectPoint(int num)
{
	// サイズが0かnumが範囲外なら処理しない
	if (mPoints.size() == 0 || mPoints.size() <= num) return;
	// 一時保存
	CConnectPoint* point = mPoints[num];
	// 要素から除外
	mPoints.erase(mPoints.begin() + num);
	// 削除
	point->Kill();
}

// 接続部をペアで削除
void CConnectPointManager::DeleteConnectPointPair(int num)
{
	// 接続部
	CConnectPoint* point1 = mPoints[num];
	CConnectPoint* point2 = point1->GetPair();

	// ペアがnullptrじゃなければ
	if (point2 != nullptr)
	{
		// 配列から取り除く
		mPoints.erase(
			std::remove_if(
				mPoints.begin(), mPoints.end(),
				[&](CConnectPoint* deletePoint)
				{return deletePoint == point1 || deletePoint == point2; }
			),
			mPoints.end()
		);
		// 削除
		point1->Kill();
		point2->Kill();
	}
	// ペアがnullptrなら
	else
	{
		// 一つだけ削除
		DeleteConnectPoint(num);
		// 杖の接続を解除
		SetWandConnect(false);
	}
}

// 最後の要素を消去する
void CConnectPointManager::DeleteLastConnectPoint()
{	
	// サイズが0なら処理しない
	if (mPoints.size() == 0) return;
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

	// 接続解除するとき
	if (!isOnOff)
	{
		// 杖と接続中のターゲットを解除
		SetConnectWandTarget(nullptr);
	}
}

// 杖が接続されているか
bool CConnectPointManager::GetWandConnect()
{
	if (mpPoint == nullptr)
		return false;

	return mpPoint->IsShow();
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
void CConnectPointManager::SetWandConnectDistance()
{
	// 杖が接続されているなら
	if (GetWandConnect())
	{
		// プレイヤーの座標
		CVector rayStart = CPlayer::Instance()->Position();
		// 杖と繋がっている接続部の座標
		CVector rayEnd = mPoints[mPoints.size() - 1]->Position();
		// 2点の距離
		float distance = (rayEnd - rayStart).Length();
		// 杖が接続している接続部とプレイヤーの距離を設定
		mWandConnectDistance = distance;
	}
	// いないなら
	else
	{
		// 不整値を設定
		mWandConnectDistance = -1.0f;
	}
}

// 杖が接続している接続部とプレイヤーの距離を取得
float CConnectPointManager::GetWandConnectDistance()
{
	return mWandConnectDistance;
}

// 杖が接続している接続部とプレイヤーの距離を設定
void CConnectPointManager::SetWandConnectDistance(int sign)
{
	mWandConnectDistance += sign * PULL_POWER / 2 *Times::DeltaTime();

	// ターザンの最短距離より短いなら
	if (mWandConnectDistance < TARZAN_MIN_DISTANCE)
	{
		// 最短距離に設定
		mWandConnectDistance = TARZAN_MIN_DISTANCE;
	}
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

// 接続できる距離を増加
void CConnectPointManager::AddConnectMaxDist()
{
	mConnectMaxDist += RAY_MAX_UPGRADE_SCALE;
}

// 接続できる数を増加
void CConnectPointManager::AddConnectMaxNum()
{
	mConnectMaxNum++;
}

// 強化アイテムの獲得数を増加
void CConnectPointManager::AddUpgradeItemNum()
{
	mUpgradeItemNum++;

	// 接続できる距離を増加
	AddConnectMaxDist();

	// 接続できる最大数の増加に必要な数なら
	if (mUpgradeItemNum % CONNECT_UPGRADE_NUM == 0)
	{
		// 接続できる数の増加
		AddConnectMaxNum();
	}
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
			mpPoint->Position(offsetPos);
		}
	}
}
