#include "CCrystal.h"
#include "CColliderSphere.h"
#include "CSwitch.h"
#include "CModel.h"
#include "CConnectPoint.h"

// 接続ターゲットの座標
#define TARGET_POS CVector( 0.0f,5.0f, 0.0f)
// コライダーの半径
#define COL_RADIUS 5.0f

// コンストラクタ
CCrystal::CCrystal(CVector pos, CSwitch* owner)
	: CConnectObject()
	, mpOwner(owner)
{
	// 接続で移動しない
	SetMove(false);
	mpModelOn = CResourceManager::Get<CModel>("CrystalOn");
	mpModelOff = CResourceManager::Get<CModel>("CrystalOff");
	// コライダーを生成
	CreateCol();

	// 接続ターゲットを生成
	CreateTarget(TARGET_POS);
	// 座標を設定
	Position(pos);
}

// デストラクタ
CCrystal::~CCrystal()
{
	SAFE_DELETE(mpCol);
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
	mpCol = new CColliderSphere
	(
		this, ELayer::eObject,
		COL_RADIUS,
		true
	);
	// プレイヤー、敵、オブジェクト、コネクトオブジェクトの探知用とだけ衝突
	mpCol->SetCollisionLayers({ ELayer::ePlayer,
		ELayer::eEnemy,ELayer::eObject,ELayer::eConnectSearch });
}