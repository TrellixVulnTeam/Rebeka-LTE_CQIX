#include "RebGame.h"



void LoadGame(IGame **IG)
{
	*IG = new RebGame;
}

void ReleaseGame(IGame **IG)
{
	delete *IG;
}



void RebGame::Init()
{

	mGDC = new RebGDC;
	
	

	rfs = new RebFileSystem;
	rfs->GetAllFiles("..\\..");
	rfs->Categorize();

	mGDC->rfs = rfs;
	mGDC->grp = &gr;


	CreateWindowManager(&winm);
	winm->Init();
	winm->CreateWin("Launcher", 1280, 720, 100, 100);
	winm->EnableRender("Launcher");
	window = winm->GetWindow("Launcher");


	CreateMEH(&meh);
	mGDC->window = window;
	mGDC->winm = winm;
	mGDC->meh = meh;

	mGDC->meh->Init(mGDC);

	CreateRenderDevice(&rd);
	rd->Init(mGDC);
	rd->SetVP(1280, 720);

	CreateEntitySystem(&ies, mGDC);

	CreateAudioDevice(&iad);
	/*ras.GetAudioDevice()->Init();
	ras.GetAudioDevice()->GetMusicPlayer()->Init();
	ras.GetAudioDevice()->GetMusicPlayer()->SetSource(rfs->Search("daft.mp3", "Music").rpath);
	ras.GetAudioDevice()->GetMusicPlayer()->Play();*/
	
	iad->Init();
	//ras.GetAudioDevice()->GetSoundSystem()->Test();

	

	

	
	/*std::vector<TComponent*> tcomps;
	TComponent * viewcompt = new TCompVisViewport();
	TComponent * inpconp = new TCompInpBasicControl();
	tcomps.push_back(viewcompt);
	tcomps.push_back(inpconp);
	res->GetTemplateManager()->CreateEntTemp("tviewport", tcomps);
	Entity * ent = res->GetTemplateManager()->CreateEntByTemp("testviwe", "tviewport");
	res->GetTemplateManager()->CreateEntByTemp("Ter1", "Terrain");
	ent->SetPos(RebVector(0.0f, 0.0f, 0.0f));
	static_cast<CompVisViewport*>(ent->GetComponent("CompVisViewport"))->SetActiveViewport();*/


	//rd->GetEnv()->CreateTerrain();


	rd->GetVertexCacheManager()->CreateCacheFromFile("testbox1", rfs->Search("phybox.obj").rpath);
	rd->GetVertexCacheManager()->CreateCacheFromFile("testbox2", rfs->Search("phybox.obj").rpath);
	rd->GetVertexCacheManager()->CreateCacheFromFile("testbox3", rfs->Search("phybox.obj").rpath);
	rd->GetVertexCacheManager()->CreateCacheFromFile("testbox4", rfs->Search("phybox.obj").rpath);
	rd->GetVertexCacheManager()->CreateCacheFromFile("kepkeret", rfs->Search("kepkeret.obj").rpath);
	//
	rd->GetVertexCacheManager()->GetVertexCache("testbox2")->GetTrans()->Scale(0.01f, 0.01f, 0.01f);
	rd->GetVertexCacheManager()->GetVertexCache("testbox2")->GetTrans()->Translate(0, 1, 0);
	rd->GetVertexCacheManager()->GetVertexCache("testbox3")->GetTrans()->Scale(0.05f, 0.05f, 0.05f);
	rd->GetVertexCacheManager()->GetVertexCache("testbox3")->GetTrans()->Translate(0, 1, 4);
	rd->GetVertexCacheManager()->GetVertexCache("testbox4")->GetTrans()->Scale(0.01f, 0.01f, 0.01f);
	rd->GetVertexCacheManager()->GetVertexCache("testbox4")->GetTrans()->Translate(2, 24, 0);
	rd->GetVertexCacheManager()->GetVertexCache("kepkeret")->GetTrans()->Scale(0.1f, 0.1f, 0.1f);
	rd->GetVertexCacheManager()->GetVertexCache("kepkeret")->GetTrans()->Translate(3, 0, -6);
	bool pressed = false;
	winm->TrapMouse(true);

	mGDC->grp = new bool(true);
}

void RebGame::GameLoop()
{
while(*mGDC->grp)
	{
		RebEvent Event;
		meh->TranslateEvent(&Event);
		
		ies->Update();
		/*ras.GetAudioDevice()->Update();*/
		rd->Render();
		rd->Swap(window);
   }
}

void RebGame::Release()
{
	winm->TrapMouse(false);
	ReleaseEntitySystem(&ies);
	/*ras.GetAudioDevice()->GetMusicPlayer()->Stop();*/
	rd->Release();
	winm->DisableRender("Launcher");
	winm->DestroyWindow("Launcher");
	winm->Release();
	ReleaseWindowManager(&winm);
	ReleaseRenderDevice(&rd);
	delete rfs;
	iad->Release();
	ReleaseAudioDevice(&iad);
	delete mGDC;
}