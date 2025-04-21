#pragma once
#include <unordered_map>
#include "Component.h"
#include "Debug.h"
class AssetManager {
private:
	std::unordered_map<const char*, Ref<Component>> componentCatalog;

public:
	AssetManager();
	~AssetManager();
	bool OnCreate();
	void RemoveAllComponents();
	void ListAllComponents();

	template<typename ComponentTemplate, typename ... Args>
	void AddComponent(const char* name, Args&& ... args_) {
		/// before you add the component ask if you have the component in the list already,
		/// if so - don't add a second one. 
		if (GetComponent<ComponentTemplate>(name).get() != nullptr) {
#ifdef _DEBUG
			std::cerr << "WARNING: Trying to add a component type that is already added - ignored\n";
#endif
			return;
		}
		/// Finish building the component and add the component to the list
		Ref<ComponentTemplate> temp = std::make_shared<ComponentTemplate>(std::forward<Args>(args_)...);
		componentCatalog[name] = temp;
	}

	template<typename ComponentTemplate>
	Ref<ComponentTemplate> GetComponent(const char* name) const {
		auto id = componentCatalog.find(name);
		if (id == componentCatalog.end()) {
			Debug::Error("Can't find  your component", __FILE__, __LINE__);
			return Ref<ComponentTemplate>(nullptr);
		}
		return std::dynamic_pointer_cast<ComponentTemplate>(id->second);
	}
	
};