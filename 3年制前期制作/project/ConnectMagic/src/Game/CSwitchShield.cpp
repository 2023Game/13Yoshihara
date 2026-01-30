#include "CSwitchShield.h"
#include "CMaterial.h"
#include "CConnectPointManager.h"
#include "CPhysicsManager.h"
#include "CollisionData.h"
#include "PhysicsMaterial.h"
#include <typeinfo>

// 頂点
const CVector VERT_POS_1 = CVector(-10.0f,  40.0f,  0.0f);
const CVector VERT_POS_2 = CVector(-10.0f,	0.0f,	0.0f);
const CVector VERT_POS_3 = CVector( 10.0f,	0.0f,	0.0f);
const CVector VERT_POS_4 = CVector( 10.0f,	40.0f,	0.0f);

// 物理設定
constexpr float MASS = 0.0f;
const CVector HALF_EXTENTS = CVector(10.0f, 20.0f, 1.0f);

#pragma pack(push,1)// パディング無効化
// 保存するデータ構造
struct SwitchShieldSaveData {
	bool isShow;		// 描画が有効か
	bool isEnableCol;	// 衝突判定が有効か
	CSwitchObject::EState state;	// 状態
};
#pragma pack(pop)	// 設定を元に戻す

std::vector<char> CSwitchShield::SaveState() const
{
	SwitchShieldSaveData data;
	data.isShow = IsShow();
	data.isEnableCol = IsEnableCol();
	data.state = GetState();

	// データをバイト列に変換して返す
	const char* dataPtr = reinterpret_cast<const char*>(&data);
	return std::vector<char>(dataPtr, dataPtr + sizeof(data));
}

void CSwitchShield::LoadState(const std::vector<char>& data)
{
	if (data.size() != sizeof(SwitchShieldSaveData))
	{
		return;
	}

	// バイト列を構造体に戻し、データを復元
	const SwitchShieldSaveData* saveData = reinterpret_cast<const SwitchShieldSaveData*>(data.data());
	SetShow(saveData->isShow);
	SetEnableCol(saveData->isEnableCol);
	SetState(saveData->state);
}

size_t CSwitchShield::GetTypeID() const
{
	return typeid(CSwitchShield).hash_code();
}

unsigned int CSwitchShield::GetUniqueInstanceID() const
{
	return mUniqueID;
}

// コンストラクタ
CSwitchShield::CSwitchShield(CVector scale)
	: CSwitchObject(ETaskPriority::eShield)
	, mUniqueID(CUIDManager::GenerateNewID())
{
	Scale(scale);
	// コライダーを生成
	CreateCol();
}

// デストラクタ
CSwitchShield::~CSwitchShield()
{
}

// 描画
void CSwitchShield::Render()
{
	// 現在の行列を退避しておく
	glPushMatrix();

	// 自身の行列を適用
	glMultMatrixf(Matrix().M());

	// アルファブレンドを有効にする
	glEnable(GL_BLEND);
	// Zバッファへの書き込みを無効
	glDepthMask(GL_FALSE);
	// ブレンド方法を指定
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// ライトオフ
	glDisable(GL_LIGHTING);
	// 裏面描画
	glDisable(GL_CULL_FACE);

	// DIFFUSE青色設定
	CColor col = CColor::cyan;
	col.A(0.2f);
	float* c = (float*)&col;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, c);
	glColor4fv(c);

	// 四角形を描画
	glBegin(GL_QUADS);
	glVertex3f(VERT_POS_1.X(),VERT_POS_1.Y(),VERT_POS_1.Z());
	glVertex3f(VERT_POS_2.X(),VERT_POS_2.Y(),VERT_POS_2.Z());
	glVertex3f(VERT_POS_3.X(),VERT_POS_3.Y(),VERT_POS_3.Z());
	glVertex3f(VERT_POS_4.X(),VERT_POS_4.Y(),VERT_POS_4.Z());
	glEnd();

	// ライトオン
	glEnable(GL_LIGHTING);
	// 裏面描画
	glEnable(GL_CULL_FACE);
	// Zバッファへの書き込みを元に戻す
	glDepthMask(GL_TRUE);
	// アルファブレンド無効
	glDisable(GL_ALPHA);

	// 描画前の行列に戻す
	glPopMatrix();
}

void CSwitchShield::UpdateOff()
{
}

void CSwitchShield::UpdateOn()
{
	SetShow(false);
	SetEnableCol(false);
}

// コライダーを生成
void CSwitchShield::CreateCol()
{
	PhysicsMaterial material;
	material.mass = MASS;

	CVector halfExtents = HALF_EXTENTS * Scale();

	CPhysicsManager::Instance()->CreateBoxRigidBody(
		this,
		material,
		halfExtents,
		Position(),
		Rotation(),
		ELayer::eShield,
		{ ELayer::ePlayer,ELayer::eObject,ELayer::eConnectObj }
	);
}
