#pragma once
#include <glew.h>
#include "Component.h"
class MaterialComponent : public Component {
	MaterialComponent(const MaterialComponent&) = delete;
	MaterialComponent(MaterialComponent&&) = delete;
	MaterialComponent& operator = (const MaterialComponent&) = delete;
	MaterialComponent& operator = (MaterialComponent&&) = delete;
private:
    
	GLuint textureID;
	const char* filename;
public:
	MaterialComponent(Ref<Component> parent_, const char* filename_);
	~MaterialComponent();
	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const;
	GLuint GetTextureID() { return textureID; }

};

