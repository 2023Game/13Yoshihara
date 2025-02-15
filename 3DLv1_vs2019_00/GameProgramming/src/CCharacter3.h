#ifndef CCHARACTER3_H
#define CCHARACTER3_H
//変換行列クラスのインクルード
#include "CTransform.h"
//モデルクラスのインクルード
#include "CModel.h"
#include "CTask.h"
class CCollider;
/*
キャラクタークラス
ゲームキャラクタの基本的な機能を定義する
*/
class CCharacter3 :public CTransform ,public CTask
{
public:
	//コンストラクタ
	CCharacter3(int priority);
	//衝突処理
	virtual void Collision(CCollider* m, CCollider* o) {}
	//コンストラクタ
	CCharacter3();
	//デストラクタ
	~CCharacter3();
	//モデルの設定
	//Model(CModel*m);
	void Model(CModel* m);
	//描画処理
	void Render();
	//キャラクタのタグ
	enum class ETag {
		EPLAYER,//プレイヤー
		EENEMY,//敵
	};
	ETag Tag();//タグの取得
protected:
	CModel* mpModel;//モデルのポインタ
	ETag mTag;//キャラクタのタグ
};
#endif