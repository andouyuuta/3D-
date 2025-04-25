#pragma once
#include <DxLib.h>
#include <vector>

class Enemy
{
public:
	// 移動量
	static constexpr float MOVE_SPEED_WALK = 2.5f;		// 歩き
	static constexpr float MOVE_SPEED_RUN = 8.0f;		// 走る
	static constexpr float MOVE_SPEED_STOP = 0.0f;

	// 初期モデル補正角度
	static constexpr VECTOR INIT_MODEL_ROT_OFFSET = { 0.0f,DX_PI_F / 2.0f,0.0f };

	// 向いている方向
	static constexpr VECTOR INIT_DIR = { 0.0f,0.0f,-1.0f };

	// 視野角
	static constexpr float VIEW_ANGLE = 30.0f;

	// アニメーションの再生速度
	static constexpr float ANIM_SPEED = 0.5f;		// 生きている時
	static constexpr float DEAD_ANIM_SPEED = 0.2f;	// 死んでいる時

	// 敵の移動状態管理用
	enum class MODE_ID
	{
		NONE,
		WALK,		// 歩き
		RUN,		// 走る
	};

	// 敵の最大HP
	static constexpr int ENEMY_MAX_HP = 10;

	// 敵の情報の構造体
	struct Info
	{
		int modelid_;			//モデルID

		VECTOR scale_;			//大きさ
		VECTOR pos_;			//座標
		VECTOR rot_;			//回転値
		float moveSpeed_;		//移動スピード
		int moveDir_;			//移動方向
		int moveKind_;			//プレイヤーの移動状態
		int hp_;				// HP

		// 移動ベクトル
		VECTOR moveVec_;		//移動ベクトル
		VECTOR oldMoveVec_;		//直前の移動ベクトル
		VECTOR moveVEcRad_;		//移動ベクトルが生成する角度

		VECTOR localRot_;		//調整用初期角度

		//アニメーション
		int animindex_;			//アニメーションの種類
		int  animAttachNo_;		//設定するアニメーションの番号
		float animTotalTime_;	//アニメーションの総再生時間
		float currentAnimTime_; //直前のアニメーションの時間
		bool animlockflg_;		//アニメーションロック

		bool deadflg_;			//死んでいるか
	};

	Enemy(void);		// コンストラクタ
	~Enemy(void);		// デストラクタ

	void Init(int org);	// 初期化
	void Update(void);	// 更新
	void Draw(void);	// 描画
	void Release(void); // 解放

	// 死亡フラグ
	bool IsDead()const;

	// セッター関数
	void SetEnemyPos(const VECTOR& pos);		// 敵の座標の設定

	// ゲッター関数
	struct Info GetInfo(void) { return list; }

	// 敵の現在位置の取得する（外部から読み取り専用）
	VECTOR GetPosition()const;
	int	   GetHP()const;

	// 敵のHPを減らす
	void SetDamage(int dp);

	// デバックの描画
	void DrawDebug(void);

	// ベクトルの移動判定
	bool IsMove();

private:
	// 構造体を格納する変数
	struct Info list;


	void UpdateMove(void);			//移動
	void PlayAnimation(void);		//アニメーション再生
	void DebugAnimation(void);		//数字キーでアニメーション切り替え
	void ChangeAnimation(int idx);	//アニメーション切り替え
};

