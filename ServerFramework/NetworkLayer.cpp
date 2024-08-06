#include "pch.h"
#include "NetworkLayer.h"
#include "I_NetworkCore.h"
#include "MemoryBlockPool.h"
#include "I_EngineEventContainer.h"
#include "Decoder.h"
#include <fstream>
#include <iostream>

#pragma region public
NetworkLayer::NetworkLayer(int engineID, I_NetworkCore* core, I_EngineEventContainer* evtContainer, Decoder* decoder)
	: _engineID(engineID), _core(core), _evtContainer(evtContainer), _decoder(decoder)
{
	_core->setOnConnect([this](int serial) { this->onConnect(serial); });
	_core->setOnDisconnect([this](int serial) { this->onDisconnect(serial); });
	_core->setOnRecv([this](int serial, int len, char* data) { this->onRecv(serial, len, data); });
}
NetworkLayer::~NetworkLayer() { delete _core; }
void NetworkLayer::start() { _core->start(); }
void NetworkLayer::send(int to, Size blockSize, int len, char* data) { _core->send(to, blockSize, len, data); }
void NetworkLayer::disconnect(int serial) { _core->disconnect(serial); }
#pragma endregion

#pragma region private
void NetworkLayer::onConnect(int serial)
{
	S_EngineEvent evt = {};
	evt.type = E_EngineEventType::EVENT_NET_CONNECT;
	evt.serial = serial;

	_evtContainer->enqueue(_engineID, evt);
	_decoder->enqueue(serial, nullptr, 0, Size::_256); // connect event push
}
void NetworkLayer::onConnectFail(int serial)
{
	S_EngineEvent evt = {};
	evt.type = E_EngineEventType::EVENT_NET_CONNECT_FAIL;
	evt.serial = serial;

	_evtContainer->enqueue(_engineID, evt);
}
void NetworkLayer::onDisconnect(int serial)
{
	S_EngineEvent evt = {};
	evt.type = E_EngineEventType::EVENT_NET_DISCONNECT;
	evt.serial = serial;

	_evtContainer->enqueue(_engineID, evt);
}
void NetworkLayer::onRecv(int serial, int len, char* data)
{
	char* block = MemoryBlockPool::get((int)Size::_1024);
	memcpy(block, data, len);

	_decoder->enqueue(serial, block, len, Size::_1024);
}
#pragma endregion