#include "pch.h"
#include "Engine.h"
#include "SFUtil.h"
#include "StringConversion.h"
#include "Define.h"
#include "EngineEventContainer.h"
#include "NetworkLayer.h"
#include "Decoder.h"
#include "Encoder.h"
#include <fstream>

#pragma region public
Engine::Engine(I_NetworkCore* core, int maxClient)
{
	_evtContainer = new EngineEventContainer;
	_decoder = new Decoder(_evtContainer, maxClient);
	_network = new NetworkLayer(core, _evtContainer, _decoder);
	_encoder = new Encoder(_network);
}
Engine::~Engine()
{
	delete _network;
	delete _evtContainer;
	delete _decoder;
	delete _encoder;
}
void Engine::run() { _network->run(); }
void Engine::send(int to, Size blockSize, char* data) { _encoder->enqueue(to, blockSize, data); }
void Engine::disconnect(int serial) { _network->disconnect(serial); }
S_EngineEvent Engine::getEvent() const { return _evtContainer->pop(); }
#pragma endregion

#pragma region private
#pragma endregion