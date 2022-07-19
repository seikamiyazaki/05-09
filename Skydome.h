#pragma once
#include "Function.h"

class Skydome
{
public:
	void Intialize(Model* model);
	void Update();
	void Draw(ViewProjection& viewProjection_);

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
};

