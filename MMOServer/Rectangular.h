#pragma once
#include "Shape.h"
#include "MMO_struct.pb.h"
#include "DAT.h"
#include "S_RectDefine.h"

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
	Rectangular(Area* area, const S_RectDefine& rect) : Shape(area), _rect(rect) { _ids.push_back(DAT::NULL_NODE); }
	~Rectangular() { this->removeFromDAT(); }
	virtual void removeFromDAT() override;
	virtual void insertToDAT(const TransformInt& pivot, Area* userData) override;
	virtual void move(const TransformInt& pivot) override;
	virtual Rectangular* clone(Area* attachedArea) override { return new Rectangular(attachedArea, _rect); }
private:

	S_RectDefine _rect = {};
};