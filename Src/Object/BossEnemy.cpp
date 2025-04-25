#include "BossEnemy.h"
#include "../Application.h"
#include "../Manager/InputManager.h"

BossEnemy::BossEnemy(void)
{
	list.modelid_=-1;

	VECTOR scale_ = { 1.0f,1.0f,1.0f };
	VECTOR pos_ = { 0.0f,0.0f,0.0f };
	VECTOR rot_ = { 0.0f,0.0f,0.0f };
	float moveSpeed_ = 0.0f;
	int moveDir_ = 0;
	int moveKind_ = 0;

	VECTOR moveVec_ = { 0.0f,0.0f,0.0f };
	VECTOR oldMoveVec_ = { 0.0f,0.0f,0.0f };
	VECTOR moveVecRad_ = { 0.0f,0.0f,0.0f };

	VECTOR localRot_ = { 0.0f,0.0f,0.0f };
	int animindex_ = 0;
	int  animAttachNo_ = 0;
	float animTotalTime_ = 0.0f;
	float currentAnimTime_ = 0.0f;
	bool animlockflg_ = false;

	bool deadflg_ = false;
}

BossEnemy::~BossEnemy(void)
{
}

// 初期化
void BossEnemy::Init(void)
{
	//モデル
	list.modelid_ = MV1LoadModel((Application::PATH_MODEL + "BossEnemyModel.mv1").c_str());
	//座標
	list.pos_ = { 0.0f,0.0f,50.0f };
	MV1SetPosition(list.modelid_, list.pos_);
	//大きさ
	list.scale_ = { 1.0f,1.0f,1.0f };
	MV1SetScale(list.modelid_, list.scale_);
	//回転
	list.rot_ = { 0.0f,0.0f,0.0f };
	MV1SetRotationXYZ(list.modelid_, list.rot_);

	//アニメーション関連
	list.animindex_ = 0;
	list.animAttachNo_ = MV1AttachAnim(list.modelid_, list.animindex_);
	list.animTotalTime_ = MV1GetAttachAnimTotalTime(list.modelid_, list.animAttachNo_);
	list.currentAnimTime_ = 0.0f;
	list.animlockflg_ = false;
	MV1SetAttachAnimTime(list.modelid_, list.animAttachNo_, list.currentAnimTime_);

	list.moveKind_ = 0;

	list.deadflg_ = false;
}

// 更新
void BossEnemy::Update(void)
{
	PlayAnimation();
	DebugAnimation();
}

// 描画
void BossEnemy::Draw(void)
{
	MV1DrawModel(list.modelid_);
}

// 解放
void BossEnemy::Release(void)
{
	MV1DeleteModel(list.modelid_);
}

//移動
void BossEnemy::UpdateMove(void)
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
void BossEnemy::PlayAnimation(void)
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
	case 3:			//パンチ
	case 4:			//大ぶりの攻撃
		list.currentAnimTime_ += ANIM_SPEED;
		if (list.currentAnimTime_ >= list.animTotalTime_)
		{
			list.currentAnimTime_ = list.animTotalTime_;
			list.animlockflg_ = false;
		}
		break;
	case 5:			//死亡
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
void BossEnemy::DebugAnimation(void)
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
	if (ins.IsTrgDown(KEY_INPUT_5)) {
		ChangeAnimation(5);
	}
}

//アニメーション切り替え
void BossEnemy::ChangeAnimation(int idx)
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
