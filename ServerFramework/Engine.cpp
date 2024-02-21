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
Engine::Engine(int id, I_NetworkCore* core, int maxClient, EngineEventContainer* container)
{
	_decoder = new Decoder(id, container, maxClient);
	_network = new NetworkLayer(id, core, container, _decoder);
	_encoder = new Encoder(_network);
}
Engine::~Engine()
{
	delete _network;
	delete _decoder;
	delete _encoder;
}
void Engine::run() { _network->run(); }
void Engine::send(int to, Size blockSize, char* data) { _encoder->enqueue(to, blockSize, data); }
void Engine::disconnect(int serial) { _network->disconnect(serial); }
#pragma endregion

#pragma region private
#pragma endregion