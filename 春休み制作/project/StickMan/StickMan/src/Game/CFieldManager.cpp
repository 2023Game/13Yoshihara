#include "CFieldManager.h"
#include "CDesk.h"
#include "CClassRoom.h"
#include "Maths.h"
#include <algorithm>
#include <random>

// マップ同士の間隔
#define MAP_DIST_X 185.0f
#define MAP_DIST_Z 245.0f

// 机の高さ分教室を下げる
#define DESK_HEIGHT 110.0f

// 机の最低数
#define MIN_DESK_NUM 5

// フィールド管理クラスのインスタンス
CFieldManager* CFieldManager::spInstance = nullptr;

// フィールド管理クラスのインスタンスを取得
CFieldManager* CFieldManager::Instance()
{
	return spInstance;
}

// コンストラクタ
CFieldManager::CFieldManager()
{
	spInstance = this;

	// マップを生成
	CreateField();
}

// デストラクタ
CFieldManager::~CFieldManager()
{
	SAFE_DELETE(spInstance);
}

// 地面のコライダ―取得
CCollider* CFieldManager::GetGroundCol()
{
	return nullptr;
}

// 壁のコライダ―取得
CCollider* CFieldManager::GetWallCol()
{
	return nullptr;
}

// オブジェクトのコライダ―取得
CCollider* CFieldManager::GetObjCol()
{
	return nullptr;
}

// マップを生成
void CFieldManager::CreateField()
{
	// 周りに拡張可能な、机を生成する行列のリスト
	std::vector<CVector2> expandColRow;

	// 1番最初の行の中央列を1に設定
	int row = 0;
	int col = (int)(DESK_COL / 2);
	mMap[row][col] = 1;

	// 生成する行列に追加
	expandColRow.push_back(CVector2(col, row));

	// 最低数以上になるまで繰り返す
	while (expandColRow.size() < MIN_DESK_NUM)
	{
		// 生成する位置の中から拡張するときの基準をランダム決定
		int index = Math::Rand(0, expandColRow.size() - 1);
		CVector2 colRow = expandColRow[index];
		col = colRow.X();	// 列を取得
		row = colRow.Y();	// 行を取得

		// 乱数シード値用
		std::random_device rnd;
		// 擬似乱数列生成器
		std::mt19937 mt(rnd());

		// 4方向（上、下、左、右）
		std::vector<CVector2> directions = { {0,-1},{0,1},{-1,0},{1,0} };
		// 要素をランダムに並び替え
		std::shuffle(directions.begin(), directions.end(), mt);

		// 拡張が成功したか
		bool isSuccess = false;

		for (const CVector2 dir : directions)
		{
			// 拡張先の行列
			int nextCol = col - dir.X();
			int nextRow = row - dir.Y();

			// リスト範囲内かつ、生成することになっていない
			if (InList(nextRow, nextCol) && mMap[nextRow][nextCol] == 0)
			{
				// 机の配置の行列を設定
				mMap[nextRow][nextCol] = 1;
				// 生成する行列に追加
				expandColRow.push_back(CVector2(nextRow, nextCol));
				// 拡張成功
				isSuccess = true;
				break;
			}
		}

		// 拡張を失敗していたら
		if (!isSuccess)
		{
			// 候補から消す
			expandColRow.erase(expandColRow.begin() + index);
		}
	}

	// 机のスケール
	float deskScale = 1.0f;
	// マップ同士が繋がれるかどうか
	bool isTop = true;
	bool isBottom = true;
	bool isLeft = true;
	bool isRight = true;
	// 机を生成
	for (int i = 0; i < DESK_ROW; i++)
	{
		for (int j = 0; j < DESK_COL; j++)
		{
			// 生成しないなら次へ
			if (mMap[i][j] == 0) continue;
			// 机を生成
			mpDeskList.push_back(new CDesk());
			// 座標を設定
			mpDeskList.back()->Position(
				(j - DESK_COL / 2) * MAP_DIST_X,
				0.0f,
				(i - DESK_ROW / 2) * MAP_DIST_Z);
			// 机のスケールを取得
			deskScale = mpDeskList.back()->Scale().Y();
		}
	}

	//// 机同士のつながりを設定して壁を生成
	//for (int i = 0; i < DESK_ROW; i++)
	//{
	//	// 一番上の行なら上へは繋がらない
	//	if (i == 0)
	//	{
	//		isTop = false;
	//		isBottom = true;
	//	}
	//	// 一番下の行なら下へは繋がらない
	//	else if (i == DESK_ROW - 1)
	//	{
	//		isTop = true;
	//		isBottom = false;
	//	}
	//	// それ以外は繋がる
	//	else
	//	{
	//		isTop = true;
	//		isBottom = true;
	//	}
	//	for (int j = 0; j < DESK_COL; j++)
	//	{
	//		// 一番左の列なら左へは繋がらない
	//		if (j == 0)
	//		{
	//			isLeft = false;
	//			isRight = true;
	//		}
	//		// 一番右の列なら右へは繋がらない
	//		else if (j == DESK_COL - 1)
	//		{
	//			isLeft = true;
	//			isRight = false;
	//		}
	//		// それ以外は繋がる
	//		else
	//		{
	//			isLeft = true;
	//			isRight = true;
	//		}
	//		// 生成されていない場所なら次へ
	//		if (mMap[i][j] == 0) continue;
	//		// つながりを設定
	//		mpDeskList[i][j]->SetIsConnect(isTop, isBottom, isLeft, isRight);
	//		// 繋ぐ壁を生成
	//		mpDeskList[i][j]->CreateConnectWall();
	//	}
	//}
	// 教室を生成
	mpClassRoom = new CClassRoom();
	// 教室の位置を調整
	mpClassRoom->Position(0.0f, -110.0f * deskScale, 0.0f);
}

// リストの範囲内か
bool CFieldManager::InList(int row, int col)
{
	if (row >= 0 &&
		row < DESK_ROW &&
		col >= 0 &&
		col < DESK_COL)
	{
		return true;
	}

	return false;
}
