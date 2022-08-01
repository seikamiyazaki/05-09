#include "Player.h"
#include <cassert>

Player::Player() {}
Player::~Player() 
{
	delete railCamera_;
}

void Player::Initialize(Model* model, uint32_t textureHandle)
{
	assert(model);// NULL�|�C���^�`�F�b�N

	model_ = model;
	textureHandle_ = textureHandle;

	// �V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.translation_ = { 0.0f,0.0f,50.0f };

	railCamera_ = new RailCamera;

	worldTransform_.Initialize();
}

void Player::Update()
{
	// �f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsDead();
		});

	Vector3 move = { 0,0,0 };// �L�����N�^�[�̈ړ��x�N�g��

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

	// �L�����N�^�[�U������
	Attack();

	// �e�X�V
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

	// �ړ����E���W
	const float kMoveLimitX = 33.0f;
	const float kMoveLimitY = 17.0f;
	// �͈͂𒴂��Ȃ�����
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();
	worldTransform_.matWorld_ *= MatRotYCreate1(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= MatTransCreate(worldTransform_.translation_);

	// �e�q
	worldTransform_.matWorld_ = MatWorldCreate(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;

	worldTransform_.TransferMatrix();

	// �f�o�b�O�p�\��
	debugText_->SetPos(50, 70);
	debugText_->Printf("player:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
}

void Player::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);// ���L�����̕`��
	// �e�`��
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Player::Attack()
{
	if (input_->TriggerKey(DIK_SPACE)) {
		// �e�̑��x
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);
		// ���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
		velocity = Vector3TransformNormal(velocity, worldTransform_);
		// �e�𔭐����A������
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, GetWorldPosition(), velocity);
		// �e��o�^����
		bullets_.push_back(std::move(newBullet));
	}
}

Vector3 Player::GetWorldPosition()
{
	// ���[���h���W������ϐ�
	Vector3 worldPos;
	// ���[���h�s��̕��s�ړ��������擾
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Player::OnCollision()
{
}