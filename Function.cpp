#include "Function.h"

float MinNum(float num, float num2) {
	if (num < num2) return num2;
	return num;
}

float MaxNum(float num, float num2) {
	if (num > num2) return num2;
	return num;
}

float Clamp(float min, float max, float num) {
	if (min > num) return min;
	else if (num > max) return max;
	return num;
}

Matrix4 MatScaleCreate(Vector3 scale) {
	Matrix4 matScale = MathUtility::Matrix4Identity();
	matScale.m[0][0] = scale.x;
	matScale.m[1][1] = scale.y;
	matScale.m[2][2] = scale.z;
	return matScale;
}

Matrix4 MatRotXCreate(float rotationx) {
	Matrix4 matRotX = MathUtility::Matrix4Identity();
	matRotX.m[1][1] = cos(rotationx);
	matRotX.m[1][2] = sin(rotationx);
	matRotX.m[2][1] = -sin(rotationx);
	matRotX.m[2][2] = cos(rotationx);
	return matRotX;
}

Matrix4 MatRotYCreate(float rotationy) {
	Matrix4 matRotY = MathUtility::Matrix4Identity();
	matRotY.m[0][0] = cos(rotationy);
	matRotY.m[0][2] = -sin(rotationy);
	matRotY.m[2][0] = sin(rotationy);
	matRotY.m[2][2] = cos(rotationy);
	return matRotY;
}

Matrix4 MatRotYCreate1(Vector3 rotationy) {
	Matrix4 matRotY = MathUtility::Matrix4Identity();
	matRotY.m[0][0] = cos(rotationy.y);
	matRotY.m[0][2] = -sin(rotationy.y);
	matRotY.m[2][0] = sin(rotationy.y);
	matRotY.m[2][2] = cos(rotationy.y);
	return matRotY;
}

Matrix4 MatRotZCreate(float rotationz) {
	Matrix4 matRotZ = MathUtility::Matrix4Identity();
	matRotZ.m[0][0] = cos(rotationz);
	matRotZ.m[0][1] = sin(rotationz);
	matRotZ.m[1][0] = -sin(rotationz);
	matRotZ.m[1][1] = cos(rotationz);
	return matRotZ;
}

Matrix4 MatRotCreate(Vector3 rotation) {
	Matrix4 matRot = MathUtility::Matrix4Identity();
	Matrix4 matRotZ = MatRotZCreate(rotation.z);
	Matrix4 matRotX = MatRotXCreate(rotation.x);
	Matrix4 matRotY = MatRotYCreate(rotation.y);
	matRot *= matRotZ;
	matRot *= matRotX;
	matRot *= matRotY;
	return matRot;
}

Matrix4 MatTransCreate(Vector3 translation) {
	Matrix4 matTrans = MathUtility::Matrix4Identity();
	matTrans.m[3][0] = translation.x;
	matTrans.m[3][1] = translation.y;
	matTrans.m[3][2] = translation.z;
	return matTrans;
}

Matrix4 MatWorldCreate(Vector3 scale, Vector3 rotation, Vector3 translation) {
	Matrix4 matWorld = MathUtility::Matrix4Identity();
	matWorld *= MatScaleCreate(scale);
	matWorld *= MatRotCreate(rotation);
	matWorld *= MatTransCreate(translation);
	return matWorld;
}

float Rad(float a) {
	return (PI * a) / 180;
}

Vector3 Vector3TransformNormal(Vector3 vec, WorldTransform worldTransform)
{
	Vector3 a;
	a.x = vec.x * worldTransform.matWorld_.m[0][0] + vec.y * worldTransform.matWorld_.m[1][0] + vec.z * worldTransform.matWorld_.m[2][0];
	a.y = vec.x * worldTransform.matWorld_.m[0][1] + vec.y * worldTransform.matWorld_.m[1][1] + vec.z * worldTransform.matWorld_.m[2][1];
	a.z = vec.x * worldTransform.matWorld_.m[0][2] + vec.y * worldTransform.matWorld_.m[1][2] + vec.z * worldTransform.matWorld_.m[2][2];
	return a;
}