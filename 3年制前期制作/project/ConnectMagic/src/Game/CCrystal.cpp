#include "CCrystal.h"
#include "CSwitch.h"
#include "CModel.h"
#include "CConnectPoint.h"
#include "CPhysicsManager.h"
#include "PhysicsMaterial.h"

// 接続ターゲットの座標
const CVector TARGET_POS =		CVector(0.0f, 5.0f, 0.0f);

// 物理設定
constexpr float MASS =			0.0f;
const CVector HALF_EXTENTS =	CVector(2.0f, 5.0f, 2.0f);

// コンストラクタ
CCrystal::CCrystal(const CVector& pos, const CVector& scale, CSwitch* owner)
	: CConnectObject()
	, mpOwner(owner)
{
	// 接続で移動しない
	SetMove(false);
	mpModelOn = CResourceManager::Get<CModel>("CrystalOn");
	mpModelOff = CResourceManager::Get<CModel>("CrystalOff");
	// 座標を設定
	Position(pos);
	Scale(scale);
	// コライダーを生成
	CreateCol();

	// 接続ターゲットを生成
	CreateTarget(TARGET_POS);
}

// デストラクタ
CCrystal::~CCrystal()
{
}

// 更新
void CCrystal::Update()
{

}

// 描画
void CCrystal::Render()
{
	// スイッチが作動中なら
	if (mpOwner->GetOnOff())
	{
		mpModelOn->Render(Matrix());
	}
	// スイッチが作動していないなら
	else {
		mpModelOff->Render(Matrix());
	}

#if _DEBUG
	// デバッグ時だけ、どれに作用するのか描画
	mpOwner->Render();
#endif
}

// 繋がった時の処理
void CCrystal::Connect(CConnectPoint* otherPoint, bool isWand)
{
	CCrystal* other = dynamic_cast<CCrystal*>(otherPoint->GetConnectObj());
	if (other)
	{
		// 自分がオフで相手がオンなら
		if (!GetOnOff() && other->GetOnOff())
		{
			// 自分をオン
			SetOnOff(true);
			// 相手をオフ
			other->SetOnOff(false);
		}
	}
}

// オンかオフかを設定
void CCrystal::SetOnOff(bool enable)
{
	mpOwner->SetOnOff(enable);
}

// オンかオフかを取得
bool CCrystal::GetOnOff()
{
	return mpOwner->GetOnOff();
}

// コライダーを生成
void CCrystal::CreateCol()
{
	// 物理設定
	PhysicsMaterial material;
	material.mass = MASS;

	// スケールが適用されたサイズ
	CVector halfExtents = HALF_EXTENTS * Scale();

	CPhysicsManager* physicsMgr = CPhysicsManager::Instance();
	// 本体コライダー
	physicsMgr->CreateBoxRigidBody(
		this,
		material,
		halfExtents,
		Position(),
		Rotation(),
		ELayer::eConnectObj,
		{ ELayer::ePlayer,ELayer::eConnectSearch,ELayer::eConnectObj }
	);
}