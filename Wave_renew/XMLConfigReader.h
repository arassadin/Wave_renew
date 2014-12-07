#pragma once

#include<iostream>

using namespace System;
using namespace System::Xml;

struct Point
{
	Double x;
	Double y;
};

ref class XMLConfigReader
{
public:
	XMLConfigReader(String^);
	bool parse(XmlReader^ reader);

public:
	Double** points;
	Double** hearth;
	int pointsQ;
	int blocksQ;

private:

private:
	XmlReader^ reader;
	void XMLRoutine_Bricks(XmlReader^ reader);
	void XMLRoutine_Brick(XmlReader^ reader, int blockn);
	void XMLRoutine_Step(XmlReader^ reader, int blockn);
	void XMLRoutine_ObservationPoints(XmlReader^ reader);
	Point* XMLRoutine_Point(XmlReader^ reader);
};

