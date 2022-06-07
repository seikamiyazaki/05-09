#pragma once
#include "Matrix4.h"
#include "WorldTransform.h"
#include "AxisIndicator.h"

#define PI 3.141592653589793

float MinNum(float num, float num2);
float MaxNum(float num, float num2);

float Clamp(float min, float max, float num);

Matrix4 MatScaleCreate(Vector3 scale);

Matrix4 MatRotXCreate(float rotationx);

Matrix4 MatRotYCreate(float rotationy);
Matrix4 MatRotYCreate1(Vector3 rotationy);

Matrix4 MatRotZCreate(float rotationz);

Matrix4 MatRotCreate(Vector3 rotation);

Matrix4 MatTransCreate(Vector3 translation);

Matrix4 MatWorldCreate(Vector3 scale, Vector3 rotation, Vector3 translation);

float Rad(float a);