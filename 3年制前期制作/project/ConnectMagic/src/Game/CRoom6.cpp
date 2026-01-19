#include "CRoom6.h"
#include "CSwitch.h"
#include "CCrystal.h"
#include "CSwitchMoveAirObj.h"
#include "CAirConnectObj.h"
#include "CSwitchPushWall.h"
#include "CWater.h"
#include "CSaveManager.h"
#include "CSwitchShield.h"
#include "CWeightGenerator.h"
#include "CSwitchWater.h"
#include "CWeight.h"

constexpr float ROOM_LENGTH = 200.0f;
// 回転
const CVector ROT_X_180 = CVector(180.0f, 0.0f,   0.0f);
const CVector ROT_X_90 =  CVector(90.0f,  0.0f,   0.0f);
const CVector ROT_Y_180 = CVector(0.0f,   180.0f, 0.0f);
const CVector ROT_Y_90 =  CVector(0.0f,   90.0f,  0.0f);

enum class ECrystalTarget
{
	eNone = -1,

	eGenerator,
	eShield,
	ePushWall,
	eAirObj,
};

// OBJの設定
struct ObjSetting
{
	CVector offsetPos;			// オフセット座標
	CVector rot = CVector::zero;// 回転
	bool isBig = false;	// 大きいか
};

// おもり
const CVector WEIGHT_POS =	CVector(-25.0f,	0.0f, -175.0f);

// 生成器
const std::vector<ObjSetting> GENERATOR_SETTING =
{
	{CVector(-30.0f, 20.0f,	-100.0f)},
	{CVector(30.0f,	 20.0f,	-100.0f), ROT_Y_180,}
};

// シールド
const std::vector<ObjSetting> SHIELD_SETTING =
{
	// グループ1
	{CVector(-175.0f, 0.0f,	-60.0f), CVector::zero,	true},
	{CVector(-175.0f, 0.0f,	-90.0f), CVector::zero,	true},
	{CVector(-150.0f, 0.0f,	-75.0f), ROT_Y_90},
	// グループ2
	{CVector(-175.0f, 0.0f,	-110.0f),CVector::zero, true},
	{CVector(-175.0f, 0.0f,	-140.0f),CVector::zero, true},
	{CVector(-150.0f, 0.0f,	-125.0f),ROT_Y_90}
};
const CVector SHIELD_SCALE_BIG =	CVector(2.5f, 1.0f, 1.0f);
const CVector SHIELD_SCALE_SMALL =  CVector(1.5f, 1.0f, 1.0f);

// プレイヤーを押す壁
const std::vector<CVector> PUSH_WALL_OFFSET_POS =
{
	CVector(224.0f,	 0.0f,	-75.0f),
	CVector(224.0f,	 0.0f,	-125.0f),
};
const CVector PUSH_WALL_SCALE =		CVector( 5.0f,  4.0f, 3.0f);
const CVector PUSH_WALL_MOVE_VEC =	CVector(-50.0f, 0.0f, 0.0f);
constexpr float PUSH_WALL_MOVE_TIME = 1.0f;

// 空中オブジェクト
const std::vector<std::vector<CVector>> AIR_OBJ_MOVE_TARGET_POS =
{
	{
		CVector(-67.5f,	 35.0f, -25.0f),
		CVector(-145.0f, 35.0f, -25.0f),
	},
	{
		CVector( 67.5f,	 35.0f, -25.0f),
		CVector( 145.0f, 35.0f, -25.0f),
	}
};

// スイッチ
const std::vector<CVector> SWITCH_OFFSET_POS =
{
	CVector(-30.0f, 0.0f, -100.0f),
	CVector(30.0f,  0.0f, -100.0f),
};

// クリスタル設定
struct CrystalSetting
{
	ObjSetting objSetting;	// OBJ設定
	ECrystalTarget target;	// 作用するターゲット
	bool isOn = false;		// 有効か
};

// クリスタル
const std::vector<CrystalSetting> CRYSTAL_SETTING =
{
	// 作用相手なし
	{ {CVector(-30.0f,  0.0f,  -35.0f),  CVector::zero, false},
	ECrystalTarget::eNone, true},
	{ {CVector(30.0f,	0.0f,  -35.0f),  CVector::zero, false},
	ECrystalTarget::eNone, true},
	{ {CVector(-155.0f, 0.0f,  -35.0f),  CVector::zero, false},
	ECrystalTarget::eNone, true},
	{ {CVector( 155.0f, 0.0f,  -35.0f),  CVector::zero, false},
	ECrystalTarget::eNone, true},
	// 空中オブジェクト用
	{ {CVector(-67.5f,  40.0f, -40.0f),  ROT_X_180},
	ECrystalTarget::eAirObj},
	{ {CVector(67.5f,	40.0f, -40.0f),  ROT_X_180},
	ECrystalTarget::eAirObj},
	// シールドグループ1用
	{ {CVector(-140.0f, 40.0f, -75.0f),  ROT_X_180},
	ECrystalTarget::eShield},
	// シールドグループ2用
	{ {CVector(-140.0f, 40.0f, -125.0f), ROT_X_180},
	ECrystalTarget::eShield},
	// プレイヤーを押す壁用
	{ {CVector(140.0f,  40.0f, -75.0f),  ROT_X_180},
	ECrystalTarget::ePushWall},
	{ {CVector( 140.0f, 40.0f, -125.0f), ROT_X_180},
	ECrystalTarget::ePushWall},
	// おもり生成器用
	{ {CVector(-175.0f, 0.0f,  -175.0f), CVector::zero, true},
	ECrystalTarget::eGenerator},
	{ {CVector( 175.0f, 0.0f,  -175.0f), CVector::zero, true},
	ECrystalTarget::eGenerator},
};
const CVector CRYSTAL_SCALE_BIG = CVector::one * 2.0f;

// 水
const CVector WATER_OFFSET_POS = CVector(0.0f,  -15.0f, -100.0f);
const CVector WATER_MOVE_POS =	 CVector(0.0f,  -45.0f, -100.0f);
const CVector WATER_SCALE =		 CVector(200.0f, 0.0f,   100.0f);


CRoom6::CRoom6(const CVector& pos)
	: CRoomBase(ROOM_LENGTH)
{
	mpModel = CResourceManager::Get<CModel>("Room6");
	Position(pos);

	CreateCol("Room6_Col");

	CreateFieldObjects();
}

CRoom6::~CRoom6()
{
}

void CRoom6::SetEnableRoom(bool enable)
{
	CRoomBase::SetEnableRoom(enable);
	// スイッチ
	for (CSwitch* s : mSwitches)
	{
		s->SetEnableSwitch(enable);
	}
	// クリスタル
	for (CSwitch* c : mCrystals)
	{
		c->SetEnableSwitch(enable);
	}

	// 保存管理クラス
	CSaveManager* saveMgr = CSaveManager::Instance();

	auto process = [&](CSavable* obj)
	{
		// 有効時は保存オブジェクトに追加
		if (enable)
		{
			saveMgr->AddSavableObj(obj);
		}
		// 無効時は保存オブジェクトから削除
		else
		{
			saveMgr->DeleteSavableObj(obj);
		}
	};

	for (CSavable* obj : mGenerators)		 process(obj);
	for (CSavable* obj : mShields)			 process(obj);
	for (CSavable* obj : mCrystals)			 process(obj);
	for (CSavable* obj : mAirObjs) process(obj);
	if (mpWater) process(mpWater);
}

void CRoom6::CreateFieldObjects()
{
	// 重りを生成
	mpWeight = new CWeight(Position() + WEIGHT_POS);
	// リストへ追加
	mObjs.push_back(mpWeight);

	// 重り生成器を生成
	for (const ObjSetting& setting : GENERATOR_SETTING)
	{
		CWeightGenerator* newWeightGenerator =
			new CWeightGenerator(Position() + setting.offsetPos);
		// 回転
		newWeightGenerator->Rotate(setting.rot);
		// リストへ追加
		mGenerators.push_back(newWeightGenerator);
		mObjs.push_back(newWeightGenerator);
	}

	// シールドを生成
	for (const ObjSetting& setting : SHIELD_SETTING)
	{
		CSwitchShield* newSwitchShield = nullptr;
		// 大きい
		if (setting.isBig)
		{
			newSwitchShield = new CSwitchShield(SHIELD_SCALE_BIG);
		}
		// 小さい
		else
		{
			newSwitchShield = new CSwitchShield(SHIELD_SCALE_SMALL);
		}
		// 座標
		newSwitchShield->Position(Position() + setting.offsetPos);
		// 回転
		newSwitchShield->Rotate(setting.rot);
		// リストへ追加
		mShields.push_back(newSwitchShield);
		mObjs.push_back(newSwitchShield);
	}

	CModel* model = CResourceManager::Get<CModel>("MoveObject");
	// プレイヤーを押す壁を生成
	for (const CVector& offsetPos : PUSH_WALL_OFFSET_POS)
	{
		CSwitchPushWall* newSwitchPushWall =
			new CSwitchPushWall(model, model, Position() + offsetPos,
				PUSH_WALL_SCALE, PUSH_WALL_MOVE_VEC, PUSH_WALL_MOVE_TIME);
		// リストへ追加
		mPushWalls.push_back(newSwitchPushWall);
		mObjs.push_back(newSwitchPushWall);
	}

	// 空中オブジェクトを生成
	for (const std::vector<CVector> moveTargets : AIR_OBJ_MOVE_TARGET_POS)
	{
		CSwitchMoveAirObj* newSwitchMoveAirObj =
			new CSwitchMoveAirObj(Position() + moveTargets[0]);
		// すべての移動先を追加
		for (const CVector& offsetPos : moveTargets)
		{
			newSwitchMoveAirObj->AddTargetPos(Position() + offsetPos);
		}
		// 方向調整
		newSwitchMoveAirObj->GetAirConnectObj()->Rotate(-ROT_Y_90);
		// リストへ追加
		mAirObjs.push_back(newSwitchMoveAirObj);
		mObjs.push_back(newSwitchMoveAirObj);
	}

	// スイッチで水位が下がる水を生成
	mpWater = new CSwitchWater(Position() + WATER_OFFSET_POS, Position() + WATER_MOVE_POS, WATER_SCALE);
	// リストへ追加
	mObjs.push_back(mpWater);

	// スイッチを生成
	for (const CVector& offsetPos : SWITCH_OFFSET_POS)
	{
		CSwitch* newSwitch = new CSwitch(Position() + offsetPos, CVector::one, true);
		// 水を作用するオブジェクトに設定
		newSwitch->SetActionObj(mpWater);
		// リストへ追加
		mSwitches.push_back(newSwitch);
	}
	// 水に作用するスイッチに設定
	mpWater->SetSwitches(mSwitches);

	// それぞれのオブジェクトのスイッチを設定した数
	int airObjNum = 0;
	int shieldNum = 0;
	int pushWallNum = 0;
	int generatorNum = 0;
	// クリスタルを生成
	for (const CrystalSetting& setting : CRYSTAL_SETTING)
	{
		// 大きいならサイズを変更
		CVector scale = CVector::one;
		if (setting.objSetting.isBig)
			scale = CRYSTAL_SCALE_BIG;

		CSwitch* newCrystal = 
			new CSwitch(Position() + setting.objSetting.offsetPos,
				scale, 
				false, ESwitchType::eBatteries);
		// 回転
		newCrystal->GetCrystal()->Rotate(setting.objSetting.rot);
		// オンオフを設定
		newCrystal->SetOnOff(setting.isOn);

		// 作用相手の設定
		switch (setting.target)
		{
			// 空中オブジェクト
		case ECrystalTarget::eAirObj:
			if (airObjNum < mAirObjs.size())
				newCrystal->SetActionObj(mAirObjs[airObjNum++]);
			break;
			// シールド
		case ECrystalTarget::eShield:
			for (int i = 0; i < 3; ++i)
			{
				if (shieldNum < mShields.size())
					newCrystal->SetActionObj(mShields[shieldNum++]);
			}
			break;
			// プレイヤーを押す壁
		case ECrystalTarget::ePushWall:
			if (pushWallNum < mPushWalls.size())
				newCrystal->SetActionObj(mPushWalls[pushWallNum++]);
			break;
			// おもり生成器
		case ECrystalTarget::eGenerator:
			if (generatorNum < mGenerators.size())
				newCrystal->SetActionObj(mGenerators[generatorNum++]);
			break;
		}
		// リストへ追加
		mCrystals.push_back(newCrystal);
	}
}
