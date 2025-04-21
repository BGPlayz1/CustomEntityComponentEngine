#pragma once
#include "Component.h"
#include <Vector.h>
#include <Quaternion.h>
#include <Matrix.h>
#include <MMath.h>
using namespace MATH;

class TransformComponent : public Component {
private:
	Quaternion orientation;
	Vec3 translate;
	Vec3 scale;
	Matrix4 modelMatrix;

public:
	TransformComponent(Ref<Component> parent_, Quaternion orientation_, Vec3 translate_, Vec3 scale_);
	~TransformComponent();
	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;

	void Rotate(float angle, Vec3 rotationAxis);

	Matrix4 GetModelMatrix() {
		return MMath::translate(translate) * MMath::scale(scale) * MMath::toMatrix4(orientation);

	};

	Quaternion GetOrientation() { return orientation; }
	void SetOrientation(Quaternion orientation_) { orientation = orientation_; }
	Vec3 GetPosition() { return translate; }
	void SetPosition(Vec3 translate_) { translate = translate_; }
};
