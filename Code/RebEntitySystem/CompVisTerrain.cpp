#include "CompVisual.h"


CompVisTerrain::CompVisTerrain(IRenderDevice * sird)
{
	IRD = sird;
	rvc = 0;
	ter = IRD->GetEnv()->CreateTerrain();
	curloaded = "none";
	Params["loaded"] = "none";
	Params["sizex"] = 32;
	Params["sizey"] = 32;
	/*ter->Hps.push_back(RebVector(0.0f, 0.0f, 0.0f));
	ter->Hps.push_back(RebVector(32.0f, 0.0f, 0.0f));
	ter->Hps.push_back(RebVector(32.0f, 0.0f, -32.0f));
	ter->Hps.push_back(RebVector(0.0f, 0.0f, -32.0f));
	ter->Hps.push_back(RebVector(0.0f, 0.0f, 0.0f));*/
}

CompVisTerrain::~CompVisTerrain()
{
	IRD->GetEnv()->DeleteTerrain(ter);
}

void CompVisTerrain::update()
{

}