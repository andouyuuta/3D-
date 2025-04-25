#pragma once
#include <DxLib.h>
#include <vector>
#include "Enemy.h"

class EnemyManager
{
public:
	EnemyManager(void);	// コンストラクタ
	~EnemyManager(void); // デストラクタ

	void Init();    // 敵の初期化
	void Update();  // 敵の更新
	void Draw();    // 敵の描画
	void Release(); // 解放処理

	// プレイヤー側で敵のリストを取得するための関数
	const std::vector<Enemy*>& GetEnemies() const;

private:
	std::vector<Enemy*> enemyies_;			// 敵のリスト
	int enemymodel_;
	int bossenemymodel_;

	static constexpr int ENEMY_COUNT = 100;	// 敵の数
};