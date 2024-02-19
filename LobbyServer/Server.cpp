#include "Server.h"
#include "User.h"
#include "PacketHandler.h"
#include "UserManager.h"
#include "Serializer.h"
#include <fstream>
#include <MemoryBlockPoolManager.h>
#include <SFUtil.h>
#include <StringConversion.h>
#include <IOCP.h>

#pragma region public
void Server::init(const char* argv0)
{
	google::InitGoogleLogging(argv0);
	LOG(INFO) << "Server Initialize...";
	this->setLogFolder();
	this->setConfig();
	_engine = new Engine(new IOCP(_config.ip, _config.port, _config.maxClient), _config.maxClient);
	_packetHandler = new PacketHandler;
	_serializer = new Serializer;
}
void Server::run()
{
	_engine->run();
	S_EngineEvent evt;
	while (true)
	{
		evt = _engine->getEvent();
		switch (evt.type)
		{
		case E_EngineEventType::EVENT_NET_CONNECT: printf("client connect : (%d)\n", evt.serial); UserManager::getInstance().getUser(evt.serial)->connect(); break;
		case E_EngineEventType::EVENT_NET_DISCONNECT: printf("client disconnect : (%d)\n", evt.serial); UserManager::getInstance().getUser(evt.serial)->disconnect(); break;
		case E_EngineEventType::EVENT_NET_RECV: _packetHandler->handle(evt.serial, evt.data); break;
		}
		if (evt.data != nullptr)
			MemoryBlockPoolManager::getInstance().release(evt.blockSize, evt.data);
	}
}
void Server::send(int to, S_PacketAttr attr, const google::protobuf::Message& messsage) const
{
	std::pair<Size, char*> val = _serializer->serialize(attr, messsage);

	_engine->send(to, val.first, val.second);
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
		json data = json::parse(f);
		data.get_to(_config);
	}
	catch (std::exception e)
	{
		LOG(ERROR) << "invalid config.json";
	}
}
#pragma endregion