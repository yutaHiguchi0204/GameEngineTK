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
	// �X�P�[�����O
	// ��]�p
	// ���[���h�s��

public:
	Obj3d();										// �R���X�g���N�^

	void Update();									// �X�V
	void Draw();									// �`��

	void LoadModel(const wchar_t* fileName);		// ���f���̃��[�h

	// �X�P�[�����O�̐ݒ�
	// ��]�p�̐ݒ�
	// ���s�ړ��̐ݒ�

	// �X�P�[�����O���擾
	// ��]�p���擾
	// ���s�ړ��̎擾
	// ���[���h�s��̎擾

};