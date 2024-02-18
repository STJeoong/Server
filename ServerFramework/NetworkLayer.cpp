#include "pch.h"
#include "NetworkLayer.h"
#include "I_NetworkCore.h"
#include "MemoryBlockPoolManager.h"
#include "NetEvent.h"
#include "EngineEventContainer.h"
#include "Decoder.h"
#include "IOCP.h"
#include <fstream>
#include <iostream>
#include "ServerConfig.h"

#pragma region public
NetworkLayer::NetworkLayer(ServerConfig config, EngineEventContainer* evtContainer, Decoder* decoder) : _evtContainer(evtContainer), _decoder(decoder)
{
	if (config.network == "iocp")
		_network = new IOCP(config);
	else
		std::cout << config.network << " is not supported. please check config.json";
	_network->setOnConnect([this](int serial) { this->onConnect(serial); });
	_network->setOnDisconnect([this](int serial) { this->onDisconnect(serial); });
	_network->setOnRecv([this](int serial, int len, char* data) { this->onRecv(serial, len, data); });
}
NetworkLayer::~NetworkLayer() { delete _network; }
void NetworkLayer::start() { _network->start(); }
void NetworkLayer::send(int to, Size blockSize, int len, char* data) { _network->send(to, blockSize, len, data); }
#pragma endregion

#pragma region private
void NetworkLayer::onConnect(int serial)
{
	S_EngineEvent evt = {};
	evt.type = E_EngineEventType::EVENT_NET_CONNECT;
	evt.serial = serial;

	_evtContainer->enqueue(evt);
}
void NetworkLayer::onDisconnect(int serial)
{
	S_EngineEvent evt = {};
	evt.type = E_EngineEventType::EVENT_NET_DISCONNECT;
	evt.serial = serial;

	_evtContainer->enqueue(evt);
	_decoder->reset(serial);
}
void NetworkLayer::onRecv(int serial, int len, char* data)
{
	// TODO : len이 256을 넘을 수 있나?
	char* block = MemoryBlockPoolManager::getInstance().get(Size::_256);
	memcpy(block, data, len);

	_decoder->enqueue(serial, block, len, Size::_256);
}
#pragma endregion