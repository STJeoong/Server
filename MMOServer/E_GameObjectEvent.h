#pragma once

enum class E_GameObjectEvent
{
	AWAKE,
	UPDATE,
	ACTIVE,
	INACTIVE,
	DESTROY,

	// area
	AREA_ENTER,
	AREA_STAY,
	AREA_EXIT,
};