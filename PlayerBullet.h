#pragma once
#include "Function.h"
#include "Model.h"
#include "WorldTransform.h"

class PlayerBullet
{
public:
	PlayerBullet();
	~PlayerBullet();
	void Initialize(Model* model, const Vector3& position);
	void Update();
	void Draw(const ViewProjection& viewProjection);

private:
	WorldTransform worldTransform_;// ���[���h�ϊ��f�[�^
	Model* model_ = nullptr;// ���f��
	uint32_t textureHandle_ = 0u;// �e�N�X�`���n���h��
};