#pragma once
#include "SpellType.h"

class CSpellBase;

// 実際に呪文を発動する呪文詠唱クラス
class CSpellCaster
{
public:
	// 対戦相手を設定
	void SetOpponent(CObjectBase* opponent);
	// 対戦相手を取得
	CObjectBase* GetOpponent() const;

	// 詠唱中かを取得
	bool IsCasting() const;

	// 詠唱開始
	bool CastStart(ESpellElementalType elemental,
		ESpellShapeType shape);

protected:
	// コンストラクタ
	CSpellCaster(CObjectBase* owner);
	// デストラクタ
	~CSpellCaster();
	
	// 更新
	void Update();

	// 対戦相手
	CObjectBase* mpOpponent;

private:
	// ボール呪文の詠唱
	void CastBall();
	// ボルト呪文の詠唱
	void CastBolt();
	// ブレス呪文の詠唱
	void CastBreath();
	// テレポート呪文の詠唱
	void CastTeleport();
	// シールド呪文の詠唱
	void CastShield();
	// リフレクター呪文の詠唱
	void CastReflector();

	// 生成速度と待機時間の設定
	void SetTime(ESpellElementalType elemental, ESpellShapeType shape);

	// 生成待機
	bool WaitGenerate();
	// 発射待機
	bool WaitShoot();
	// 発射
	bool Shoot();

	// 詠唱中の呪文の属性
	ESpellElementalType mSpellElemental;
	// 詠唱中の呪文の形
	ESpellShapeType mSpellShape;

	// 詠唱中か
	bool mIsSpellCasting;
	// ステップ
	int mStep;
	// 経過時間
	float mElapsedTime;

	// 生成間隔
	float mGenerateInterval;
	// 待機時間
	float mIdleTime;

	// 詠唱した呪文
	std::vector<CSpellBase*> mSpells;

	// 生成回数
	int mGenerateNum;

	// このクラスの持ち主
	CObjectBase* mpOwner;
};