#ifndef SCENE1_H
#define SCENE1_H
#include "Scene.h"
#include "Component.h"
#include <Vector.h>
#include <Matrix.h>
#include <unordered_map>
using namespace MATH;

/// Forward declarations 
union SDL_Event;
class Body;
class Actor;
class CameraActor;
class AssetManager;

class Scene1 : public Scene {
private:
	Ref<AssetManager> assetManager;
	std::unordered_map<std::string, Ref<Actor>> actors;
	Matrix4 projectionMatrix;
	Matrix4 viewMatrix;
	Matrix4 modelMatrix;
	bool drawInWireMode;
	Ref<CameraActor> camera;
	int mousePosX, mousePosY;
	bool isPicking;
	std::string pickedActorName;

public:
	explicit Scene1();
	virtual ~Scene1();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;
	Vec3 ScreenToWorldCoordinates(int mouseX, int mouseY);
	std::string Pick(int x, int y);
};


#endif // Scene1_H