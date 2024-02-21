#include "pch.h"
#include "EngineManager.h"
#include "Engine.h"

#pragma region public
EngineManager::~EngineManager()
{
	for (auto val : _mp)
		delete val.second;
}
void EngineManager::addEngine(int engineID, Engine* engine) { _mp[engineID] = engine; }
void EngineManager::run() { for (auto val : _mp) val.second->run(); }
void EngineManager::run(int engineID) { _mp[engineID]->run(); }
void EngineManager::shutdown(int engineID) { delete _mp[engineID]; _mp[engineID] = nullptr; }
void EngineManager::send(int engineID, int serial, Size blockSize, char* data) { _mp[engineID]->send(serial, blockSize, data); }
void EngineManager::disconnect(int engineID, int serial) { _mp[engineID]->disconnect(serial); }
#pragma endregion

#pragma region private
#pragma endregion

