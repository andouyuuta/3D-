#include "EnemyManager.h"
#include "../Application.h"

// コンストラクタ
EnemyManager::EnemyManager(void)
	:enemymodel_(-1), bossenemymodel_(-1)
{
}

// デストラクタ
EnemyManager::~EnemyManager(void)
{
}

// 初期化
void EnemyManager::Init(void)
{
	enemyies_.clear();

	// 敵のモデルの読み込み
	enemymodel_ = MV1LoadModel((Application::PATH_MODEL + "EnemyModel.mv1").c_str());
	bossenemymodel_ = MV1LoadModel((Application::PATH_MODEL + "BossEnemyModel.mv1").c_str());

	//複数の敵の初期化(今は10)
	for (int i = 0; i <= ENEMY_COUNT; i++)
	{
		Enemy* enemy = new Enemy();
		enemy->Init(enemymodel_);

		// ランダム座標に配置
		VECTOR pos = { GetRand(500) - 250.0f,0.0f,GetRand(500) - 250.0f };
		enemy->SetEnemyPos(pos);

		enemyies_.push_back(enemy);
	}
}

// 更新
void EnemyManager::Update(void)
{
	//複数の敵の更新
	for (Enemy* enemy : enemyies_)
	{
		if (enemy != nullptr)
		{
			enemy->Update();
		}
	}
}

// 描画
void EnemyManager::Draw(void)
{
	//複数の敵の描画
	for (Enemy* enemy : enemyies_)
	{
		if (enemy != nullptr)
		{
			enemy->Draw();
		}
	}

}

// 解放
void EnemyManager::Release(void)
{
	//複数の敵の解放
	for (Enemy* enemy : enemyies_)
	{

		if (enemy != nullptr)
		{
			enemy->Release();
			delete enemy;
		}
	}
	enemyies_.clear();

	// モデル自体の解放
	if (enemymodel_ != -1)
	{
		MV1DeleteModel(enemymodel_);
		enemymodel_ = -1;
	}

	// ボス
	if (bossenemymodel_ != -1)
	{
		MV1DeleteModel(bossenemymodel_);
		bossenemymodel_ = -1;
	}
}

// プレイヤー側で敵のリストを取得するための関数
const std::vector<Enemy*>& EnemyManager::GetEnemies() const
{
	return enemyies_;
}
