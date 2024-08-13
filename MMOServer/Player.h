#pragma once
#include <basetsd.h>
#include "MMO_protocol.pb.h"

using namespace protocol::mmo;
class Player
{
public:
	static void init();
private:
	static void broadcast(UINT16 packetID, UINT8 option, const google::protobuf::Message& message);
	static void broadcast(UINT16 packetID, UINT8 option);
	static void writeAllPlayerInfo(google::protobuf::RepeatedPtrField<PlayerInfo>* infos);
	static void onPlayerEnter(int serial);
	static void onPlayerExit(int serial);

	void spawn(int id, int x, int y);
	void despawn();

	static int s_maxPlayer;
	static Player* s_players;

	PlayerInfo _info = {};
};