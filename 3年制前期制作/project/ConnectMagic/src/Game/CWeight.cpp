#include "CWeight.h"
#include "CColliderMesh.h"
#include "CColliderSphere.h"
#include "CColliderCapsule.h"
#include "CConnectPointManager.h"
#include <typeinfo>
#include "CPhysicsManager.h"
#include "CInput.h"
#include "btBulletDynamicsCommon.h"

// 接続ターゲットの座標
const CVector TARGET_POS_1 =	CVector(0.0f, 10.0f, 0.0f);

// 重りの重さ
constexpr float WEIGHT =		0.1f;

// コライダーの半径
constexpr float RADIUS =		2.4f;

// スケールの倍率
constexpr float SCALE =			2.0f;

// 水に当たって返ってくるまでの時間
constexpr float RETURN_TIME =	0.5f;

// 削除されるY座標
constexpr float DELETE_POS_Y = -50.0f;

// 物理設定
constexpr float MASS =			1.0f;
const CVector HALF_EXTENTS =	CVector(5.0f, 5.0f, 5.0f);
constexpr float FRICTION =		0.1f;	// 摩擦（値が高いと停止まで早くなる）
constexpr float LIN_DAMPING =	0.8f;	// 線形減衰(値が高いと滑りが小さくなる)
constexpr float ANG_DAMPING =	0.9f;	// 角減衰(値が高いと微細な回転振動を吸収する）

#pragma pack(push,1)// パディング無効化
// 保存するデータ構造
struct WeightSaveData {
	CVector pos;		// 現在座標
	float elapsedTime;	// 経過時間
	bool isAttach;		// スイッチに張り付いているか
};
#pragma pack(pop)	// 設定を元に戻す

std::vector<char> CWeight::SaveState() const
{
	WeightSaveData data;
	data.pos = Position();
	data.elapsedTime = GetElapsedTime();
	data.isAttach = GetIsAttach();

	// データをバイト列に変換して返す
	const char* dataPtr = reinterpret_cast<const char*>(&data);
	return std::vector<char>(dataPtr, dataPtr + sizeof(data));
}

void CWeight::LoadState(const std::vector<char>& data)
{
	// データサイズのチェック
	if (data.size() != sizeof(WeightSaveData))
	{
		return;
	}

	// バイト列を構造体に戻し、データを復元
	const WeightSaveData* saveData = reinterpret_cast<const WeightSaveData*>(data.data());
	Position(saveData->pos);
	SetElapsedTime(saveData->elapsedTime);
	SetIsAttach(saveData->isAttach);
}

size_t CWeight::GetTypeID() const
{
	return typeid(CWeight).hash_code();
}

unsigned int CWeight::GetUniqueInstanceID() const
{
	return mUniqueID;
}

// コンストラクタ
CWeight::CWeight(CVector defaultPos, float scaleRatio)
	: CConnectObject(WEIGHT)
	, mDefaultPos(defaultPos)
	, mIsRespawn(false)
	, mElapsedTime(0.0f)
	, mUniqueID(CUIDManager::GenerateNewID())
{
	mpModel = CResourceManager::Get<CModel>("Weight");

	Scale(Scale() * SCALE * scaleRatio);

	Position(defaultPos);

	// コライダーを生成
	CreateCol();
	
	// 接続ターゲットを生成
	CreateTarget(TARGET_POS_1);
}

// デストラクタ
CWeight::~CWeight()
{
}

// 衝突処理
void CWeight::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	CConnectObject::Collision(self, other, hit);

	if (mpCol == self)
	{
		// 相手が地面なら
		if (other->Layer() == ELayer::eGround)
		{
			// 乗ることが出来るオブジェクトなら
			if (other->Tag() == ETag::eRideableObject)
			{
				// 乗っているオブジェクトに設定する
				mpRideObject = other->Owner();
			}
		}
		// 相手が水なら
		if (other->Layer() == ELayer::eCrushed)
		{
			mIsRespawn = true;
		}
	}
}

// 更新
void CWeight::Update()
{
	if (CInput::PushKey(VK_SPACE))
	{
		AddImpulse(CVector(0.0f, 500.0f, 0.0f));
	}

	// リスポーンするなら
	if (mIsRespawn)
	{
		// 時間を経過
		mElapsedTime += Times::DeltaTime();
		if (mElapsedTime >= RETURN_TIME)
		{
			// リスポーン無効
			mIsRespawn = false;
			// 経過時間リセット
			mElapsedTime = 0.0f;
			// 初期座標に戻す
			Position(mDefaultPos);
		}
	}

	// 削除される座標以下なら
	if (Position().Y() <= DELETE_POS_Y)
	{
		SetEnable(false);
	}

	CConnectObject::Update();
	// 重りが張り付いていない
	SetIsAttach(false);
}

// 重りがスイッチに張り付いているかを設定
void CWeight::SetIsAttach(bool enable)
{
	mIsAttach = enable;
}

// 重りがスイッチに張り付いているか
bool CWeight::GetIsAttach() const
{
	return mIsAttach;
}

void CWeight::SetElapsedTime(float time)
{
	mElapsedTime = time;
}

float CWeight::GetElapsedTime() const
{
	return mElapsedTime;
}

// コライダーを生成
void CWeight::CreateCol()
{
	CPhysicsManager::Instance()->CreateBoxRigidBody(
		this, MASS,
		HALF_EXTENTS,
		Position(),
		Rotation()
	);
	btRigidBody* body = GetRigidBody();
	// 摩擦
	body->setFriction(FRICTION);
	// 減衰
	body->setDamping(LIN_DAMPING, ANG_DAMPING);

	// フィールドやオブジェクトと衝突判定をするコライダー
	mpCol = new CColliderSphere
	(
		this, ELayer::eObject,
		RADIUS
	);
	// 座標を調整
	mpCol->Position(Position() + CVector(0.0f, RADIUS / 2, 0.0f));
	// プレイヤー、敵、フィールド、オブジェクト、コネクトオブジェクトの探知用、スイッチ、水と衝突判定
	mpCol->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy,
		ELayer::eGround, ELayer::eWall,
		ELayer::eObject, ELayer::eConnectSearch, ELayer::eSwitch,
		ELayer::eCrushed});
}
