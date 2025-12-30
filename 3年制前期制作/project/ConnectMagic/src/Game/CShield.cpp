#include "CShield.h"
#include "CMaterial.h"
#include "CConnectPointManager.h"
#include "CPhysicsManager.h"
#include "CollisionData.h"
#include "PhysicsMaterial.h"

// 頂点
const CVector VERT_POS_1 =		CVector(-10.0f, 40.0f, 0.0f);
const CVector VERT_POS_2 =		CVector(-10.0f, 0.0f,  0.0f);
const CVector VERT_POS_3 =		CVector( 10.0f, 0.0f,  0.0f);
const CVector VERT_POS_4 =		CVector( 10.0f, 40.0f, 0.0f);

// 物理設定
constexpr float MASS =			0.0f;
const CVector HALF_EXTENTS =	CVector( 10.0f, 20.0f, 1.0f);

// コンストラクタ
CShield::CShield()
	: CObjectBase(ETag::eField, ETaskPriority::eShield, 0, ETaskPauseType::eGame)
{
	// コライダーを生成
	CreateCol();
}

// デストラクタ
CShield::~CShield()
{
}

// 描画
void CShield::Render()
{	
	// 現在の行列を退避しておく
	glPushMatrix();

	// 自身の行列を適用
	glMultMatrixf(Matrix().M());

	// アルファブレンドを有効にする
	glEnable(GL_BLEND);
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
	glVertex3f(VERT_POS_1.X(), VERT_POS_1.Y(), VERT_POS_1.Z());
	glVertex3f(VERT_POS_2.X(), VERT_POS_2.Y(), VERT_POS_2.Z());
	glVertex3f(VERT_POS_3.X(), VERT_POS_3.Y(), VERT_POS_3.Z());
	glVertex3f(VERT_POS_4.X(), VERT_POS_4.Y(), VERT_POS_4.Z());
	glEnd();

	// ライトオン
	glEnable(GL_LIGHTING);
	// 裏面描画
	glEnable(GL_CULL_FACE);
	// アルファブレンド無効
	glDisable(GL_ALPHA);

	// 描画前の行列に戻す
	glPopMatrix();
}

// コライダーを生成
void CShield::CreateCol()
{
	PhysicsMaterial material;
	material.mass = MASS;

	CPhysicsManager::Instance()->CreateBoxRigidBody(
		this,
		material,
		HALF_EXTENTS,
		Position(),
		Rotation(),
		ELayer::eShield,
		{ ELayer::eObject,ELayer::eConnectObj }
	);
}
