#include "CSwitchShield.h"
#include "CColliderRectangle.h"
#include "CMaterial.h"
#include "CConnectPointManager.h"

// 頂点
#define VERT_POS_1 -10.0f,	40.0f,	0.0f
#define VERT_POS_2 -10.0f,	0.0f,	0.0f
#define VERT_POS_3  10.0f,	0.0f,	0.0f
#define VERT_POS_4  10.0f,	40.0f,	0.0f

// コンストラクタ
CSwitchShield::CSwitchShield(CVector scale)
	: CSwitchObject(ETaskPriority::eShield)
{
	// コライダーを生成
	CreateCol();

	Scale(scale);
}

// デストラクタ
CSwitchShield::~CSwitchShield()
{
	SAFE_DELETE(mpCol1);
	SAFE_DELETE(mpCol2);
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
	glVertex3f(VERT_POS_1);
	glVertex3f(VERT_POS_2);
	glVertex3f(VERT_POS_3);
	glVertex3f(VERT_POS_4);
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
	// オブジェクトとプレイヤーだけ衝突判定
	mpCol1->SetCollisionLayers({ ELayer::eObject, ELayer::ePlayer });
	mpCol2->SetCollisionLayers({ ELayer::eObject, ELayer::ePlayer });

	// 接続部の管理クラス
	CConnectPointManager* pointMgr = CConnectPointManager::Instance();
	// 衝突判定するコライダーに追加
	pointMgr->AddCollider(mpCol1);
	pointMgr->AddCollider(mpCol2);
}
