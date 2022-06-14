#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "DebugText.h"
#include "Input.h"
#include "ViewProjection.h"
#include "PlayerBullet.h"
#include <memory>
#include<list>

class Player
{
public:
	Player();
	~Player();
	void Initialize(Model* model, uint32_t textureHandle);//������
	void Update();// �X�V
	void Draw(ViewProjection& viewProjection);// �`��

	void Attack();// �U��

private:
	WorldTransform worldTransform_;// ���[���h�ϊ��f�[�^
	Model* model_ = nullptr;// ���f��
	uint32_t textureHandle_ = 0u;// �e�N�X�`���n���h��

	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	std::list<std::unique_ptr<PlayerBullet>> bullets_;// �e
};