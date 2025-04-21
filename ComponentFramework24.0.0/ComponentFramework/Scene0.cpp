#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "Scene0.h"
#include <MMath.h>
#include "Debug.h"
#include "MeshComponent.h"
#include "ShaderComponent.h"
#include "TransformComponent.h"
#include "CameraActor.h"
#include "Body.h"
#include "Actor.h"
#include "MaterialComponent.h"
#include <QMath.h>
Scene0::Scene0() :sphere{nullptr}, shader{nullptr},
drawInWireMode{false} {
	Debug::Info("Created Scene0: ", __FILE__, __LINE__);
}

Scene0::~Scene0() {
	Debug::Info("Deleted Scene0: ", __FILE__, __LINE__);
}

bool Scene0::OnCreate() {
	Debug::Info("Loading assets Scene0: ", __FILE__, __LINE__);

	/*	mario = new Actor(nullptr);
		camera = new CameraActor(nullptr);
		camera->OnCreate();

	if (!mario) { return false; } 

		mario->AddComponent<MeshComponent>(nullptr, "meshes/Mario.obj");
		mario->AddComponent<ShaderComponent>(nullptr, "shaders/textureVert.glsl", "shaders/textureFrag.glsl");
		mario->AddComponent<TransformComponent>(nullptr, QMath::angleAxisRotation(180, Vec3(0.0f, 1.0f, 0.0f)), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
		mario->AddComponent<MaterialComponent>(nullptr, "textures/mario_mime.png");
		mario->ListComponents();
		mario->OnCreate();
	
		hammer = new Actor(mario);
		if (!hammer) { return false; }
		hammer->AddComponent<MeshComponent>(nullptr, "meshes/Hammer.obj");
		hammer->AddComponent<ShaderComponent>(nullptr, "shaders/textureVert.glsl", "shaders/textureFrag.glsl");
		hammer->AddComponent<TransformComponent>(nullptr, QMath::angleAxisRotation(-90.0f, Vec3(1.0f, 0.0f, 0.0f)), Vec3(1.1f, 0.1f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
		hammer->AddComponent<MaterialComponent>(nullptr, "textures/hammer_BaseColor.png");
		hammer->ListComponents();
		hammer->OnCreate();*/
	return true;
}

void Scene0::OnDestroy() {
	Debug::Info("Deleting assets Scene0: ", __FILE__, __LINE__);
	sphere->OnDestroy();
	delete sphere;


	mario->GetComponent<ShaderComponent>()->OnDestroy();

	camera->OnDestroy();
}

void Scene0::HandleEvents(const SDL_Event &sdlEvent) {
	switch( sdlEvent.type ) {
    case SDL_KEYDOWN:
		switch (sdlEvent.key.keysym.scancode) {
			case SDL_SCANCODE_W:
				drawInWireMode = !drawInWireMode;
				break;
		}
		break;

	case SDL_MOUSEMOTION:          
		break;

	case SDL_MOUSEBUTTONDOWN:              
		break; 

	case SDL_MOUSEBUTTONUP:            
	break;

	default:
		break;
    }
}

void Scene0::Update(const float deltaTime) {
	static float angle = 1;
	mario->GetComponent<TransformComponent>()->Rotate(angle, Vec3(0.0f, 1.0f, 0.0f));
	
}

void Scene0::Render() const {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	/// Set the background color then clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(drawInWireMode){
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}else{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	Ref<ShaderComponent> shaderComponent = mario->GetComponent<ShaderComponent>();
	glUseProgram(shaderComponent->GetProgram());
	glUniformMatrix4fv(shaderComponent->GetUniformID("projectionMatrix"), 1, GL_FALSE, camera->GetProjectionMatrix());
	glUniformMatrix4fv(shaderComponent->GetUniformID("viewMatrix"), 1, GL_FALSE, camera->GetViewMatrix());
	glUniformMatrix4fv(shaderComponent->GetUniformID("modelMatrix"), 1, GL_FALSE, mario->GetModelMatrix());
	glBindTexture(GL_TEXTURE_2D, mario->GetComponent<MaterialComponent>()->GetTextureID());
	mario->Render();

	glUniformMatrix4fv(shaderComponent->GetUniformID("modelMatrix"), 1, GL_FALSE, hammer->GetModelMatrix());
	glBindTexture(GL_TEXTURE_2D, hammer->GetComponent<MaterialComponent>()->GetTextureID());
	hammer->Render();

	glUseProgram(0);
}



	
