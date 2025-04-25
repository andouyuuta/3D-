#include "Enemy.h"
#include "../Application.h"
#include "../Manager/InputManager.h"

namespace
{
	// 移動量
	const float MOVE_SPEED_WALK = 2.5f;	// 歩き
	const float MOVE_SPEED_RUN = 8.0f;	// 走る
	const float MOVE_SPEED_STOP = 0.0f;

	// 初期モデル補正角度
	const VECTOR INIT_MODEL_ROT_OFFSET = { 0.0f,DX_PI_F / 2.0f,0.0f };

	// 向いている方向
	const VECTOR INIT_DIR = { 0.0f,0.0f,-1.0f };

	// 視野角
	const float VIEW_ANGLE = 30.0f;

	// アニメーションの再生速度
	const float ANIM_SPEED = 0.5f;		// 生きている時
	const float DEAD_ANIM_SPEED = 0.2f;	// 死んでいる時
}

Enemy::Enemy(void)
{
	list.modelid_ = -1;
	list.scale_ = VGet(1.0f, 1.0f, 1.0f);
	list.pos_ = VGet(0.0f, 0.0f, 0.0f);
	list.rot_ = VGet(0.0f, 0.0f, 0.0f);

	list.moveSpeed_ = MOVE_SPEED_STOP;
	list.moveDir_ = 0;
	list.moveKind_ = 0;

	list.moveVec_ = VGet(0.0f, 0.0f, 0.0f);
	list.oldMoveVec_ = VGet(0.0f, 0.0f, 0.0f);
	list.moveVEcRad_ = VGet(0.0f, 0.0f, 0.0f);
	list.localRot_ = INIT_MODEL_ROT_OFFSET;

	list.animindex_ = 0;
	list.animAttachNo_ = -1;
	list.animTotalTime_ = 0.0f;
	list.currentAnimTime_ = 0.0f;
	list.animlockflg_ = false;

	list.deadflg_ = false;
}

Enemy::~Enemy(void)
{
}

// 初期化
void Enemy::Init(int org)
{
	//モデル
	list.modelid_ = MV1DuplicateModel(org);
	//座標
	list.pos_ = { 0.0f,0.0f,50.0f };
	MV1SetPosition(list.modelid_, list.pos_);
	//大きさ
	list.scale_ = { 1.0f,1.0f,1.0f };
	MV1SetScale(list.modelid_, list.scale_);
	//回転
	list.rot_ = { 0.0f,0.0f,0.0f };
	MV1SetRotationXYZ(list.modelid_, list.rot_);

	// 敵のHPの初期化
	list.hp_ = ENEMY_MAX_HP;


	//アニメーション関連
	list.animindex_ = 0;
	list.animAttachNo_ = MV1AttachAnim(list.modelid_, list.animindex_);
	list.animTotalTime_ = MV1GetAttachAnimTotalTime(list.modelid_, list.animAttachNo_);
	list.currentAnimTime_ = 0.0f;
	list.animlockflg_ = false;
	MV1SetAttachAnimTime(list.modelid_, list.animAttachNo_, list.currentAnimTime_);

	list.deadflg_ = false;
}

// 更新
void Enemy::Update(void)
{
	PlayAnimation();
	DebugAnimation();
}

// 描画
void Enemy::Draw(void)
{
	MV1DrawModel(list.modelid_);
}

// 解放
void Enemy::Release(void)
{
	MV1DeleteModel(list.modelid_);
}

// 死亡フラグ
bool Enemy::IsDead() const
{
	return list.deadflg_;
}

void Enemy::SetEnemyPos(const VECTOR& pos)
{
	list.pos_ = pos;
	MV1SetPosition(list.modelid_, list.pos_);
}


// 敵の現在位置の取得する（外部から読み取り専用）
VECTOR Enemy::GetPosition() const
{
	return list.pos_;
}

// 敵のHPの取得
int Enemy::GetHP() const
{
	return list.hp_;
}

// 敵のHPを減らす
void Enemy::SetDamage(int dp)
{
	list.hp_ -= dp;
	// HPが0以下なら死亡状態へ移行
	if (list.hp_ <= 0)
	{
		list.hp_ = 0;
		// 死んだフラグ
		list.deadflg_ = true;
	}
}

//移動
void Enemy::UpdateMove(void)
{
	//// 入力制御取得
	//InputManager& ins = InputManager::GetInstance();

	//if (list.animlockflg_)
	//{
	//	return;
	//}

	////死亡時にも移動させない
	//if (list.deadflg_)
	//{
	//	list.moveSpeed_ = MOVE_SPEED_STOP;
	//	ChangeAnimation(19);
	//	return;
	//}

	//// WASDでプレイヤー移動
	//list.moveVec_ = { 0.0f, 0.0f, 0.0f };

	//// 左・右・手前・奥のベクトルを作成する
	//VECTOR RIGHT_MOVE_VEC = { 1.0f,  0.0f,  0.0f };
	//VECTOR LEFT_MOVE_VEC = { -1.0f,  0.0f,  0.0f };
	//VECTOR FRONT_MOVE_VEC = { 0.0f,  0.0f,  1.0f };
	//VECTOR BACK_MOVE_VEC = { 0.0f,  0.0f, -1.0f };

	//// 入力方向に移動ベクトルを追加する
	//if (ins.IsNew(KEY_INPUT_UP)) { list.moveVec_ = VAdd(list.moveVec_, FRONT_MOVE_VEC); }
	//if (ins.IsNew(KEY_INPUT_LEFT)) { list.moveVec_ = VAdd(list.moveVec_, LEFT_MOVE_VEC); }
	//if (ins.IsNew(KEY_INPUT_DOWN)) { list.moveVec_ = VAdd(list.moveVec_, BACK_MOVE_VEC); }
	//if (ins.IsNew(KEY_INPUT_RIGHT)) { list.moveVec_ = VAdd(list.moveVec_, RIGHT_MOVE_VEC); }



	//// ベクトルの移動が行われていたら座標更新
	//if (IsMove(list.moveVec_))
	//{
	//	// 移動状態の設定
	//	list.moveKind_ = 1;

	//	//// カメラ角度分設定する
	//	//VECTOR cameraAngles = SceneManager::GetInstance().GetCamera()->GetCameraAngles();
	//	//MATRIX cameraMatY = MGetRotY(cameraAngles.y);
	//	//list.moveVec_ = VTransform(list.moveVec_, cameraMatY);

	//	// スタミナが切れているかどうか
	//	bool spFlg_ = false;

	//	if (spFlg_)
	//	{
	//		// スタミナない状態
	//		list.moveSpeed_ = MOVE_SPEED_STOP;
	//	}
	//	else
	//	{
	//		if (ins.IsNew(KEY_INPUT_LSHIFT))
	//		{
	//			// ダッシュ状態
	//			list.moveSpeed_ = MOVE_SPEED_RUN;
	//			ChangeAnimation(2);
	//		}
	//		else
	//		{
	//			// 歩き状態
	//			list.moveSpeed_ = MOVE_SPEED_WALK;
	//			ChangeAnimation(1);
	//		}
	//	}

	//	// 座標更新
	//	list.moveVec_ = VNorm(list.moveVec_);
	//	list.moveVec_ = VScale(list.moveVec_, list.moveSpeed_);
	//	list.pos_ = VAdd(list.pos_, list.moveVec_);
	//	list.oldMoveVec_ = list.moveVec_;

	//	// 方向を角度に変換する( XZ平面 Y軸)
	//	list.moveVecRad_.y = atan2f(list.moveVec_.x, list.moveVec_.z);

	//	// シンプルに計算角度を設定してみる
	//	list.rot_.y = list.moveVecRad_.y;

	//	// 座標設定
	//	MV1SetPosition(list.modelid_, list.pos_);
	//}
	//else
	//{
	//	// 移動状態の設定
	//	list.moveKind_ = 0;

	//	//アニメーションロックされていないとき待機
	//	if (!list.animlockflg_)
	//	{
	//		ChangeAnimation(0);
	//	}
	//}

}

//アニメーション再生
void Enemy::PlayAnimation(void)
{
	DebugAnimation();			//数字キーでアニメーション切り替え

	switch (list.animindex_)
	{
		//ループさせる
	case 0:			//待機
	case 1:			//歩く
	case 2:			//走る
		list.currentAnimTime_ += ANIM_SPEED;
		if (list.currentAnimTime_ > list.animTotalTime_)
		{
			list.currentAnimTime_ = 0.0f;
		}
		break;
		//ループさせない
	case 3:			//攻撃
		list.currentAnimTime_ += ANIM_SPEED;
		if (list.currentAnimTime_ >= list.animTotalTime_)
		{
			list.currentAnimTime_ = list.animTotalTime_;
			list.animlockflg_ = false;
		}
		break;
	case 4:			//死亡
		list.currentAnimTime_ += DEAD_ANIM_SPEED;
		if (list.currentAnimTime_ >= list.animTotalTime_)
		{
			list.currentAnimTime_ = list.animTotalTime_;
			list.animlockflg_ = false;
		}
		break;
	default:
		break;
	}
	MV1SetAttachAnimTime(list.modelid_, list.animAttachNo_, list.currentAnimTime_);
	MV1SetPosition(list.modelid_, list.pos_);
}

//矢印キーでアニメーション切り替え
void Enemy::DebugAnimation(void)
{
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_0)) {
		ChangeAnimation(0);
	}
	if (ins.IsTrgDown(KEY_INPUT_1)) {
		ChangeAnimation(1);
	}
	if (ins.IsTrgDown(KEY_INPUT_2)) {
		ChangeAnimation(2);
	}
	if (ins.IsTrgDown(KEY_INPUT_3)) {
		ChangeAnimation(3);
	}
	if (ins.IsTrgDown(KEY_INPUT_4)) {
		ChangeAnimation(4);
	}
}

//アニメーション切り替え
void Enemy::ChangeAnimation(int idx)
{
	if (list.animindex_ != idx) {
		MV1DetachAnim(list.modelid_, list.animAttachNo_);
		list.animindex_ = idx;
		list.animAttachNo_ = MV1AttachAnim(list.modelid_, list.animindex_);
		list.animTotalTime_ = MV1GetAttachAnimTotalTime(list.modelid_, list.animAttachNo_);
		list.currentAnimTime_ = 0.0f;
		MV1SetAttachAnimTime(list.modelid_, list.animAttachNo_, list.currentAnimTime_);
	}
}
