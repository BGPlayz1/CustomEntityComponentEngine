#include "AssetManager.h"
#include "ShaderComponent.h"
#include "MeshComponent.h"
#include "MaterialComponent.h"

AssetManager::AssetManager() {
	AddComponent<ShaderComponent>("ShaderComponent", nullptr, "shaders/textureVert.glsl", "shaders/textureFrag.glsl");
	AddComponent<ShaderComponent>("ColorPickingShader", nullptr, "shaders/ColorPickerVert.glsl", "shaders/ColorPickerFrag.glsl");
	AddComponent<MeshComponent>("MeshComponent", nullptr, "meshes/CheckerPiece.obj");
	AddComponent<MaterialComponent>("MaterialComponent1", nullptr, "textures/whiteCheckerPiece.png");
	AddComponent<MaterialComponent>("MaterialComponent2", nullptr, "textures/blackCheckerPiece.png");

}

AssetManager::~AssetManager()
{
}

bool AssetManager::OnCreate() {
	for (std::pair<const char*, Ref<Component>> c : componentCatalog) {
		if (c.second->OnCreate() == false) {
			return false;
		}
	}
	return true;
}

void AssetManager::RemoveAllComponents() {
	componentCatalog.clear();
}

void AssetManager::ListAllComponents() {
	for (auto c : componentCatalog) {
		std::cout << c.first << "; " << typeid(*(c.second.get())).name() << std::endl;
	}
}