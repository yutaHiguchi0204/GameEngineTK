//
// CollisionNode.cpp
// �Փ˔���m�[�h
//

// �w�b�_�t�@�C���̃C���N���[�h
#include "CollisionNode.h"

// ���O���
using namespace DirectX::SimpleMath;

/// <summary>
/// CollisionNode�̐e�ݒ�֐�
/// </summary>
/// <param name="parent">�e</param>
void CollisionNode::SetParent(Obj3d* parent)
{
	m_obj.SetParent(parent);
}

/// <summary>
/// SphereNode�̃R���X�g���N�^
/// </summary>
SphereNode::SphereNode()
{
	m_radius = 1.0f;
}

/// <summary>
/// SphereNode�̏�����
/// </summary>
void SphereNode::Initialize()
{
	// �f�o�b�O�\���p���f����ǂݍ���
	m_obj.LoadModel(L"Resources/sphereNode.cmo");
}

/// <summary>
/// SphereNode�̍X�V
/// </summary>
void SphereNode::Update()
{
	m_obj.SetTranslate(m_trans);
	m_obj.SetScale(Vector3(m_radius));

	m_obj.Update();

	// ���[���h���W�ł̓����蔻�苅���v�Z
	{
		// �v�Z�ς݂̃��[���h�s����v�Z
		const Matrix& worldM = m_obj.GetWorldMatrix();

		// ���f�����W�n�ł̒��S�_
		Vector3 center(0, 0, 0);

		// ���f�����W�n�ł̉E�[
		Vector3 right(1, 0, 0);

		// ���[���h���W�ɕϊ�
		center = Vector3::Transform(center, worldM);
		right = Vector3::Transform(right, worldM);

		// �����蔻�苅�Ɍ��ʂ���
		Sphere::center = center;
		Sphere::radius = Vector3::Distance(center, right);
	}
}

/// <summary>
/// SphereNode�̕`��
/// </summary>
void SphereNode::Draw()
{
	m_obj.Draw();
}