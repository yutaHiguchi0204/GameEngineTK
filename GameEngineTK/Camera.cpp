//
// Camera.cpp
// �J�����𐧌䂷��N���X
//
#include "Camera.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;

// �R���X�g���N�^
Camera::Camera(int w, int h)
{
	// �r���[�s��֌W�̏����ݒ�
	m_eyePos = Vector3(0.0f, 0.0f, 5.0f);
	m_refPos = Vector3(0.0f, 0.0f, 0.0f);
	m_upVec = Vector3(0.0f, 1.0f, 0.0f);
	m_view = GetViewMatrix();

	// �v���W�F�N�V�����s��֌W�̏����ݒ�
	m_fovY = XMConvertToRadians(60.0f);
	m_aspect = (float)w / h;
	m_nearClip = 0.1f;
	m_farClip = 1000.0f;
}

// ���z�f�X�g���N�^
Camera::~Camera()
{

}

/// <summary>
/// �X�V����
/// </summary>
void Camera::Update()
{
	// �r���[�s��̎擾
	m_view = Matrix::CreateLookAt(m_eyePos, m_refPos, m_upVec);

	// �v���W�F�N�V�����s��̎擾
	m_proj = Matrix::CreatePerspectiveFieldOfView(m_fovY, m_aspect, m_nearClip, m_farClip);
}

/// <summary>
/// �r���[�s��̎擾
/// </summary>
/// <returns>�s��</returns>
DirectX::SimpleMath::Matrix Camera::GetViewMatrix()
{
	return m_view;
}

/// <summary>
/// �v���W�F�N�V�����s��̎擾
/// </summary>
/// <returns>�s��</returns>
DirectX::SimpleMath::Matrix Camera::GetProjectionMatrix()
{
	return m_proj;
}

/// <summary>
/// ���_�̐ݒ�
/// </summary>
/// <param name="pos">���_</param>
void Camera::SetEyePos(DirectX::SimpleMath::Vector3 pos)
{
	m_eyePos = pos;
}

/// <summary>
/// �����_�̐ݒ�
/// </summary>
/// <param name="pos">�����_</param>
void Camera::SetRefPos(DirectX::SimpleMath::Vector3 pos)
{
	m_refPos = pos;
}

/// <summary>
/// ������x�N�g���̐ݒ�
/// </summary>
/// <param name="vec">������x�N�g��</param>
void Camera::SetUpVec(DirectX::SimpleMath::Vector3 vec)
{
	m_upVec = vec;
}

/// <summary>
/// ������������p�̐ݒ�
/// </summary>
/// <param name="fovY">������������p</param>
void Camera::SetFovY(float fovY)
{
	m_fovY = fovY;
}

/// <summary>
/// �A�X�y�N�g��̐ݒ�
/// </summary>
/// <param name="aspect">�A�X�y�N�g��</param>
void Camera::SetAspect(float aspect)
{
	m_aspect = aspect;
}

/// <summary>
/// ��O�̕\�����E�����̐ݒ�
/// </summary>
/// <param name="nearClip">��O�̕\�����E����</param>
void Camera::SetNearClip(float nearClip)
{
	m_nearClip = nearClip;
}

/// <summary>
/// ���̕\�����E�����̐ݒ�
/// </summary>
/// <param name="farClip">���̕\�����E����</param>
void Camera::SetFarClip(float farClip)
{
	m_farClip = farClip;
}