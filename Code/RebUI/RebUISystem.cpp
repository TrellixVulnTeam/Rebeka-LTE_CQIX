#include "RebUISystem.h"


extern "C"
{
	void _declspec(dllexport) CreateUISystem(RebGDC * gdc)
	{
		gdc->uis = new RebUISystem(gdc);
	}

	void _declspec(dllexport) ReleaseUISystem(RebGDC * gdc)
	{
		delete gdc->uis;
		gdc->uis = 0;
	}
}


RebUISystem::RebUISystem(RebGDC * gdc) : gdc(gdc), rt(RebText(gdc, gdc->rfs->Search(".*/Consolas.ttf")[0], 64, 1280, 720, "Hello World\nThis is Rebeka."))
{
	InitFBO();
}



void RebUISystem::InitFBO()
{
	gdc->window->GetClientSize(&w, &h);

	glGenFramebuffers(1, &uiframe);
	glBindFramebuffer(GL_FRAMEBUFFER, uiframe);

	glGenTextures(1, &uiframetex);
	glBindTexture(GL_TEXTURE_2D, uiframetex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, uiframetex, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	int err = glGetError();
}

void RebUISystem::SetToDraw()
{
	glBindFramebuffer(GL_FRAMEBUFFER, uiframe);
	glViewport(0, 0, w, h);

	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);
}

void StopDraw()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}






void RebUISystem::TestDraw()
{
	gdc->rd->GetShaderSystem()->GetFromBank("UIBasic")->Use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, rt.GetHandle());

	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_QUADS);
	glVertex3f(-1.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glEnd();


	int err = glGetError();
}


unsigned int RebUISystem::RenderUI()
{
	SetToDraw();
	//rtr.CreateText(gdc->rfs->Search(".*/Consolas.ttf")[0], "Hello RebText", 24, 0, 0, 100, 100);
	TestDraw();
	StopDraw();
	return uiframetex;
}