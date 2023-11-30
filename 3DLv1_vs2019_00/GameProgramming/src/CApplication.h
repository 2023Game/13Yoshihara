#pragma once
#include "CCharacter3.h"
#include "CTransform.h"
#include "CTriangle.h"
#include "CMatrix.h"
#include "CModel.h"
#include "CVector.h"
#include "CRectangle.h"
#include "CTexture.h"
#include "CCharacter.h"
#include "CBullet.h"
#include "CEnemy.h"
#include "CPlayer.h"
#include "CInput.h"
#include "CFont.h"
#include "CMiss.h"
#include <vector>
#include "CCharacterManager.h"
#include "CTaskManager.h"
#include "CCollisionManager.h"
#include "CBillBoard.h"
#include "CColliderTriangle.h"

#include "CGame.h"
#include "CSound.h"

class CApplication
{
private:
	//三角コライダの作成
	CColliderTriangle mColliderTriangle;
	CColliderTriangle mColliderTriangle2;
	//モデルビューの逆行列
	static CMatrix mModelViewInverse;
	CModel mModelC5;//C5モデル
	CPlayer mPlayer;
	//CCharacter3 mCharacter;
	CModel mBackGround;//背景モデル
	CModel mModel; //モデルクラスのインスタンス作成
	CVector mEye; //カメラの視点
	CSound mSoundBgm;	//BGM
	CSound mSoundOver;	//ゲームオーバー
	CGame* mpGame;
	static CCharacterManager mCharacterManager;
	//static CTaskManager mTaskManager;
	enum class EState
	{
		ESTART,	//ゲーム開始
		EPLAY,	//ゲーム中
		ECLEAR,	//ゲームクリア
		EOVER,	//ゲームオーバー
	};
	EState mState;
//	CCharacter mRectangle;
	CPlayer* mpPlayer;
	static CTexture mTexture;
	CEnemy* mpEnemy;
//	CBullet* mpBullet;
	CInput mInput;
	CFont mFont;
	CMiss* mpMiss;
	//CCharacterのポインタの可変長配列
//	std::vector<CCharacter*> mCharacters;
public:
	//モデルビュー行列の取得
	static const CMatrix& ModelViewInverse();
	//static CTaskManager* TaskManager();
	static CCharacterManager* CharacterManager();
	static CTexture* Texture();
	//最初に一度だけ実行するプログラム
	void Start();
	//繰り返し実行するプログラム
	void Update();
};