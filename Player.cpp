#include "Player.h"
#include <cassert>

Player::Player() {}
Player::~Player() {}

void Player::Initilaize(Model* model, uint32_t textureHandle)
{
	assert(model);// NULLポインタチェック

	model_ = model;
	textureHandle_ = textureHandle;

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();
}

void Player::Update()
{
	Vector3 move = { 0,0,0 };// キャラクターの移動ベクトル

	if (input_->PushKey(DIK_U)) {
		worldTransform_.rotation_.y -= 0.2f;
	}
	else if (input_->PushKey(DIK_I)) {
		worldTransform_.rotation_.y += 0.2f;
	}

	if (input_->PushKey(DIK_LEFT)) {
		move.x -= 0.2f;
	}
	else if (input_->PushKey(DIK_RIGHT)) {
		move.x += 0.2f;
	}
	if (input_->PushKey(DIK_UP)) {
		move.y += 0.2f;
	}
	else if (input_->PushKey(DIK_DOWN)) {
		move.y -= 0.2f;
	}
	worldTransform_.translation_ += move;

	// キャラクター攻撃処理
	Attack();

	// 弾更新
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

	// 移動限界座標
	const float kMoveLimitX = 33.0f;
	const float kMoveLimitY = 17.0f;
	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();
	worldTransform_.matWorld_ *= MatRotYCreate1(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= MatTransCreate(worldTransform_.translation_);

	worldTransform_.TransferMatrix();

	// デバッグ用表示
	debugText_->SetPos(50, 70);
	debugText_->Printf("player:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
}

void Player::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);// 自キャラの描画
	// 弾描画
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Player::Attack()
{
	if (input_->PushKey(DIK_SPACE)) {
		// 弾を発生し、初期化
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, worldTransform_.translation_);
		// 弾を登録する
		bullets_.push_back(std::move(newBullet));
	}
}
