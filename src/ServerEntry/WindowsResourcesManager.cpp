#include <stdafx.h>
#include <WindowsResourcesManager.h>

WindowsResourcesManager* WindowsResourcesManager::getInstance(){
	if(instance == nullptr)
		instance = new WindowsResourcesManager();
	return instance;
}

void WindowsResourcesManager::releaseInstance(){
	if(instance != nullptr)
		delete instance;
	instance = nullptr;
}

WindowsResourcesManager* WindowsResourcesManager::instance = nullptr;

WindowsResourcesManager::WindowsResourcesManager(){

}

WindowsResourcesManager::~WindowsResourcesManager(){

}