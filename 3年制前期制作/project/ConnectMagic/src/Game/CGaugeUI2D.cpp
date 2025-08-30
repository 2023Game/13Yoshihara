#include "CGaugeUI2D.h"
#include "CImage.h"
#include "Maths.h"


// コンストラクタ
CGaugeUI2D::CGaugeUI2D(CObjectBase* owner, std::string gaugePath, bool addTaskList)
	: CObjectBase(ETag::eUI, ETaskPriority::eUI, 0, ETaskPauseType::eGame, false, addTaskList)
	, mpOwner(owner)
	, mpGaugeImg(nullptr)
	, mpWhiteImg(nullptr)
	, mMaxPoint(100)
	, mCurrPoint(mMaxPoint)
	, mPercent(1.0f)
{
	// ゲージのイメージを読み込み
	mpGaugeImg = new CImage
	(
		gaugePath.c_str(),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mGaugeSize = mpGaugeImg->GetSize();

	// 白イメージを読み込み
	mpWhiteImg = new CImage
	(
		"UI\\white.png",
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
	CVector2 center = CVector2(-mGaugeSize.X() * 0.5f, 0.0f);
	mpWhiteImg->SetCenter(center);
}

// デストラクタ
CGaugeUI2D::~CGaugeUI2D()
{
	// 読み込んだイメージを削除
	SAFE_DELETE(mpGaugeImg);
	SAFE_DELETE(mpWhiteImg);

	// 持ち主に削除されたことを伝える
	if (mpOwner != nullptr)
	{
		mpOwner->DeleteObject(this);
	}
}

// 持ち主を設定
void CGaugeUI2D::SetOwner(CObjectBase* owner)
{
	mpOwner = owner;
}

// ポイント残量を反映
void CGaugeUI2D::ApplyPoint()
{
	// 最大値が不正値でなければ
	if (mMaxPoint > 0)
	{
		// 現在値が最大値の何パーセントか求める
		mPercent = Math::Clamp01((float)mCurrPoint / mMaxPoint);
	}
	// 不正値ならば、100%固定
	else
	{
		mPercent = 1.0f;
	}
}

// 最大値を設定
void CGaugeUI2D::SetMaxPoint(int point)
{
	mMaxPoint = point;
	ApplyPoint();
}

// 現在値を設定
void CGaugeUI2D::SetCurrPoint(int point)
{
	mCurrPoint = point;
	ApplyPoint();
}

// ポイント残量の割合を設定（0.0～1.0）
void CGaugeUI2D::SetPercent(float per)
{
	mPercent = Math::Clamp01(per);
}

// 更新
void CGaugeUI2D::Update()
{
	mpGaugeImg->Update();
	mpWhiteImg->Update();
}

// 描画
void CGaugeUI2D::Render()
{
	// 行列の保存
	glPushMatrix();

	CVector2 pos = Position();
	{
		// ゲージ背景を描画
		CVector2 barPos = pos;
		barPos.X(barPos.X() - mGaugeSize.X() * 0.5f);
		mpWhiteImg->SetPos(barPos);
		mpWhiteImg->SetSize(mGaugeSize);
		mpWhiteImg->SetColor(CColor::black);
		mpWhiteImg->Render();
	}


	// バーのサイズ、座標、色を
	// ポイント残量の割合に合わせて調整して、バーを描画
	{
		// バーのサイズを調整
		CVector2 barSize = mGaugeSize;
		barSize.X(barSize.X() * mPercent);
		mpWhiteImg->SetSize(barSize);
		// バーの座標を調整
		CVector2 barPos = pos;
		barPos.X(barPos.X() - mGaugeSize.X() * 0.5f);
			//mGaugeSize - barSize;
		mpWhiteImg->SetPos(barPos);
		//CVector2 barPos = pos;
		//barPos.X(barPos.X() - mGaugeSize.X() * 0.5f);
		//mpWhiteImg->SetPos(barPos);
		// バーの色を設定
		CColor barColor = CColor::green;
		if (mPercent <= 0.2f) barColor = CColor::red;
		else if (mPercent <= 0.5f)barColor = CColor::yellow;
		mpWhiteImg->SetColor(barColor);
		// バーを描画
		mpWhiteImg->Render();
	}

	// ゲージ本体を描画
	mpGaugeImg->SetSize(mGaugeSize);
	mpGaugeImg->Render();
	mpGaugeImg->SetPos(pos);

	// 行列を戻す
	glPopMatrix();
}

// サイズを設定
void CGaugeUI2D::Size(float rate)
{
	mGaugeSize = mGaugeSize * rate;
	CVector2 center = CVector2(-mGaugeSize.X() * 0.5f, 0.0f);
	mpWhiteImg->SetCenter(center);
}
