//
// Obj3d.h
// �R�c�I�u�W�F�N�g�̃N���X
//

// �w�b�_�t�@�C���̃C���N���[�h
#include "Obj3d.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace std;

// �ÓI�����o�̒�`
Microsoft::WRL::ComPtr<ID3D11Device> Obj3d::m_d3dDevice;			// �f�o�C�X
Microsoft::WRL::ComPtr<ID3D11DeviceContext> Obj3d::m_d3dContext;	// �f�o�C�X�R���e�L�X�g
Camera* Obj3d::m_camera;											// �J����
std::unique_ptr<DirectX::CommonStates> Obj3d::m_states;				// �ėp�`��X�e�[�g
std::unique_ptr<DirectX::EffectFactory> Obj3d::m_factory;			// �G�t�F�N�g�t�@�N�g��

/// <summary>
/// �ÓI�����o�ϐ��̏�����
/// </summary>
/// <param name="d3dDevice">�f�o�C�X</param>
/// <param name="d3dContext">�f�o�C�X�R���e�L�X�g</param>
/// <param name="camera">�J����</param>
void Obj3d::InitializeStatic(Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext, Camera* camera)
{
	m_d3dDevice = d3dDevice;
	m_d3dContext = d3dContext;
	m_camera = camera;

	// �ėp�`��X�e�[�g����
	m_states = make_unique<CommonStates>(m_d3dDevice.Get());

	// �G�t�F�N�g�t�@�N�g������
	m_factory = make_unique<EffectFactory>(m_d3dDevice.Get());
	m_factory->SetDirectory(L"Resources");
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
Obj3d::Obj3d()
{

}

/// <summary>
/// �X�V����
/// </summary>
void Obj3d::Update()
{

}

/// <summary>
/// �`�揈��
/// </summary>
void Obj3d::Draw()
{

}

void Obj3d::LoadModel(const wchar_t * fileName)
{
}
