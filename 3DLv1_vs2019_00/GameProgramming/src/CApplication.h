#pragma once
#include "CCharacter3.h"
#include "CTransform.h"
#include "CTriangle.h"
#include "CMatrix.h"
#include "CModel.h"
#include "CVector.h"
#include "CTexture.h"
#include "CBullet.h"
#include "CEnemy.h"
#include "CEnemy3.h"
#include "CPlayer.h"
#include "CInput.h"
#include "CFont.h"
#include <vector>
#include "CTaskManager.h"
#include "CCollisionManager.h"
#include "CBillBoard.h"
#include "CColliderTriangle.h"
#include "CColliderMesh.h"
#include "CUi.h"
#include "CModelX.h"

#include "CSound.h"

class CApplication
{
private:
	CMatrix mMatrix;
	CModelX mModelX;
	static CUi* spUi;//Uiクラスのポインタ
	//モデルからコライダを生成
	CColliderMesh mColliderMesh;
	//モデルビューの逆行列
	static CMatrix mModelViewInverse;
	CModel mModelC5;//C5モデル
	CPlayer mPlayer;
	CModel mBackGround;//背景モデル
	CModel mModel; //モデルクラスのインスタンス作成
	CVector mEye; //カメラの視点
	CSound mSoundBgm;	//BGM
	CSound mSoundOver;	//ゲームオーバー
	//static CTaskManager mTaskManager;
	enum class EState
	{
		ESTART,	//ゲーム開始
		EPLAY,	//ゲーム中
		ECLEAR,	//ゲームクリア
		EOVER,	//ゲームオーバー
	};
	EState mState;
	CPlayer* mpPlayer;
	static CTexture mTexture;
	CEnemy* mpEnemy;
	CInput mInput;
	CFont mFont;
public:
	~CApplication();
	static CUi* Ui();//UIクラスのインスタンスを取得
	//モデルビュー行列の取得
	static const CMatrix& ModelViewInverse();
	static CTexture* Texture();
	//最初に一度だけ実行するプログラム
	void Start();
	//繰り返し実行するプログラム
	void Update();
};