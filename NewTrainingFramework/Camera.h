#pragma once
#include "../Utilities/Math.h"
class Camera
{
	Vector3 position;
	Vector3 target;
	Vector3 up;
	GLfloat moveSpeed;
	GLfloat rotateSpeed;
	GLfloat nearPlane;
	GLfloat farPlane;
	GLfloat fov;
	GLfloat deltaTime;
	Vector3 xAxis;
	Vector3 yAxis;
	Vector3 zAxis;
	Matrix viewMatrix;
	Matrix worldMatrix;
	Matrix perspectiveMatrix;
public:

    Camera():
		position(0.0f, 0.0f, 1.0f),  
        target(0.0f, 0.0f, 0.0f),    
        up(0.0f, 1.0f, 0.0f),         
        rotateSpeed(0.1f),        
        nearPlane(0.2f),
        farPlane(10.0f),
        fov(45.0f),
        deltaTime(0.016f)       
    {
        updateWorldView(); 

		float aspect = 800.0f / 600.0f; 
		perspectiveMatrix.SetPerspective(fov, aspect, nearPlane, farPlane);
    }

	void moveOx(int sens);

	void moveOy(int sens);

	void moveOz(int sens);

	void rotateOx(int sens);

	void rotateOy(int sens);

	void rotateOz(int sens);
	
	void updateAxes();

	void updateWorldView();

	void setDeltaTime(GLfloat);

	// Returnează matricea de view a camerei
	Matrix getViewMatrix() {
		return viewMatrix;
	}

	// Returnează matricea de perspectivă a camerei
	Matrix getPerspectiveMatrix() {
		return perspectiveMatrix;
	}

};