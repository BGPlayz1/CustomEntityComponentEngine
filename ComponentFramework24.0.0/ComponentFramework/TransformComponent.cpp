#include "TransformComponent.h"
#include <QMath.h>
TransformComponent::TransformComponent(Ref<Component> parent_, Quaternion orientation_, Vec3 translate_, Vec3 scale_ = Vec3(1.0f, 1.0f, 1.0f)) :
	Component(parent_), orientation(orientation_), translate(translate_), scale(scale_)
{
	/*translate = Vec3(0.0f, 0.0f, 0.0f);
	orientation = Quaternion(1.0f, Vec3(0.0f, 0.0f, 0.0f));*/

}

TransformComponent::~TransformComponent() {}

bool TransformComponent::OnCreate() {

	isCreated = true;
	return true;
}

void TransformComponent::OnDestroy() {}

void TransformComponent::Update(const float deltaTime) {}

void TransformComponent::Render() const
{
}

void TransformComponent::Rotate(float angle, Vec3 rotationAxis)
{
	Quaternion rotate = QMath::angleAxisRotation(angle, rotationAxis);
	orientation *= rotate;
}


