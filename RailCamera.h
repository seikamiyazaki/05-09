#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugText.h"

class RailCamera
{
public:
	void Initialize(const Vector3& position, const Vector3& rot);
	void Update();

	const ViewProjection& GetViewProjection() { return viewProjection_; }
	WorldTransform* GetWorldTransform() { return &worldTransform_; }

private:
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
	DebugText* debugText_ = nullptr;
};