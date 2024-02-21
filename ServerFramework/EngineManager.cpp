#include "pch.h"
#include "EngineManager.h"
#include "Engine.h"
#include "SFUtil.h"
#include "StringConversion.h"
#include "IOCPServer.h"
#include "IOCPClient.h"

#pragma region public
EngineManager::EngineManager(const char* argv0)
{
	google::InitGoogleLogging(argv0);
	LOG(INFO) << "Server Initialize...";
	this->setLogFolder();
}
EngineManager::~EngineManager() { for (auto val : _mp) delete val.second; }
void EngineManager::addEngine(int engineID, const S_ServerConfig& config, EngineEventContainer* container) { _mp[engineID] = this->parseConfig(engineID, config, container); }
void EngineManager::run() { for (auto val : _mp) val.second->run(); }
void EngineManager::run(int engineID) { _mp[engineID]->run(); }
void EngineManager::shutdown(int engineID) { delete _mp[engineID]; _mp[engineID] = nullptr; }
void EngineManager::send(int engineID, int serial, Size blockSize, char* data) { _mp[engineID]->send(serial, blockSize, data); }
void EngineManager::disconnect(int engineID, int serial) { _mp[engineID]->disconnect(serial); }
#pragma endregion

#pragma region private
void EngineManager::setLogFolder()
{
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
Engine* EngineManager::parseConfig(int engineID, const S_ServerConfig& config, EngineEventContainer* container)
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

	ret = new Engine(engineID, core, config.maxClient, container);
	return ret;
}
#pragma endregion

