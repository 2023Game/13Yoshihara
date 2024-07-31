#include "CApplication.h"
#include "CCamera.h"
//OpenGL
#include "glut.h"


//クラスのstatic変数
CTexture CApplication::mTexture;
CMatrix CApplication::mModelViewInverse;

//背景モデルデータの指定
#define MODEL_BACKGROUND "res\\sky.obj","res\\sky.mtl"
#define MODEL_OBJ "res\\f14.obj","res\\f14.mtl" //モデルデータの指定
#define MODEL_C5 "res\\c5.obj","res\\c5.mtl"//敵輸送機モデル
#define SOUND_BGM "res\\mario.wav" //BGM音声ファイル
#define SOUND_OVER "res\\mdai.wav" //ゲームオーバー音声ファイル

CUi* CApplication::spUi = nullptr;

CUi* CApplication::Ui()
{
	return spUi;//インスタンスのポインタを返す
}

CApplication::~CApplication()
{
	delete spUi;//インスタンスUiの削除
}

const CMatrix& CApplication::ModelViewInverse()
{
	return mModelViewInverse;
}

CTexture* CApplication::Texture()
{
	return &mTexture;
}

void CApplication::Start()
{
	//カメラの設定
	mActionCamera.Set(5.0f, -15.0f, 180.0f);
	//3Dモデルファイルの読み込み
	mModelX.Load(MODEL_FILE);
	mKnight.Load(KNIGHT_MODEL);
	//アニメーションセットの分割
	mKnight.SeparateAnimationSet(0, 10, 80, "walk");//1:移動
	mKnight.SeparateAnimationSet(0, 1530, 1830, "idle1");//2:待機
	mKnight.SeparateAnimationSet(0, 10, 80, "walk");//3:ダミー
	mKnight.SeparateAnimationSet(0, 10, 80, "walk");//4:ダミー
	mKnight.SeparateAnimationSet(0, 10, 80, "walk");//5:ダミー
	mKnight.SeparateAnimationSet(0, 10, 80, "walk");//6:ダミー
	mKnight.SeparateAnimationSet(0, 440, 520, "attack1");//7:Attack1
	mKnight.SeparateAnimationSet(0, 520, 615, "attack2");//8:Attack2
	mKnight.SeparateAnimationSet(0, 10, 80, "walk");//9:ダミー
	mKnight.SeparateAnimationSet(0, 10, 80, "walk");//10:ダミー
	mKnight.SeparateAnimationSet(0, 1160, 1260, "death1");//11:ダウン
	//キャラクターにモデルを設定
	mXPlayer.Init(&mModelX);
	//敵の初期設定
	mXEnemy.Init(&mKnight);
	//敵の配置
	mXEnemy.Position(CVector(7.0f, 0.0f, 0.0f));
	//敵のアニメーションを待機に変更
	mXEnemy.ChangeAnimation(2, true, 200);
	//CPaladinの作成
	mpPaladin = new CPaladin();
	mpPaladin->Position(CVector(-1.0f, 0.0, 5.0f));
	mpPaladin->ChangeAnimation(1, true, 100);
	mFont.Load("FontG.png", 1, 4096 / 64);
}

void CApplication::Update()
{	
	//キャラクタクラスの更新
	mXPlayer.Update();
	//敵の更新
	mXEnemy.Update();
	//パラディンの更新
	mpPaladin->Update();

	//カメラ設定
	mActionCamera.Position(mXPlayer.Position()
		+ CVector(0.0f, 2.0f, 0.0f));
	mActionCamera.Update();
	mActionCamera.Render();

	//モデルビュー行列の取得
	glGetFloatv(GL_MODELVIEW_MATRIX, mModelViewInverse.M());
	//逆行列の取得
	mModelViewInverse = mModelViewInverse.Transpose();
	mModelViewInverse.M(0, 3, 0);
	mModelViewInverse.M(1, 3, 0);
	mModelViewInverse.M(2, 3, 0);

	//モデル描画
	mXPlayer.Render();
	//敵描画
	mXEnemy.Render();
	//パラディンの更新
	mpPaladin->Render();

	//コライダの描画
	CCollisionManager::Instance()->Render();
	//衝突処理
	CCollisionManager::Instance()->Collision();

	//2D描画開始
	CCamera::Start(0, 800, 0, 600);

	mFont.Draw(20, 20, 10, 12, "3D PROGRAMING");

	CVector screen;
	//Enemyの座標をスクリーン座標へ変換
	if (CActionCamera::Instance()->WorldToScreen(&screen, mXEnemy.Position()))
	{
		//変換先の座標に文字列を出力する
		mFont.Draw(screen.X(), screen.Y(), 7, 14, "ENEMY");
	}

	//2Dの描画終了
	CCamera::End();
}
