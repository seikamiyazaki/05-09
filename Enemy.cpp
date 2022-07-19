#include "Enemy.h"
#include <cassert>
#include "Player.h"

Enemy::Enemy() {}
Enemy::~Enemy() {}

void Enemy::Initialize(Model* model, const Vector3& velocity)
{
	assert(model);

	debugText_ = DebugText::GetInstance();

	model_ = model;
	textureHandle_ = TextureManager::Load("kamata.png");

	worldTransform_.Initialize();

	worldTransform_.translation_ = { 10.0f,2.0f,20.0f };
	velocity_ = velocity;

	// �ڋ߃t�F�[�Y������
	ApproachInitialize();
}

void Enemy::Update()
{
	switch (phase_)
	{
	case Phase::Approach:
		//// �ړ��x�N�g��
		//worldTransform_.translation_ += Vector3(0.0f, 0.0f, -0.1f);
		//// �K��̈ʒu�ɓ��B�����痣�E
		//if (worldTransform_.translation_.z < 0.0f) {
		//	phase_ = Phase::Leave;
		//}
		ApproachInitialize();
		break;
	//case Phase::Leave:
	//	// �ړ��x�N�g��
	//	worldTransform_.translation_ += Vector3(-0.1f, 0.1f, 0);
	//	break;
	}

	// �f�X�t���O�̗������e���폜
	enemyBullet_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->IsDead();
		});

	for (std::unique_ptr<EnemyBullet>& enemyBullet : enemyBullet_) {
		enemyBullet->Update();
	}

	//worldTransform_.translation_ -= velocity_;

	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();
	worldTransform_.matWorld_ *= MatRotYCreate1(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= MatTransCreate(worldTransform_.translation_);

	worldTransform_.TransferMatrix();

	// �f�o�b�O�p�\��
	debugText_->SetPos(50, 90);
	debugText_->Printf("enemy:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	// �e�`��
	for (std::unique_ptr<EnemyBullet>& enemyBullet : enemyBullet_) {
		enemyBullet->Draw(viewProjection);
	}
}

void Enemy::Fire()
{
	// �e�̑��x
	const float kBulletSpeed = 1.0f;
	// ���W�̎擾
	Vector3 playerPos = player_->GetWorldPosition();
	Vector3 enemyPos = this->GetWorldPosition();
	// �����x�N�g�� = �S�[���n�_ - �X�^�[�g�n�_
	Vector3 velocity = playerPos;
	velocity -= enemyPos;
	// �x�N�g���̐��K��
	velocity.normalize();
	// �x�N�g���̒������A�����ɍ��킹��
	velocity *= kBulletSpeed;
	// ���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
	velocity = VW(velocity, worldTransform_);
	// �e�𔭐����A������
	std::unique_ptr<EnemyBullet> newEnemyBullet = std::make_unique<EnemyBullet>();
	newEnemyBullet->Initialize(model_, worldTransform_.translation_, velocity);
	// �e��o�^����
	enemyBullet_.push_back(std::move(newEnemyBullet));
}

void Enemy::ApproachInitialize()
{
	// ���˃^�C�}�[�J�E���g�_�E��
	kFireTimer--;
	// �w�莞�ԂɒB����
	if (kFireTimer < 0) {
		// �e�𔭎�
		Fire();
		// ���˃^�C�}�[��������
		kFireTimer = kFireInteral;
	}
}

Vector3 Enemy::GetWorldPosition()
{
	// ���[���h���W������ϐ�
	Vector3 worldPos;
	// ���[���h�s��̕��s�ړ��������擾
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

void Enemy::OnCollision()
{
}
