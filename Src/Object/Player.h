#pragma once
#include <DxLib.h>
#include <vector>

class Player
{
public:
	// 移動量
	static constexpr float MOVE_SPEED_STOP = 0.0f;		// しゃがみ&止まる
	static constexpr float MOVE_SPEED_WALK = 2.5f;		// 歩き
	static constexpr float MOVE_SPEED_RUN = 8.0f;		// 走る

	// 初期モデル補正角度
	static constexpr VECTOR INIT_MODEL_ROT_OFFSET = { 0.0f,DX_PI_F / 2.0f,0.0f };

	// 向いている方向
	static constexpr VECTOR INIT_DIR = { 0.0f,0.0f,-1.0f };

	// 視野角
	static constexpr float VIEW_ANGLE = 30.0f;

	// アニメションの再生速度
	static constexpr float ANIM_SPEED = 0.5f;			//生きている時
	static constexpr float CROUCH_ANIM_SPEED = 1.5f;	//しゃがんでいるとき
	static constexpr float CROUCH_ATTACK_SPEED = 1.0f;	//しゃがみ攻撃
	static constexpr float ATTACK_ANIM_SPEED = 0.7f;	//攻撃時
	static constexpr float DEAD_ANIM_SPEED = 0.2f;		//死んでいる時

	static constexpr int NoWeaponIdle = 0;				//武器なし待機
	static constexpr int NoWeaponWalk = 1;				//武器なし歩き
	static constexpr int NoWeaponRun = 2;				//武器なし走り
	static constexpr int WeaponOut = 3;					//武器取り出し	
	static constexpr int Sheach = 4;					//武器戻す
	static constexpr int WeaponIdle = 5;				//武器あり待機
	static constexpr int WeaponWalk = 6;				//武器あり歩き
	static constexpr int WeaponRun = 7;					//武器あり走り
	static constexpr int Strafe = 8;					//横歩き
	static constexpr int Jump = 9;						//ジャンプ
	static constexpr int Crouch = 10;					//しゃがみ
	static constexpr int Crouch_Cancel = 11;			//しゃがみ解除
	static constexpr int Crouch_Idle = 12;				//しゃがみ待機
	static constexpr int Crouch_Guard = 13;				//しゃがみ防御
	static constexpr int Crouch_Attack = 14;			//しゃがみ攻撃
	static constexpr int Attack_1 = 15;					//攻撃１
	static constexpr int Attack_2 = 16;					//攻撃２
	static constexpr int Attack_3 = 17;					//攻撃３
	static constexpr int Attack_4 = 18;					//攻撃４
	static constexpr int Death = 19;					//死亡

	// プレイヤーの情報の構造体
	struct Info
	{
		int modelid_;			// モデルID

		VECTOR pos_;			// 座標
		VECTOR rot_;			// 回転値
		VECTOR scale_;			// 大きさ
		float moveSpeed_;		// 移動スピード
		int moveDir_;			// 移動方向
		int moveKind_;			// プレイヤーの移動状態

		// 移動ベクトル
		VECTOR moveVec_;		// 移動ベクトル
		VECTOR oldMoveVec_;		// 直前の移動ベクトル
		VECTOR moveVecRad_;		// 移動ベクトルが生成する角度

		VECTOR localRot_;		// 調整用初期角度

		//アニメーション
		int animindex_;			//アニメーションの種類
		int  animAttachNo_;		// 設定するアニメーションの番号
		float animTotalTime_;	// アニメーションの総再生時間
		float currentAnimTime_; // 直前のアニメーションの時間
		bool animlockflg_;		//アニメーションロック

		int oldAttackNo_;		//前のアニメーション番号
		int blendAnimNo_;		//ブレンド中のアニメーション
		float blendCurrentTime_;//ブレンド経過時間
		float blendMaxTime_;	//ブレンド最大時間
		bool blendflg_;			//ブレンド中か

		bool isdead_;			//死亡したかどうか
		bool crouchflg_;		//しゃがんでいるか
		bool crouchattackflg_;	//しゃがみ攻撃しているか
		bool weaponflg_;		//武器出しているか
		bool attackflg_;		//攻撃しているか
		bool comboflg_;			//コンボしているか
		int combostep_;			//コンボ段階

		float currentratio_;	//現在のアニメーションの再生時間(割合)
		float remainingtime_;	//アニメーションがあとどれくらいで終わるか
	};

	Player(void);		// コンストラクタ
	~Player(void);		// デストラクタ

	void SystemInit(void);	// 初期化
	void GameInit(void);
	void Update(void);	// 更新
	void UpdateMove(void);
	void Draw(void);	// 描画
	void Release(void); // 解放

	// ベクトルの移動判定
	bool IsMove(VECTOR _moveVec);

	void SetRotation(void);

	[[nodiscard]] VECTOR GetPlayerPos(void) { return list.pos_; }

private:
	// 構造体を格納する変数
	struct Info list;

	void PlayerMove(int idle, int walk, int run);		//プレイヤーの動き
	bool CrouchUpdate(void);							//しゃがみ
	bool AttackUpdate(void);							//攻撃
	void PlayAnimation(void);							//アニメーション再生
	void DebugAnimation(void);							//数字キーでアニメーション切り替え
	void ChangeAnimation(int idx, bool lock = false);	//アニメーション切り替え
	//攻撃コンボ(今のコンボ、次のアニメーション、次の段階、受付時間(後ろから)、アニメーションスキップ時間(後ろから))
	void AttackCombo(int nowcombo, int nextanimidx, int nextstep, float reception, float remainingtime);

};

