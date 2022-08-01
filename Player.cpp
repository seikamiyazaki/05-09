#include "Player.h"
#include <cassert>

Player::Player() {}
Player::~Player() 
{
	delete railCamera_;
}

void Player::Initialize(Model* model, uint32_t textureHandle)
{
	assert(model);// NULLポインタチェック

	model_ = model;
	textureHandle_ = textureHandle;

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.translation_ = { 0.0f,0.0f,50.0f };

	railCamera_ = new RailCamera;

	worldTransform_.Initialize();
}

void Player::Update()
{
	// デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsDead();
		});

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

	// 親子
	worldTransform_.matWorld_ = MatWorldCreate(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;

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
	if (input_->TriggerKey(DIK_SPACE)) {
		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);
		// 速度ベクトルを自機の向きに合わせて回転させる
		velocity = Vector3TransformNormal(velocity, worldTransform_);
		// 弾を発生し、初期化
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, GetWorldPosition(), velocity);
		// 弾を登録する
		bullets_.push_back(std::move(newBullet));
	}
}

Vector3 Player::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Player::OnCollision()
{
}