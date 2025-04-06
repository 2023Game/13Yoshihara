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
#define MIN_DESK_NUM 9

// 机の座標計算
#define DESK_POS(row,col) (CVector((col - DESK_COL / 2) * MAP_DIST_X, 0.0f,(row - DESK_ROW / 2) * MAP_DIST_Z))

// フィールド管理クラスのインスタンス
CFieldManager* CFieldManager::spInstance = nullptr;

// フィールド管理クラスのインスタンスを取得
CFieldManager* CFieldManager::Instance()
{
	return spInstance;
}

// コンストラクタ
CFieldManager::CFieldManager()
	: mSpawnColRow(0.0f,0.0f)
	, mDeskNum(-1)
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

// リスポーン地点座標を取得
CVector CFieldManager::GetSpawnPos()
{
	return DESK_POS(mSpawnColRow.Y(), mSpawnColRow.X());
}

// マップを生成
void CFieldManager::CreateField()
{
	// 周りに拡張可能な、机を生成する行列のリスト
	std::vector<CVector2> expandColRow;
	// 最後に拡張した行列
	CVector2 lastExpandColRow;

	// 1番最初の行の中央列を生成予定に設定
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
		// 方向をランダムに並び替え
		std::shuffle(directions.begin(), directions.end(), mt);

		// 拡張が成功したか
		bool isSuccess = false;

		for (const CVector2 dir : directions)
		{
			// 拡張先の行列
			int nextCol = col + dir.X();
			int nextRow = row + dir.Y();

			// リスト範囲内かつ、生成することになっていない
			if (InList(nextRow, nextCol) && mMap[nextRow][nextCol] == 0)
			{
				// 机の配置の行列を設定
				mMap[nextRow][nextCol] = 1;
				// 生成する行列に追加
				expandColRow.push_back(CVector2(nextCol, nextRow));
				// 拡張成功
				isSuccess = true;
				// 最後に拡張した行列
				lastExpandColRow = CVector2(nextCol, nextRow);
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

	for (int i = 0; i < DESK_ROW; i++)
	{
		for (int j = 0; j < DESK_COL; j++)
		{
			if (j == DESK_COL - 1)
			{
				printf("%i\n", mMap[i][j]);
			}
			else
			{
				printf("%i", mMap[i][j]);
			}
		}
	}

	// 最後に拡張した行列の机をスポーン地点とする
	mSpawnColRow.X(lastExpandColRow.X());
	mSpawnColRow.Y(lastExpandColRow.Y());

	// 机を生成
	CreateDesk(mSpawnColRow.Y(), mSpawnColRow.X());
	// 全ての繋げる壁を生成
	for (auto* desk : mpDesks)
	{
		desk->CreateConnectWall();
	}

	// 教室を生成
	mpClassRoom = new CClassRoom();
	// 教室の位置を調整
	mpClassRoom->Position(0.0f, -110.0f * mpDesks.back()->Scale().Y(), 0.0f);
}

// 机を生成
void CFieldManager::CreateDesk(int row, int col)
{
	// 作業中の机の番号を進める
	mDeskNum++;
	// 作業番号が最後のインデックス以下なら
	// 作業番号に要素数を設定する
	if (mDeskNum <= mpDesks.size() - 1)
		mDeskNum = mpDesks.size();
	// 机を生成
	mpDesks.push_back(new CDesk());
	// 座標を設定
	mpDesks[mDeskNum]->Position(DESK_POS(row, col));

	CVector pos = DESK_POS(row, col);
	printf("Desk%d:%f,%f,%f\n", mDeskNum, pos.X(),pos.Y(),pos.Z());

	// 生成済みに変更
	mMap[row][col] = 2;

	// 乱数シード値用
	std::random_device rnd;
	// 擬似乱数列生成器
	std::mt19937 mt(rnd());

	// 4方向（上、下、左、右）
	std::vector<CVector2> directions = { {0,-1},{0,1},{-1,0},{1,0} };
	// 方向をランダムに並び替え
	std::shuffle(directions.begin(), directions.end(), mt);

	for (const CVector2 dir : directions)
	{
		// 繋げる先の行列
		int connectCol = col + dir.X();
		int connectRow = row + dir.Y();

		// リスト内かつ生成予定なら繋げることが出来る。
		if (InList(connectRow, connectCol) && mMap[connectRow][connectCol] == 1)
		{
			// 上なら
			if (dir == CVector2(0, -1))
			{
				// 上につなげることができる
				mpDesks[mDeskNum]->SetIsConnectTop(true);
			}
			// 下なら
			else if (dir == CVector2(0, 1))
			{
				// 下につなげることができる
				mpDesks[mDeskNum]->SetIsConnectBottom(true);
			}
			// 左なら
			else if (dir == CVector2(-1, 0))
			{
				// 左につなげることができる
				mpDesks[mDeskNum]->SetIsConnectLeft(true);
			}
			// 右なら
			else
			{
				// 右につなげることができる
				mpDesks[mDeskNum]->SetIsConnectRight(true);
			}

			// 繋げた先の机を生成
			CreateDesk(connectRow, connectCol);
		}
	}

	// 作業中の番号を戻す
	mDeskNum--;
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
