#pragma once

class I_Revertable
{
public:
	virtual ~I_Revertable() = default;
	virtual void revert() = 0;
};