//
// Obj3d.h
// �R�c�I�u�W�F�N�g�̃N���X
//
#pragma once

// �w�b�_�t�@�C���̃C���N���[�h
#include <d3d11_1.h>
#include <Windows.h>
#include <wrl\client.h>

#include <CommonStates.h>
#include <Effects.h>
#include <SimpleMath.h>
#include <Model.h>

#include "Camera.h"

// �N���X�̒�`
class Obj3d
{
private:
	static Microsoft::WRL::ComPtr<ID3D11Device>            m_d3dDevice;			// �f�o�C�X
	static Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_d3dContext;		// �f�o�C�X�R���e�L�X�g

	static Camera* m_camera;													// �J����

	static std::unique_ptr<DirectX::CommonStates> m_states;						// �ėp�`��X�e�[�g

	static std::unique_ptr<DirectX::EffectFactory> m_factory;					// �G�t�F�N�g�t�@�N�g��

public:
	// �ÓI�����o�ϐ��̏�����
	static void InitializeStatic(
		Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice, 
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext,
		Camera* camera
	);

private:

	std::unique_ptr<DirectX::Model>		m_model;															// �R�c���f��

	DirectX::SimpleMath::Vector3		m_scale;															// �X�P�[�����O
	DirectX::SimpleMath::Vector3		m_rotate;															// ��]�p
	DirectX::SimpleMath::Vector3		m_translate;														// ���s�ړ�

	DirectX::SimpleMath::Matrix			m_world;															// ���[���h�s��
	Obj3d*								m_pParent;															// �e�̂R�c�I�u�W�F�N�g

public:
	Obj3d();																								// �R���X�g���N�^

	void Update();																							// �X�V
	void Draw();																							// �`��

	void LoadModel(const wchar_t* fileName);																// ���f���̃��[�h

	void SetScale(const DirectX::SimpleMath::Vector3& scale)			{ m_scale = scale; };				// �X�P�[�����O�̐ݒ�
	void SetRotate(const DirectX::SimpleMath::Vector3& rotate)			{ m_rotate = rotate; };				// ��]�p�̐ݒ�
	void SetTranslate(const DirectX::SimpleMath::Vector3& translate)	{ m_translate = translate; };		// ���s�ړ��̐ݒ�

	void SetParent(Obj3d* obj)											{ m_pParent = obj; };				// �e�̂R�c�I�u�W�F�N�g��ݒ�

	const DirectX::SimpleMath::Vector3 GetScale()						{ return m_scale; };				// �X�P�[�����O���擾
	const DirectX::SimpleMath::Vector3 GetRotate()						{ return m_rotate; };				// ��]�p���擾
	const DirectX::SimpleMath::Vector3 GetTranslate()					{ return m_translate; };			// ���s�ړ��̎擾

	const DirectX::SimpleMath::Matrix GetWorldMatrix()					{ return m_world; };				// ���[���h�s��̎擾

	Obj3d* GetParent()													{ return m_pParent; };				// �e�̂R�c�I�u�W�F�N�g�̎擾
};