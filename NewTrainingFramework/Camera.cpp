#include "stdafx.h"
#include "Camera.h"

void Camera::moveOx(int sens) {
	Vector3 forward = xAxis * sens;
	Vector3 vectorDeplasare = forward * moveSpeed * deltaTime;
	position += vectorDeplasare;
	target += vectorDeplasare;

	updateWorldView();
}

void Camera::moveOy(int sens) {
	Vector3 forward = yAxis * sens;
	Vector3 vectorDeplasare = forward * moveSpeed * deltaTime;
	position += vectorDeplasare;
	target += vectorDeplasare;

	updateWorldView();
}

void Camera::moveOz(int sens) {
	Vector3 forward = zAxis * sens;
	Vector3 vectorDeplasare = forward * moveSpeed * deltaTime;
	position += vectorDeplasare;
	target += vectorDeplasare;

	updateWorldView();
}

void Camera::rotateOx(int sens)
{

	/*float unghiRotatie = sens * rotateSpeed * deltaTime;

	Matrix mRotateOX;
	mRotateOX.SetRotationX(unghiRotatie);

	Vector4 localUp = Vector4(0, 1, 0, 0);
	Vector4 rotatedLocalUp = localUp * mRotateOX;

	up = (rotatedLocalUp * worldMatrix).toVector3();
	up = up.Normalize();

	Vector4 localTarget = Vector4(0.0f, 0.0f, -(target - position).Length(), 1.0f);
	Vector4 rotatedTarget = localTarget * mRotateOX;

	target = (rotatedTarget * worldMatrix).toVector3();

	updateWorldView();*/
}

void Camera::rotateOy(int sens)
{
	/*float unghiRotatie = sens * rotateSpeed * deltaTime;

	Matrix mRotateOY;
	mRotateOY.SetRotationY(unghiRotatie);

	Vector4 localTarget = Vector4(0.0f, 0.0f, -(target - position).Length(), 1.0f);
	Vector4 rotatedTarget = localTarget * mRotateOY;

	target = (rotatedTarget * worldMatrix).toVector3();

	updateWorldView();*/
}

void Camera::rotateOz(int sens) {
	updateWorldView();
}

void Camera::updateAxes() {
	zAxis = -(target - position).Normalize(); 
	yAxis = up.Normalize();
	xAxis = zAxis.Cross(yAxis).Normalize();
}

void Camera::updateWorldView() {
	Matrix R;
	R.SetIdentity();
	//worldMatrix = 0;
	R.m[0][0] = xAxis.x;
	R.m[0][1] = xAxis.y;
	R.m[0][2] = xAxis.z;

	R.m[1][0] = yAxis.x;
	R.m[1][1] = yAxis.y;
	R.m[1][2] = yAxis.z;

	R.m[2][0] = zAxis.x;
	R.m[2][1] = zAxis.y;
	R.m[2][2] = zAxis.z;

	Matrix T;

	T.SetIdentity();
	T.SetTranslation(position);

	worldMatrix = R * T;

	Matrix T1;

	T1.SetIdentity();
	T1.SetTranslation(-position);
	
	Matrix R1;

	R1 = R.Transpose();

	viewMatrix = T1 * R1;

	updateAxes();

}

void Camera::setDeltaTime(GLfloat) {

}