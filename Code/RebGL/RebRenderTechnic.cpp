//#include "RebRenderTechnic.h"
//
//
//
//
//void ShadowMap::SetPos(RebVector set)
//{
//	lpos = set;
//}
//
//ShadowMap2D::ShadowMap2D(RebGDC * rgdc)
//{
//	ird = rgdc->rd;
//	shadowProgram.AddShaderFile(rgdc->rfs->Search("vShadow2D.rvs", "Shaders"));
//	shadowProgram.Link();
//
//	glGenFramebuffersEXT(1, &sfbo);
//
//
//	// Bind the FBO so that the next operations will be bound to it
//	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, sfbo);
//
//
//
//	// Generate and bind the OGL texture for diffuse
//	glGenTextures(1, &st);
//	glBindTexture(GL_TEXTURE_2D, st);
//	// fixes seam-artifacts due to numerical precision limitations
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//
//
//	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
//	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
//	//glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
//
//	GLint internal_format = GL_DEPTH_COMPONENT32F;
//	GLenum data_type = GL_FLOAT;
//	// float z-buffer (if more precision is needed)
//	// GLint internal_format = GL_DEPTH_COMPONENT32F;
//	// GLenum data_type = GL_FLOAT;
//	GLenum format = GL_DEPTH_COMPONENT;
//
//
//	glTexImage2D(GL_TEXTURE_2D,
//		0,
//		internal_format,
//		w, h, 0,
//		format,
//		data_type,
//		NULL); //content need not be specified
//	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
//		st, 0);
//
//
//	glDrawBuffer(GL_NONE);
//
//	// Check if all worked fine and unbind the FBO
//	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
//	if (status != GL_FRAMEBUFFER_COMPLETE_EXT)
//		throw new std::exception("Can't initialize an FBO render texture. FBO initialization failed.");
//
//	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
//
//}
//
//void ShadowMap2D::Write()
//{
//	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, sfbo);
//	glDrawBuffer(GL_NONE);
//	glViewport(0, 0, w, h);
//}
//
//void ShadowMap2D::Read()
//{
//
//	glActiveTextureARB(GL_TEXTURE1);
//	glBindTexture(GL_TEXTURE_2D, st);
//}
//
//void ShadowMap2D::ShadowPass()
//{
//	Write();
//	shadowProgram.Use();
//	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_CULL_FACE);
//	glCullFace(GL_BACK);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	//test
//
//	RebMatrix rot, sca;
//	rot.Identity();
//	rot.RotyByDeg(0, 90, 0);
//
//	sca.Identity();
//	sca.Translate(0, -20, 0);
//
//	RebVector rv(0, 0, 0);
//
//	//end of test
//
//	glUniform3f(glGetUniformLocation(shadowProgram.GetHandle(), "ep"), 0, 20, 0);
//
//	RebMatrix shadowmat, sha, res, bias;
//	shadowmat.Identity();
//	shadowmat.Translate(0, -20, 0);
//
//
//
//
//	bias.Identity();
//	bias.Perspective(90, 1, 1, 1000);
//
//	res.Identity();
//	res.RotyByDeg(90, 0, 0);
//	/*	shadowmat.InverseOf(shadowmat);*/
//	float mm[16];
//
//
//	GLuint mmloc;
//	(shadowmat * res).glm(mm);
//	mmloc = glGetUniformLocation(shadowProgram.GetHandle(), "sm");
//	glUniformMatrix4fv(mmloc, 1, 0, mm);
//	bias.glm(mm);
//	mmloc = glGetUniformLocation(shadowProgram.GetHandle(), "cm");
//	glUniformMatrix4fv(mmloc, 1, 0, mm);
//
//	
//	for (UINT i3 = 0; i3 < ird->GetVertexCacheManager()->GetRVCs()->size(); i3++)
//	{
//
//
//		for (UINT i = 0; i < ird->GetVertexCacheManager()->GetRVCs()->at(i3)->GetRVBs()->size(); i++)
//		{
//			if (ird->GetVertexCacheManager()->GetRVCs()->at(i3)->GetRVBs()->at(i)->isRenderable())
//			{
//
//
//
//				(*ird->GetVertexCacheManager()->GetRVCs()->at(i3)->GetRVBs()->at(i)->GetTrans() * *ird->GetVertexCacheManager()->GetRVCs()->at(i3)->GetTrans()).glm(mm);
//				mmloc = glGetUniformLocation(shadowProgram.GetHandle(), "mmat");
//				glUniformMatrix4fv(mmloc, 1, 0, mm);
//				ird->GetVertexCacheManager()->GetRVCs()->at(i3)->GetRVBs()->at(i)->Draw();
//				
//			}
//		}
//	}
//	glDisable(GL_CULL_FACE);
//	glDisable(GL_DEPTH_TEST);
//}
//
//ShadowMap2D::~ShadowMap2D()
//{
//	glDeleteTextures(1, &st);
//	glDeleteFramebuffers(1, &sfbo);
//}
//
//
//ShadowMapCube::ShadowMapCube(RebGDC * rgdc)
//{
//	ird = rgdc->rd;
//	shadowProgram.AddShaderFile(rgdc->rfs->Search("vshadow.rvs", "Shaders"));
//	shadowProgram.AddShaderFile(rgdc->rfs->Search("fshadow.rfs", "Shaders"));
//	shadowProgram.AddShaderFile(rgdc->rfs->Search("gshadow.rgs", "Shaders"));
//	shadowProgram.Link();
//
//
//	w = 512;
//	h = 512;
//
//
//	glGenFramebuffersEXT(1, &sfbo);
//
//
//	// Bind the FBO so that the next operations will be bound to it
//	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, sfbo);
//
//	// Bind the depth buffer
//	//glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, srbo);
//	//glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, w, h);
//	//glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, srbo);
//
//	// Generate and bind the OGL texture for diffuse
//	glGenTextures(1, &st);
//	glBindTexture(GL_TEXTURE_CUBE_MAP, st);
//	// fixes seam-artifacts due to numerical precision limitations
//
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
//
//	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
//	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
//	//glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
//
//	GLint internal_format = GL_DEPTH_COMPONENT32F;
//	GLenum data_type = GL_FLOAT;
//	// float z-buffer (if more precision is needed)
//	// GLint internal_format = GL_DEPTH_COMPONENT32F;
//	// GLenum data_type = GL_FLOAT;
//	GLenum format = GL_DEPTH_COMPONENT;
//	for (GLint face = 0; face < 6; face++) {
//		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
//			0,
//			internal_format,
//			w, h, 0,
//			format,
//			data_type,
//			NULL //content need not be specified
//			);
//	}
//
//	//glTexImage2D(GL_TEXTURE_2D, 
//	//0, 
//	//internal_format, 
//	//w, h, 0, 
//	//format, 
//	//data_type, 
//	//NULL); //content need not be specified
//	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
//		st, 0);
//
//
//	glDrawBuffer(GL_NONE);
//
//	// Check if all worked fine and unbind the FBO
//	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
//	if (status != GL_FRAMEBUFFER_COMPLETE_EXT)
//		throw new std::exception("Can't initialize an FBO render texture. FBO initialization failed.");
//
//	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
//
//}
//
//void ShadowMapCube::Write()
//{
//	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, sfbo);
//	glDrawBuffer(GL_NONE);
//	glViewport(0, 0, w, h);
//}
//
//void ShadowMapCube::Read()
//{
//	glActiveTextureARB(GL_TEXTURE2);
//	glBindTexture(GL_TEXTURE_CUBE_MAP, st);
//	GLenum en = glGetError();
//}
//
//void my_PerspectiveFOV(double fov, double aspect, double nears, double fars, float* mret) {
//	double D2R = PI / 180.0;
//	double yScale = 1.0 / tan(D2R * fov / 2);
//	double xScale = yScale / aspect;
//	double nearmfar = nears - fars;
//	double m[] = {
//		xScale, 0, 0, 0,
//		0, yScale, 0, 0,
//		0, 0, (fars + nears) / nearmfar, -1,
//		0, 0, 2 * fars*nears / nearmfar, 0
//	};
//	for (unsigned int i = 0; i < 16; i++)
//	{
//		mret[i] = (float)m[i];
//	}
//
//}
//
//void ShadowMapCube::ShadowPass()
//{
//	
//	shadowProgram.Use();
//	Write();
//	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_CULL_FACE);
//	glCullFace(GL_FRONT);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	
//
//	shadowProgram.Use();
//
//	glUniform3f(glGetUniformLocation(shadowProgram.GetHandle(), "ep"), lpos.x, lpos.y, lpos.z);
//
//	RebMatrix shadowmat, bias;
//	shadowmat.Identity();
//	shadowmat.Translate(-lpos.x, -lpos.y, -lpos.z);
//
//
//
//
//	bias.Identity();
//	bias.Perspective(90, 1, 1, 1000);
//
//	
//	/*	shadowmat.InverseOf(shadowmat);*/
//	float mm[16];
//	float bia[16];
//	SetCUBE(shadowProgram.GetHandle());
//	
//
//	GLuint mmloc;
//
//	shadowmat.glm(mm);
//	mmloc = glGetUniformLocation(shadowProgram.GetHandle(), "sm");
//	glUniformMatrix4fv(mmloc, 1, 0, mm);
//	bias.glm(mm);
//	
//	my_PerspectiveFOV(90, 1, 1, 1000, bia);
//	mmloc = glGetUniformLocation(shadowProgram.GetHandle(), "cm");
//	GLenum en = glGetError();
//	glUniformMatrix4fv(mmloc, 1, 0, bia);
//	en = glGetError();
//	
//
//	for (UINT i3 = 0; i3 < ird->GetVertexCacheManager()->GetRVCs()->size(); i3++)
//	{
//
//
//		for (UINT i = 0; i < ird->GetVertexCacheManager()->GetRVCs()->at(i3)->GetRVBs()->size(); i++)
//		{
//			if (ird->GetVertexCacheManager()->GetRVCs()->at(i3)->GetRVBs()->at(i)->isRenderable())
//			{
//				(*ird->GetVertexCacheManager()->GetRVCs()->at(i3)->GetRVBs()->at(i)->GetTrans() * *ird->GetVertexCacheManager()->GetRVCs()->at(i3)->GetTrans()).glm(mm);
//				mmloc = glGetUniformLocation(shadowProgram.GetHandle(), "mmat");
//				glUniformMatrix4fv(mmloc, 1, 0, mm);
//				ird->GetVertexCacheManager()->GetRVCs()->at(i3)->GetRVBs()->at(i)->Draw();
//			}
//		}
//	}
//
//	
//
//	glDisable(GL_CULL_FACE);
//	glDisable(GL_DEPTH_TEST);
//}
//
//ShadowMapCube::~ShadowMapCube()
//{
//	glDeleteTextures(1, &st);
//	glDeleteFramebuffers(1, &sfbo);
//}
//
//void ShadowMapCube::SetCUBE(GLuint handle)
//{
//	RebMatrix pos;
//	pos.Identity();
//	pos.Translate(-lpos.x, -lpos.y, -lpos.z);
//
//	RebMatrix cms[6];
//	for (short i = 0; i < 6; i++)
//	{
//		cms[i].Identity();
//	}
//	cms[0]._13 = -1;
//	cms[0]._22 = -1;
//	cms[0]._31 = -1;
//	cms[0]._11 = 0;
//	cms[0]._33 = 0;
//	cms[0] = pos * cms[0];
//
//	cms[1]._13 = 1;
//	cms[1]._22 = -1;
//	cms[1]._31 = 1;
//	cms[1]._11 = 0;
//	cms[1]._33 = 0;
//	cms[1] = pos * cms[1];
//
//	cms[3]._11 = 1;
//	cms[3]._23 = 1;
//	cms[3]._32 = -1;
//	cms[3]._22 = 0;
//	cms[3]._33 = 0;
//	cms[3] = pos * cms[3];
//
//	cms[2]._11 = 1;
//	cms[2]._23 = -1;
//	cms[2]._32 = 1;
//	cms[2]._22 = 0;
//	cms[2]._33 = 0;
//	cms[2] = pos * cms[2];
//
//	cms[4]._11 = 1;
//	cms[4]._22 = -1;
//	cms[4]._33 = -1;
//	cms[4] = pos * cms[4];
//
//	cms[5]._11 = -1;
//	cms[5]._22 = -1;
//	cms[5]._33 = 1;
//	cms[5] = pos * cms[5];
//
//	for (short i = 0; i < 6; i++)
//	{
//		std::string cmname = "cm_mat[" + std::to_string(i) + "]";
//		float mm[16];
//		cms[i].glm(mm);
//		GLuint locat = glGetUniformLocation(handle, cmname.c_str());
//		glUniformMatrix4fv(locat, 1, 0, mm);
//		GLenum en = glGetError();
//	}
//}
//
//
//
