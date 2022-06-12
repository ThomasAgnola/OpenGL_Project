#pragma once
#include "libs.h"

enum direction
{
	Forward = 0,
	Backward,
	Left,
	Right,
	Up,
	Down
};

class Camera
{
private:
	glm::mat4 ViewMatrix;

	GLfloat movementSpeed;
	GLfloat sensitivity;

	glm::vec3 worldUp;
	glm::vec3 Position;
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 up;

	GLfloat pitch;
	GLfloat yaw;
	GLfloat roll;

	void updateCameraVectors()
	{
		this->front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		this->front.y = sin(glm::radians(this->pitch));
		this->front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));

		this->front = glm::normalize(this->front);
		this->right = glm::normalize(glm::cross(this->front, this->worldUp));
		this->up = glm::normalize(glm::cross(this -> right, this->front));
	}


public:
	Camera()
	{

	}

	Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 worldup)
	{
		this->ViewMatrix = glm::mat4(1.f);

		this->movementSpeed = 3.f;
		this->sensitivity = 5.f;

		this->worldUp = worldup;
		this->Position = position;
		this->right = glm::vec3(0.f);
		this->up = worldup;

		this->pitch = 0.f;
		this->yaw = -90.f;
		this->roll = 0.f;

		this->updateCameraVectors();
	}

	~Camera()
	{

	}

	void initCamera(glm::vec3 position, glm::vec3 direction, glm::vec3 worldup)
	{
		this->ViewMatrix = glm::mat4(1.f);

		this->movementSpeed = 3.f;
		this->sensitivity = 5.f;

		this->worldUp = worldup;
		this->Position = position;
		this->right = glm::vec3(0.f);
		this->up = worldup;

		this->pitch = 0.f;
		this->yaw = -90.f;
		this->roll = 0.f;

		this->updateCameraVectors();
	}

	// Accessors
	const glm::mat4 getViewMatrix()
	{
		this->updateCameraVectors();

		this->ViewMatrix = glm::lookAt(this->Position, this->Position + this->front, this->up);

		return this->ViewMatrix;
	}

	const glm::vec3 getPosition() const
	{
		return this->Position;
	}

	//Functions
	void move(const float& dt, const int direction)
	{
		// update position vector
		switch (direction)
		{
		case Forward:
			this->Position -= this->front * this->movementSpeed * dt;
			break;
		case Backward:
			this->Position += this->front * this->movementSpeed * dt;
			break;
		case Left:
			this->Position -= this->right * this->movementSpeed * dt;
			break;
		case Right:
			this->Position += this->right * this->movementSpeed * dt;
			break;
		case Up:
			this->Position += this->up * this->movementSpeed * dt;
			break;
		case Down:
			this->Position -= this->up * this->movementSpeed * dt;
			break;
		default:
			break;
		}
	}

	void updateMouseInput(const float& dt, const double& offsetX, const double& offsetY)
	{
		// update pitch, yaw and roll
		this->pitch -= static_cast<GLfloat>(offsetY) * this->sensitivity * dt;
		this->yaw += static_cast<GLfloat>(offsetX) * this->sensitivity * dt;

		if (this->pitch > 80.f) this->pitch = 80.f;
		else if (this->pitch < -80.f) this->pitch = -80.f;

		if ( this->yaw > 360.f || this->yaw < -360.f ) this->yaw = 0.f;
	}


	void updateInput(const float& dt, const int direction, const double& offsetX, const double& offsetY)
	{
		this->move(dt, direction);
		this->updateMouseInput(dt, offsetX, offsetY);
	}

};