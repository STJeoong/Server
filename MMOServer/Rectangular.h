#pragma once
#include "Shape.h"
#include "MMO_struct.pb.h"
#include "DAT.h"

// yExtension, xExtension�� offset���κ��� ��������� ������� �� �ƴ϶� ������ �Ʒ��� �������.
// offset =>  *=========|
//			  |         |
//			  |         |
//			  ===========
// ���簢���� ���α��� : xExtension
// ���簢���� ���α��� : yExtension
// * : offset
class Rectangular : public Shape
{
public:
	Rectangular(int offsetY, int offsetX, int yExtension, int xExtension)
		: _offsetY(offsetY), _offsetX(offsetX), _yExtension(yExtension), _xExtension(xExtension)
	{ _ids.push_back(DAT::NULL_NODE); }
	~Rectangular() { this->removeFromDAT(); }
	virtual void removeFromDAT() override;
	virtual void insertToDAT(const TransformInt& pivot, void* userData) override;
	virtual void move(const TransformInt& pivot) override;
private:
	virtual Rectangular* clone() override { return new Rectangular(_offsetY, _offsetX, _yExtension, _xExtension); }

	int _offsetY;
	int _offsetX;
	int _yExtension = 0;
	int _xExtension = 0;
};