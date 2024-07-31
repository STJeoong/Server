#include "pch.h"
#include "Engine.h"
#include "SFUtil.h"
#include "StringConversion.h"
#include "Define.h"
#include "I_EngineEventContainer.h"
#include "NoWaitEngineEventContainer.h"
#include "WaitEngineEventContainer.h"
#include "NetworkLayer.h"
#include "Decoder.h"
#include "Encoder.h"
#include "IOCPClient.h"
#include "IOCPServer.h"
#include "ServerMode.h"
#include "PassiveMode.h"
#include "PhysicsMode.h"
#include "ThreadPool.h"
#include "MemoryBlockPool.h"
#include <fstream>

#pragma region public
void Engine::setLogFolder(const char* argv0)
{
	static bool isSet = false;
	if (isSet)
		return;
	isSet = true;

	google::InitGoogleLogging(argv0);
	LOG(INFO) << "Server Initialize...";

	TCHAR programPath[MAX_PATH] = {};
	GetModuleFileName(NULL, programPath, MAX_PATH);

	TCHAR* dirPath = SFUtil::ExtractPathInfo(programPath, SFUtil::PATH_DIR);
	SetCurrentDirectory(dirPath);

	tstring logPath = dirPath;
	logPath += _T("Log\\");
	tstring infoPath = logPath + _T("INFO\\");
	tstring warningPath = logPath + _T("WARNING\\");
	tstring errorPath = logPath + _T("ERROR\\");
	tstring fatalPath = logPath + _T("FATAL\\");

	CreateDirectory(logPath.c_str(), NULL);
	CreateDirectory(infoPath.c_str(), NULL);
	CreateDirectory(warningPath.c_str(), NULL);
	CreateDirectory(errorPath.c_str(), NULL);
	CreateDirectory(fatalPath.c_str(), NULL);

	google::SetLogDestination(google::GLOG_INFO, StringConversion::ToASCII(infoPath).c_str());
	google::SetLogDestination(google::GLOG_WARNING, StringConversion::ToASCII(warningPath).c_str());
	google::SetLogDestination(google::GLOG_ERROR, StringConversion::ToASCII(errorPath).c_str());
	google::SetLogDestination(google::GLOG_FATAL, StringConversion::ToASCII(fatalPath).c_str());
	LOG(INFO) << "Log Destination " << StringConversion::ToASCII(logPath);
}
void Engine::setServerMode(E_ServerMode mode)
{
	static bool isSet = false;
	if (isSet)
		return;

	switch (mode)
	{
	case E_ServerMode::PASSIVE_MODE: s_evtContainer = new WaitEngineEventContainer(); s_serverMode = new PassiveMode(); break;
	case E_ServerMode::PHYSICS_MODE: s_evtContainer = new NoWaitEngineEventContainer(); s_serverMode = new PhysicsMode(); break;
	}

	isSet = true;
	MemoryBlockPool::makePool(static_cast<int>(Size::_128));
	MemoryBlockPool::makePool(static_cast<int>(Size::_256));
	MemoryBlockPool::makePool(static_cast<int>(Size::_1024));
	MemoryBlockPool::makePool(static_cast<int>(Size::_8192));
	s_serverMode->setEventContainer(s_evtContainer);
}
bool Engine::addEngine(int engineID, const S_ServerConfig& config, I_Broadcaster* broadcaster)
{
	if (s_engines[engineID] != nullptr || broadcaster == nullptr || engineID >= Engine::MAX_ENGINE || engineID < 0)
		return false;
	s_engines[engineID] = Engine::parseConfig(engineID, config);
	s_serverMode->addBroadcaster(engineID, broadcaster);
	return true;
}
const S_ServerConfig& Engine::getEngineConfig(int engineID) { return s_engines[engineID]->_config; }
void Engine::runEngine()
{
	for (int i = 0; i < Engine::MAX_ENGINE; ++i)
		if (s_engines[i] != nullptr && s_engines[i]->_state == E_EngineState::INITIAL)
			s_engines[i]->run();
}
void Engine::runEngine(int engineID)
{
	if (s_engines[engineID] == nullptr)
		return;
	s_engines[engineID]->run();
}
void Engine::runServer() { s_serverMode->run(); }
void Engine::shutdown(int engineID)
{
	if (s_engines[engineID] == nullptr)
		return;
	delete s_engines[engineID];
	s_engines[engineID] = nullptr;
}
void Engine::send(int engineID, int serial, Size blockSize, char* data) { s_engines[engineID]->send(serial, blockSize, data); }
void Engine::disconnect(int engineID, int serial) { s_engines[engineID]->disconnect(serial); }
void Engine::setTimer(int engineID, int serial, int millisec, int evt)
{
	ThreadPool::enqueue([engineID, serial, millisec, evt]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(millisec));
		S_EngineEvent engineEvt = {};
		engineEvt.serial = serial;
		engineEvt.type = E_EngineEventType::EVENT_TIMER;
		engineEvt.data = (char*)((size_t)evt);
		s_evtContainer->enqueue(engineID, engineEvt); });
}
void Engine::setRepetitiveTimer(int engineID, int serial, int millisec, int evt)
{
	ThreadPool::enqueue([engineID, serial, millisec, evt]() {
		S_EngineEvent engineEvt = {};
		engineEvt.serial = serial;
		engineEvt.type = E_EngineEventType::EVENT_TIMER;
		engineEvt.data = (char*)((size_t)evt);
		while (true)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(millisec));
			s_evtContainer->enqueue(engineID, engineEvt);
		}});
}
#pragma endregion

#pragma region private
Engine* Engine::parseConfig(int engineID, const S_ServerConfig& config)
{
	Engine* ret = nullptr;
	I_NetworkCore* core = nullptr;

	if (config.network == "iocp_server")
		core = new IOCPServer(config.ip, config.port, config.maxClient);
	else if (config.network == "iocp_client")
		core = new IOCPClient(config.ip, config.port);
	else
	{
		printf("%s is not supported\n", config.network.c_str());
		return nullptr;
	}

	ret = new Engine(engineID, core, config.maxClient, s_evtContainer);
	ret->_config = config;
	return ret;
}

Engine::Engine(int id, I_NetworkCore* core, int maxClient, I_EngineEventContainer* container)
{
	_decoder = new Decoder(id, container, maxClient);
	_network = new NetworkLayer(id, core, container, _decoder);
	_encoder = new Encoder(_network);
}
Engine::~Engine()
{
	_state = E_EngineState::SHUTDOWN;
	delete _network;
	delete _decoder;
	delete _encoder;
}
void Engine::run() { _state = E_EngineState::RUN; _network->run(); }
void Engine::send(int to, Size blockSize, char* data) { _encoder->enqueue(to, blockSize, data); }
void Engine::disconnect(int serial) { _network->disconnect(serial); }
#pragma endregion

Engine* Engine::s_engines[Engine::MAX_ENGINE] = {};
ServerMode* Engine::s_serverMode = nullptr;
I_EngineEventContainer* Engine::s_evtContainer = nullptr;