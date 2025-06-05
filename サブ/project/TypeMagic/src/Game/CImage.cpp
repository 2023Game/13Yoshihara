#include "CImage.h"
#include <cmath>

//コンストラクタ
CImage::CImage(const char* path, ETaskPriority prio, int sortOrder,
	ETaskPauseType pause, bool dontDelete, bool addTaskList)
	: CUIBase(prio, sortOrder, pause, dontDelete, addTaskList)
	, mpTexture(nullptr)
	, mUV(0.0f, 0.0f, 1.0f, 1.0f)
	, mIsRot(false)
	, mRotSpeed(0.0f)
	, mAngle(0.0f)
{
	mPosition = CVector2(0.0f, 0.0f);
	mSize = CVector2(128.0f, 128.0f);
	mColor = CColor(1.0f, 1.0f, 1.0f, 1.0f);

	Load(path, dontDelete);
}

//デストラクタ
CImage::~CImage()
{
}

//テクスチャの読み込み
void CImage::Load(const char* path, bool dontDelete)
{
	mpTexture = CResourceManager::Load<CTexture>(path, path, dontDelete);
	//読み込み成功
	if (mpTexture != nullptr)
	{
		//表示サイズをテクスチャのサイズで初期化
		const STgamHeader& header = mpTexture->Header();
		SetSize(header.width, header.height);
	}
}

//表示する範囲を設定（CVector4版）
void CImage::SetUV(const CVector4& uv)
{
	mUV = uv;
}

//表示する範囲を設定
void CImage::SetUV(const float& left, const float& top, const float& right, const float& bottom)
{
	SetUV(CVector4(left, top, right, bottom));
}

//表示する範囲を取得
const CVector4& CImage::GetUV() const
{
	return mUV;
}

// 回転するかを設定
void CImage::SetRot(bool isRoll)
{
	mIsRot = isRoll;
}

// 回転速度を設定
void CImage::SetRotSpeed(float rotSpeed)
{
	mRotSpeed = rotSpeed;
}

//描画
void CImage::Render()
{
	//テクスチャが読み込まれてなければ、描画処理は行わない
	if (mpTexture == nullptr) return;

	glPushMatrix();

	glLoadIdentity();

	// 回転するなら
	if (mIsRot)
	{
		// 回転の中心
		float posX = mPosition.X() + mSize.X() / 2;
		float posY = mPosition.Y() + mSize.Y() / 2;
		// 回転角度
		mAngle += mRotSpeed * Times::DeltaTime();
		// 1000度を超えたら
		if (std::abs(mAngle) > 1000.0f)
		{
			// 360度以内に戻す
			mAngle = std::fmod(mAngle, 360.0f);
		}
		// 画像の中心へ移動
		glTranslated(posX, posY, 0.0f);
		// Z軸で回転
		glRotated(mAngle, 0.0f, 0.0f, 1.0f);
		// 元の位置へ戻す
		glTranslated(-posX, -posY, 0.0f);
	}

	//テクスチャを有効にする
	glEnable(GL_TEXTURE_2D);
	//アルファブレンドを有効にする
	glEnable(GL_BLEND);
	//ブレンド方法を指定
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//テクスチャを指定
	glBindTexture(GL_TEXTURE_2D, mpTexture->Id());

	glBegin(GL_TRIANGLES);
	// 表示カラーを設定
	glColor4fv((GLfloat*)&mColor);

	float uvLeft = mUV.X();
	float uvTop = mUV.Y();
	float uvRight = mUV.Z();
	float uvBottom = mUV.W();

	float vtxLeft = mPosition.X() - mCenter.X();
	float vtxRight = vtxLeft + mSize.X();
	float vtxBottom = mPosition.Y() - mCenter.Y();
	float vtxTop = vtxBottom + mSize.Y();

	glTexCoord2f(uvLeft, uvTop);
	glVertex2d(vtxLeft, vtxTop);

	glTexCoord2f(uvLeft, uvBottom);
	glVertex2d(vtxLeft, vtxBottom);

	glTexCoord2f(uvRight, uvBottom);
	glVertex2d(vtxRight, vtxBottom);

	glTexCoord2f(uvLeft, uvTop);
	glVertex2d(vtxLeft, vtxTop);

	glTexCoord2f(uvRight, uvBottom);
	glVertex2d(vtxRight, vtxBottom);

	glTexCoord2f(uvRight, uvTop);
	glVertex2d(vtxRight, vtxTop);

	glEnd();

	//テクスチャを解放
	glBindTexture(GL_TEXTURE_2D, 0);
	//アルファブレンドを無効
	glDisable(GL_BLEND);
	//テクスチャを無効
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}
