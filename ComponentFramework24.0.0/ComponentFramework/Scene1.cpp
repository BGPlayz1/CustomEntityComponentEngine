#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "Scene1.h"
#include <MMath.h>
#include "Debug.h"
#include "MeshComponent.h"
#include "ShaderComponent.h"
#include "TransformComponent.h"
#include "CameraActor.h"
#include "Body.h"
#include "Actor.h"
#include "AssetManager.h"
#include "MaterialComponent.h"
#include <QMath.h>
Scene1::Scene1() : drawInWireMode{ false } {
	Debug::Info("Created Scene1: ", __FILE__, __LINE__);
}

Scene1::~Scene1() {
	Debug::Info("Deleted Scene1: ", __FILE__, __LINE__);
}

bool Scene1::OnCreate() {
    Debug::Info("Loading assets Scene1: ", __FILE__, __LINE__);

	actors["checkerBoard"] = std::make_shared<Actor>(nullptr);
	camera = std::make_shared <CameraActor>(nullptr);
	camera->OnCreate();	
	assetManager = std::make_shared<AssetManager>();
	assetManager->OnCreate();

    if (!actors["checkerBoard"]) { return false; }

    actors["checkerBoard"]->AddComponent<MeshComponent>(nullptr, "meshes/Plane.obj");
    actors["checkerBoard"]->AddComponent<ShaderComponent>(assetManager->GetComponent<ShaderComponent>("ShaderComponent"));
    actors["checkerBoard"]->AddComponent<TransformComponent>(nullptr, QMath::angleAxisRotation(0, Vec3(0.0f, 1.0f, 0.0f)), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.4f, 0.4f, 0.4f));
    actors["checkerBoard"]->AddComponent<MaterialComponent>(nullptr, "textures/8x8_checkered_board.png");
    actors["checkerBoard"]->OnCreate();



    const float boardSize = 8.0f;  // Assuming an 8x8 board
    const float spaceSize = 1.25f;
    const float spacingBetweenBlackSpaces = 2.5f;

    float baseX = -4.35f;
    float baseY = -4.35f;

	for (int i = 0; i < boardSize; ++i) {
		for (int j = 0; j < boardSize; ++j) {
			// Calculate position for each piece
			float x = baseX + j * spaceSize;
			float y = baseY + i * spaceSize;

			// Check if the space is black
			if ((i + j) % 2 == 0 && i != 3 && i != 4) {
				std::string actorName = "checkerPiece_" + std::to_string(i) + "_" + std::to_string(j);
				actors[actorName] = std::make_shared<Actor>(actors["checkerBoard"]); // Add checker piece to actors map
				actors[actorName]->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("MeshComponent"));
				actors[actorName]->AddComponent<ShaderComponent>(assetManager->GetComponent<ShaderComponent>("ShaderComponent"));
				actors[actorName]->AddComponent<TransformComponent>(nullptr, QMath::angleAxisRotation(180.0f, Vec3(0.0f, 1.0f, 0.0f)), Vec3(x, y, 0.0f), Vec3(0.13f, 0.13f, 0.13f));
				actors[actorName]->ListComponents();

				if (i < 3) {
					actors[actorName]->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("MaterialComponent1"));
				}
				else if (i > 4) {
					actors[actorName]->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("MaterialComponent2"));
				}
				actors[actorName]->OnCreate();

			}
		}
	}
		return true;
}

void Scene1::OnDestroy() {
	Debug::Info("Deleting assets Scene1: ", __FILE__, __LINE__);

}

void Scene1::HandleEvents(const SDL_Event& sdlEvent) {
	switch (sdlEvent.type) {
	case SDL_KEYDOWN:
		switch (sdlEvent.key.keysym.scancode) {
		case SDL_SCANCODE_W:
			drawInWireMode = !drawInWireMode;
			break;
		}
		break;

	case SDL_MOUSEMOTION:
		if (sdlEvent.button.button == SDL_BUTTON_LEFT) {
			if (isPicking) {
				pickedActorName = Pick(mousePosX, mousePosY);

				if (pickedActorName.substr(0, 13) == "checkerPiece_") {
					SDL_GetMouseState(&mousePosX, &mousePosY);
					int width, height;
					SDL_GetWindowSize(SDL_GL_GetCurrentWindow(), &width, &height);
					// Convert screen coordinates to normalized device coordinates (NDC)
					float x_ndc = (2.0f * (float)mousePosX) / (float)width - 1.0f;
					float y_ndc = 1.0f - (2.0f * (float)mousePosY) / (float)height;

					Vec4 clipCoords = Vec4(x_ndc, y_ndc, -1.0f, 1.0f);

					// Convert clip coordinates to eye (camera) coordinates
					Matrix4 invProjection = MMath::inverse(camera->GetProjectionMatrix());
					Vec4 eyeCoords = invProjection * clipCoords;
					eyeCoords.z = -1.0f; // Assuming the point lies on the near plane
					eyeCoords.w = 0.0f;

					// Convert eye coordinates to world coordinates
					Matrix4 invView = MMath::inverse(camera->GetViewMatrix());
					Vec4 worldCoords = invView * eyeCoords;
					
					worldCoords.x *= 12.5f;
					worldCoords.y *= 12.5f;
					actors[pickedActorName]->GetComponent<TransformComponent>()->SetPosition(Vec3(worldCoords.x, worldCoords.y, 0.0f));
				}
			}
		}
		break;

	case SDL_MOUSEBUTTONDOWN:
		if (sdlEvent.button.button == SDL_BUTTON_LEFT) {
			SDL_GetMouseState(&mousePosX, &mousePosY);
			isPicking = true;
		}
		break;

	case SDL_MOUSEBUTTONUP:
		if (sdlEvent.button.button == SDL_BUTTON_LEFT) {
			isPicking = false;
		}
		break;

	default:
		break;
	}
}

void Scene1::Update(const float deltaTime) {
	
	
}


std::string Scene1::Pick(int x, int y) {
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f); /// Paint the backgound white which is 0x00FFFFFF
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Ref<ShaderComponent> shader = assetManager->GetComponent<ShaderComponent>("ColorPickingShader");
	glUseProgram(shader->GetProgram());
	glUniformMatrix4fv(shader->GetUniformID("projectionMatrix"), 1, GL_FALSE, camera->GetProjectionMatrix());
	glUniformMatrix4fv(shader->GetUniformID("viewMatrix"), 1, GL_FALSE, camera->GetViewMatrix());

	GLuint i = 1;
	for (const auto& actorPair : actors) {
		std::shared_ptr<Actor> actor = actorPair.second;

		glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, actor->GetModelMatrix());
		glUniform1ui(shader->GetUniformID("colorID"), i);
		actor->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);
		actor->colorID = i;

		i++;		
	}


	GLuint colorIndex;
	glReadPixels(x, 720 - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &colorIndex);
	colorIndex &= 0x00FFFFFF; /// This zeros out the alpha component
	//std::cout << "clicked on " << colorIndex << std::endl;

	glUseProgram(0);
	if (colorIndex == 0x00FFFFFF)
		return ""; // No actor picked
	else {
		// Subtract 1 from colorIndex to map it back to the correct actor index
		for (const auto& actorPair : actors) {
			std::shared_ptr<Actor> actor = actorPair.second;
			if (actor->colorID == colorIndex)
				return actorPair.first; // Return the name of the picked actor
		}
	}

}

void Scene1::Render() const {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	/// Set the background color then clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (drawInWireMode) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

		// Get shader component from actor
		glUseProgram(assetManager->GetComponent<ShaderComponent>("ShaderComponent")->GetProgram());

		// Set uniforms
		glUniformMatrix4fv(assetManager->GetComponent<ShaderComponent>("ShaderComponent")->GetUniformID("projectionMatrix"), 1, GL_FALSE, camera->GetProjectionMatrix());
		glUniformMatrix4fv(assetManager->GetComponent<ShaderComponent>("ShaderComponent")->GetUniformID("viewMatrix"), 1, GL_FALSE, camera->GetViewMatrix());

	for (const auto& actorPair : actors) {
		std::shared_ptr<Actor> actor = actorPair.second;

		glUniformMatrix4fv(assetManager->GetComponent<ShaderComponent>("ShaderComponent")->GetUniformID("modelMatrix"), 1, GL_FALSE, actor->GetModelMatrix());

		// Bind texture
		glBindTexture(GL_TEXTURE_2D, actor->GetComponent<MaterialComponent>()->GetTextureID());

		// Render actor
		actor->Render();
	}

	glUseProgram(0);
}




