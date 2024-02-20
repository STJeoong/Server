#include "Server.h"
#include "User.h"
#include "LobbyServerHandler.h"
#include "DBClientHandler.h"
#include "UserManager.h"
#include "Serializer.h"
#include "E_EngineType.h"
#include <EngineManager.h>
#include <fstream>
#include <MemoryBlockPoolManager.h>
#include <SFUtil.h>
#include <Engine.h>
#include <StringConversion.h>
#include <IOCPServer.h>
#include <IOCPClient.h>

#pragma region public
void Server::init(const char* argv0)
{
	_engineManager = new EngineManager;
	_lobbyHandler = new LobbyServerHandler;
	_dbHandler = new DBClientHandler;
	_serializer = new Serializer;

	google::InitGoogleLogging(argv0);
	LOG(INFO) << "Server Initialize...";
	this->setLogFolder();
	this->setConfig();
	this->setEngine();
}
void Server::run()
{
	_engineManager->run();
	int type;
	S_EngineEvent evt;
	while (true)
	{
		std::tie(type, evt) = _engineManager->getEvent();
		switch ((E_EngineType)type)
		{
		case E_EngineType::LOBBY_SERVER: _lobbyHandler->handle(evt); break;
		case E_EngineType::DB_CLIENT: _dbHandler->handle(evt); break;
		}
		if (evt.data != nullptr)
			MemoryBlockPoolManager::getInstance().release(evt.blockSize, evt.data);
	}
}
void Server::send(E_EngineType type, int serial, S_PacketAttr attr, const google::protobuf::Message& messsage) const
{
	std::pair<Size, char*> val = _serializer->serialize(attr, messsage);
	_engineManager->send((int)type, serial, val.first, val.second);
}
#pragma endregion

#pragma region private
void Server::setLogFolder()
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
void Server::setConfig()
{
	try
	{
		std::ifstream f("config.json");
		_json = json::parse(f);
	}
	catch (std::exception e)
	{
		LOG(ERROR) << "invalid config.json";
	}
}
void Server::setEngine()
{
	S_ServerConfig dbConfig = {};

	_json["server"].get_to(_config);
	_json["dbClient"].get_to(dbConfig);
	_engineManager->addEngine((int)E_EngineType::LOBBY_SERVER, new Engine(new IOCPServer(_config.ip, _config.port, _config.maxClient), _config.maxClient));
	_engineManager->addEngine((int)E_EngineType::DB_CLIENT, new Engine(new IOCPClient(dbConfig.ip, dbConfig.port), 1));
}
#pragma endregion