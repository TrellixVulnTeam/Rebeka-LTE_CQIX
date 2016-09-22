#include "RebTextRenderer.h"
#include "../Reb3D/Reb3d.h"


void RebText::Render()
{

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	GLenum dbs[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, dbs);

	textprog->Use();
	//uniform set


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, glyptex);

	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float penx, peny;
	penx = peny = 0.0f;
	for (auto c : data)
	{
		FT_Load_Char(face, c, FT_LOAD_RENDER);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

		//Draw
		glBegin(GL_QUADS);
		glVertex4f(penx, peny, 0.0, 0.0);
		glVertex4f(penx + (face->glyph->bitmap.width / fw), peny, 1.0, 0.0);
		glVertex4f(penx + (face->glyph->bitmap.width / fw), peny - (face->glyph->bitmap.rows / fh), 1.0, 1.0);
		glVertex4f(penx, peny - (face->glyph->bitmap.rows / fh), 0.0, 1.0);
		glEnd();


		penx += (face->glyph->advance.x / (64.0 * fw));

	}
	glDisable(GL_BLEND);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


RebText::RebText(RebGDC * gdc, IFile * font, size_t bbx, size_t bby, std::string text) : data(text), fw(bbx), fh(bby)
{

	textprog = gdc->rd->GetShaderSystem()->GetFromBank("UIText");

	FT_Init_FreeType(&lib);
	FT_New_Face(lib, font->GetAPath().c_str(), 0, &face);
	FT_Set_Char_Size(face, 12 * 64, 12 * 64, 96, 96);

	glGenTextures(1, &glyptex);

	glGenFramebuffers(1, &fbo);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, fw, fh, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);

	Render();

}