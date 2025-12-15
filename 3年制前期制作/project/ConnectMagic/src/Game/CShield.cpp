#include "CShield.h"
#include "CColliderRectangle.h"
#include "CMaterial.h"
#include "CConnectPointManager.h"

// 頂点
const CVector VERT_POS_1 = CVector(-10.0f, 40.0f, 0.0f);
const CVector VERT_POS_2 = CVector(-10.0f, 0.0f,  0.0f);
const CVector VERT_POS_3 = CVector( 10.0f, 0.0f,  0.0f);
const CVector VERT_POS_4 = CVector( 10.0f, 40.0f, 0.0f);

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
	SAFE_DELETE(mpCol1);
	SAFE_DELETE(mpCol2);
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
	// 四角形コライダーを生成
	mpCol1 = new CColliderRectangle(
		this, ELayer::eWall,
		CVector(VERT_POS_1),
		CVector(VERT_POS_2),
		CVector(VERT_POS_3),
		CVector(VERT_POS_4),
		true
	);
	mpCol2 = new CColliderRectangle(
		this, ELayer::eWall,
		CVector(VERT_POS_4),
		CVector(VERT_POS_3),
		CVector(VERT_POS_2),
		CVector(VERT_POS_1),
		true
	);
	// 少しずらす
	mpCol1->Position(VectorZ());
	mpCol2->Position(-VectorZ());
	// オブジェクトとだけ衝突判定
	mpCol1->SetCollisionLayers({ ELayer::eObject });
	mpCol2->SetCollisionLayers({ ELayer::eObject });

	// 接続部の管理クラス
	CConnectPointManager* pointMgr = CConnectPointManager::Instance();
	// 衝突判定するコライダーに追加
	pointMgr->AddCollider(mpCol1);
	pointMgr->AddCollider(mpCol2);
}
