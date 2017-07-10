#include "ModelEffect.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

ModelEffectManager* ModelEffectManager::getInstance()
{
	static ModelEffectManager instance;

	return &instance;
}

void ModelEffectManager::Dispose()
{
	modelEffects.clear();
}

void ModelEffectManager::Entry(
	const wchar_t*filename,	// �t�@�C����
	int frame,	// �t���[����
	Vector3 position,	// ���W
	Vector3 velocity,	// ���x
	Vector3 accel,	// �����x
	Vector3 start_rotation, Vector3 end_rotation, // ��]�E�J�n�p�x�ƏI���p�x
	Vector3 start_scale, Vector3 end_scale // �g�k�E�J�n�{���ƏI���{��
)
{
	// �ꎞ�I�u�W�F�N�g
	ModelEffect effect;

	effect.obj.LoadModel(filename);

	effect.position = position;
	effect.velocity = velocity;
	effect.accel = accel;
	effect.s_rotation = start_rotation;
	effect.e_rotation = end_rotation;
	effect.s_scale = start_scale;
	effect.e_scale = end_scale;
	effect.frame = 0;
	effect.num_frame = frame;

	// �ꎞ�I�u�W�F�N�g�����X�g�Ɉڂ�
	modelEffects.push_back(std::move(effect));

}

void ModelEffectManager::Update()
{
	// �p�[�e�B�N����S�čX�V
	std::list<ModelEffect>::iterator it;
	for (it = modelEffects.begin(); it != modelEffects.end(); )
	{
		// �X�V
		if (it->Update(gravity))
		{
			it++;
		}
		else
		{
			// �����ɒB�����p�[�e�B�N�����������
			it = modelEffects.erase(it);
		}
	}
}

void ModelEffectManager::Draw()
{
	// �p�[�e�B�N����S�ĕ`��
	std::list<ModelEffect>::iterator it;
	for (it = modelEffects.begin(); it != modelEffects.end(); it++)
	{
		it->obj.Draw();
	}
}

ModelEffectManager::ModelEffectManager()
{
}

bool ModelEffectManager::ModelEffect::Update(const Vector3& gravity)
{
	// �o�߃t���[�������J�E���g
	frame++;
	if (frame > num_frame) return false;
	// �i�s�x��0�`1�͈̔͂Ɋ��Z
	float f = (float)num_frame / frame;

	// ���x�ɉ����x�����Z
	velocity += accel;

	// �d�͉����x�ɂ�����
	velocity += gravity;

	// ���x�ɂ��ړ�
	position += velocity;

	// �p�x�̐��`�⊮
	Vector3 rotation = (e_rotation - s_rotation) / f;
	rotation += s_rotation;
	rotation.x = XMConvertToRadians(rotation.x);
	rotation.y = XMConvertToRadians(rotation.y);
	rotation.z = XMConvertToRadians(rotation.z);

	// �X�P�[���̐��`���
	Vector3 scale = (e_scale - s_scale) / f;
	scale += s_scale;

	obj.SetTranslate(position);
	obj.SetRotate(rotation);
	obj.SetScale(scale);
	obj.Update();

	return true;
}
