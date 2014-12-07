#include "stdafx.h"
#include "XMLConfigReader.h"

XMLConfigReader::XMLConfigReader(String^ path)
{
	reader = gcnew XmlTextReader(path);
}

Point* XMLConfigReader::XMLRoutine_Point(XmlReader^ reader)
{
	Console::WriteLine("----point---");

	Point* p;

	reader->ReadToFollowing("x");
	reader->Read();
	p->x = System::Convert::ToDouble(reader->Value);
	reader->ReadToFollowing("y");
	reader->Read();
	p->x = System::Convert::ToDouble(reader->Value);

	Console::WriteLine("element {0}_{1}", p->x, p->y);

	Console::WriteLine("----end point---");

	return p;
}

void XMLConfigReader::XMLRoutine_ObservationPoints(XmlReader^ reader)
{
	Console::WriteLine("----observationpoints---");

	reader->Read();
	reader->MoveToAttribute("count");
	pointsQ = System::Convert::ToInt32(reader->Value);
	points = new Double*[pointsQ];
	for (int i = 0; i < pointsQ; i++)
	{
		points[i] = new Double(3);
	}
	for (int i = 0; i < pointsQ; i++)
	{
		reader->ReadToFollowing("Point");
		Point* p = XMLRoutine_Point(reader->ReadSubtree());
		points[i][0] = p->x;
		points[i][1] = p->y;
		points[i][2] = 0.0;
		delete p;
	}

	Console::WriteLine("----end observationpoints---");
}

void XMLConfigReader::XMLRoutine_Step(XmlReader^ reader, int blockn)
{
	Console::WriteLine("----step---");

	reader->ReadToFollowing("StartTime");
	reader->Read();

	reader->ReadToFollowing("FinishTime");
	reader->Read();
	hearth[blockn][0] = System::Convert::ToInt32(reader->Value);

	reader->ReadToFollowing("Height");
	reader->Read();
	hearth[blockn][1] = System::Convert::ToInt32(reader->Value);

	Console::WriteLine("element {0}_{1}", hearth[blockn][0], hearth[blockn][1]);

	Console::WriteLine("----end step---");
}

void XMLConfigReader::XMLRoutine_Brick(XmlReader^ reader, int blockn)
{
	Console::WriteLine("----brick---");

	reader->ReadToFollowing("Coordinates");
	for (int i = 0; i < 4; i++)
	{
		reader->ReadToFollowing("Point");
		Point* p=XMLRoutine_Point(reader->ReadSubtree());
		hearth[blockn][2*(i+1)] = p->x;
		hearth[blockn][2*(i+1)+1] = p->y;
		delete p;
	}
	reader->ReadToFollowing("Scenario");
	reader->MoveToAttribute("stepq");
	int stepq = System::Convert::ToInt32(reader->Value);
	for (int i = 0; i < stepq; i++)
	{
		reader->ReadToFollowing("Step");
		XMLRoutine_Step(reader->ReadSubtree(), blockn);
	}

	Console::WriteLine("----end brick---");
}

void XMLConfigReader::XMLRoutine_Bricks(XmlReader^ reader)
{
	Console::WriteLine("----bricks---");

	reader->Read();
	reader->MoveToAttribute("count");
	blocksQ = System::Convert::ToInt32(reader->Value);
	hearth = new Double*[blocksQ];
	for (int i = 0; i < blocksQ; i++)
	{
		hearth[i] = new Double[10];
	}
	for (int i = 0; i < blocksQ; i++)
	{
		reader->ReadToFollowing("Brick");
		XMLRoutine_Brick(reader->ReadSubtree(), i);
	}

	Console::WriteLine("----end bricks---");
}

bool XMLConfigReader::parse(XmlReader^ reader)
{
	Console::WriteLine("start reading");
	while (reader->Read())
	{
		switch (reader->NodeType)
		{
		case XmlNodeType::Element:
			if (reader->Name->ToLower() == "observationpoints")
			{
				XMLRoutine_ObservationPoints(reader->ReadSubtree());
			}
			if (reader->Name->ToLower() == "bricks")
			{
				XMLRoutine_Bricks(reader->ReadSubtree());
			}
		}
	}
	Console::WriteLine("----end reading---");

	return true;
}
