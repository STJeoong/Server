#pragma once
#include "Shape.h"
#include "MMO_struct.pb.h"
#include "DAT.h"
#include "S_RectDefine.h"

// yExtension, xExtension은 offset으로부터 양방향으로 뻗어나가는 게 아니라 오른쪽 아래로 뻗어나간다.
// offset =>  *=========|
//			  |         |
//			  |         |
//			  ===========
// 직사각형의 가로길이 : xExtension
// 직사각형의 세로길이 : yExtension
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