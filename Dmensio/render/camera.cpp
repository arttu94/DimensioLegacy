#include "camera.h"

Camera::Camera()
{
	/*m_positionX = 0.0f;
	m_positionY = 5.0f;
	m_positionZ = -15.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;

	m_lookAtX = 0.0f;
	m_lookAtY = 0.0f;
	m_lookAtZ = -1.0f;

	m_upX = 0.0f;
	m_upY = 1.0f;
	m_upZ = 0.0f;*/

	//SetPosition(0, 10, -15);

	m_startTime = timeGetTime();
}

Camera::Camera(const Camera& other)
{
}

Camera::~Camera()
{
}

void Camera::SetPosition(float x, float y, float z)
{
	Eye.m128_f32[0] = x;
	Eye.m128_f32[1] = y;
	Eye.m128_f32[2] = z;
	return;
}

void Camera::SetPosition(XMFLOAT3 newPos)
{
	Eye.m128_f32[0] = newPos.x;
	Eye.m128_f32[1] = newPos.y;
	Eye.m128_f32[2] = newPos.z;
	return;
}

void Camera::SetRotation(float yaw, float pitch)
{
	camYaw = yaw;
	camPitch = pitch;
	return;
}

/*XMFLOAT3 Camera::GetRotation()
{
	return XMFLOAT3(m_rotationX, m_rotationY, m_rotationZ);
}*/

XMFLOAT3 Camera::GetPosition()
{
	//return XMFLOAT3(m_positionX, m_positionY, m_positionZ);
	return XMFLOAT3(Eye.m128_f32[0], Eye.m128_f32[1], Eye.m128_f32[2]);
}

XMFLOAT3 Camera::GetAt()
{
	return XMFLOAT3(At.m128_f32[0], At.m128_f32[1], At.m128_f32[2]);
}

XMFLOAT3 Camera::GetEye()
{
	return XMFLOAT3(Eye.m128_f32[0], Eye.m128_f32[1], Eye.m128_f32[2]);
}

void Camera::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
	return;
}

void Camera::CalculatePositionFromVector()
{
	XMVECTOR lookingVector = XMVectorSubtract(At, Eye);
	
	XMVECTOR normdVec = XMVector3Normalize(lookingVector);
	XMVECTOR newPos = XMVectorScale(normdVec, -15);

	SetPosition(newPos.m128_f32[0], newPos.m128_f32[1]+5, newPos.m128_f32[2]);
}

void Camera::UpdateCamera()
{
	camRotationMatrix = XMMatrixRotationRollPitchYaw(camPitch, camYaw, 0);
	At = XMVector3TransformCoord(DefaultForward, camRotationMatrix);
	At = XMVector3Normalize(At);

	XMMATRIX RotateYTempMatrix;
	RotateYTempMatrix = XMMatrixRotationY(camYaw);

	camRight = XMVector3TransformCoord(DefaultRight, RotateYTempMatrix);
	Up = XMVector3TransformCoord(Up, RotateYTempMatrix);
	camForward = XMVector3TransformCoord(DefaultForward, RotateYTempMatrix);

	//Eye += moveLeftRight*camRight;
	//Eye += moveBackForward*camForward;

	moveLeftRight = 0.0f;
	moveBackForward = 0.0f;

	At = XMVectorAdd(Eye, At);

	/*if (timeGetTime() >= (m_startTime + 1000))
	{
		cout << "At vector : x " << At.m128_f32[0] << " y " << At.m128_f32[1] << " z " << At.m128_f32[2] << endl;
		cout << "Position : x " << Eye.m128_f32[0] << " y " << Eye.m128_f32[1] << " z " << Eye.m128_f32[2] << endl;
		m_startTime = timeGetTime();
	}*/

	m_viewMatrix = XMMatrixLookAtLH(Eye, At, Up); 
}

/*void Camera::Render()
{
	XMFLOAT3 up, position, lookAt;
	XMVECTOR upVector, positionVector, lookAtVector;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;


	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Load it into a XMVECTOR structure.
	upVector = XMLoadFloat3(&up);

	// Setup the position of the camera in the world.
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	position = XMFLOAT3(Eye.m128_f32[0], Eye.m128_f32[1], Eye.m128_f32[2]);
	// Load it into a XMVECTOR structure.
	positionVector = XMLoadFloat3(&position);

	// Setup where the camera is looking by default.
	//At = XMVector3Normalize(At);
	lookAt = XMFLOAT3(At.m128_f32[0], At.m128_f32[1], At.m128_f32[2]);

	// Load it into a XMVECTOR structure.
	lookAtVector = XMLoadFloat3(&lookAt);

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = m_rotationX * 0.0174532925f;
	yaw = m_rotationY * 0.0174532925f;
	//roll = m_rotationZ * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	rotationMatrix = XMMatrixRotationRollPitchYaw(camPitch, camYaw, 0);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	// Finally create the view matrix from the three updated vectors.
	m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);

	return;
}*/

/*void Camera::Render()
{
	XMFLOAT3 up, position, lookAt;
	XMVECTOR upVector, positionVector, lookAtVector;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;


	// Setup the vector that points upwards.
	up.x = m_upX;
	up.y = m_upY;
	up.z = m_upZ;

	// Load it into a XMVECTOR structure.
	upVector = XMLoadFloat3(&up);

	// Setup the position of the camera in the world.
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	// Load it into a XMVECTOR structure.
	positionVector = XMLoadFloat3(&position);

	// Setup where the camera is looking by default.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Load it into a XMVECTOR structure.
	lookAtVector = XMLoadFloat3(&lookAt);

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = m_rotationX * 0.0174532925f;
	yaw = m_rotationY * 0.0174532925f;
	roll = m_rotationZ * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	//m_lookAtX = lookAtVector.m128_f32[0];
	//m_lookAtY = lookAtVector.m128_f32[1];
	//m_lookAtZ = lookAtVector.m128_f32[2];

	// Finally create the view matrix from the three updated vectors.
	m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);

	return;
}*/