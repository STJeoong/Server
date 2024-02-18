#include "pch.h"
#include "Engine.h"
#include "SFUtil.h"
#include "StringConversion.h"
#include "Define.h"
#include "EngineEventContainer.h"
#include "NetworkLayer.h"
#include "Decoder.h"
#include "Encoder.h"
#include "Serializer.h"
#include <fstream>

#pragma region public
Engine::Engine(const char* argv0)
{
	google::InitGoogleLogging(argv0);
	LOG(INFO) << "Engine Initialize...";
	this->setLogFolder();
	this->setConfig();
	_evtContainer = new EngineEventContainer;
	_decoder = new Decoder(_evtContainer);
	_network = new NetworkLayer(_config, _evtContainer, _decoder);
	_encoder = new Encoder(_network);
}
Engine::~Engine()
{
	delete _network;
	delete _evtContainer;
	delete _decoder;
	delete _encoder;
}
void Engine::start()
{
	_network->start();
}
void Engine::send(int to, S_PacketAttr attr, const google::protobuf::Message& message)
{
	std::pair<Size, char*> val = _serializer->serialize(attr, message);

	_encoder->enqueue(to, val.first, val.second);
}
S_EngineEvent Engine::getEvent() const { return _evtContainer->pop(); }
#pragma endregion

#pragma region private
void Engine::setLogFolder()
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
void Engine::setConfig()
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