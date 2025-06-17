#include "CDamageUI3D.h"
#include "Maths.h"

// 座標のぶれ
#define POS_RAND 5.0f

// ダメージ表記の削除までの時間
#define DAMAGE_TEXT_DELETE_TIME 1.0f

// テキストの最小フォントサイズ
#define TEXT_FONT_SIZE_MIN 8
// テキストの最大フォントサイズ
#define TEXT_FONT_SIZE_MAX 32

// テキストサイズが最小になる距離
#define TEXT_FONT_SIZE_MIN_DIST 500.0f
// テキストサイズが最大になる距離
#define TEXT_FONT_SIZE_MAX_DIST 50.0f

// コンストラクタ
CDamageUI3D::CDamageUI3D(CVector pos)
	: CTextUI3D(RandPos(pos))
	, mAutoDeleteTime(DAMAGE_TEXT_DELETE_TIME)
{
}

// デストラクタ
CDamageUI3D::~CDamageUI3D()
{
}

// 更新
void CDamageUI3D::Update()
{

	CCamera* camera = CCamera::CurrentCamera();
	// 距離
	float dist = CVector::Distance(mWorldPos, camera->Position());
	// 距離によってサイズを求める
	float size = Math::Lerp(TEXT_FONT_SIZE_MAX, TEXT_FONT_SIZE_MIN, (dist - TEXT_FONT_SIZE_MAX_DIST) / (TEXT_FONT_SIZE_MIN_DIST - TEXT_FONT_SIZE_MAX_DIST));
	// フォントサイズを設定
	SetFontSize(size);

	CTextUI3D::Update();

	// 自動削除までの時間が経過したら
	mAutoDeleteTime -= Times::DeltaTime();
	if (mAutoDeleteTime < 0.0f)
	{
		// 削除
		Kill();
	}
}

// 座標をぶらして設定
CVector CDamageUI3D::RandPos(const CVector& pos)
{
	// ぶらした座標を設定
	CVector randPos = pos;
	randPos.X(randPos.X() + Math::Rand(-POS_RAND, POS_RAND));
	randPos.Y(randPos.Y() + Math::Rand(-POS_RAND, POS_RAND));
	randPos.Z(randPos.Z() + Math::Rand(-POS_RAND, POS_RAND));

	return randPos;
}