#include "headers/FBO.h"

FBO::FBO(int width, int height) {
	glGenFramebuffers(1, &ID);
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
	resize(width, height);
}

void FBO::resize(int width, int height) {
	glGenTextures(1, &textID);
	glBindTexture(GL_TEXTURE_2D, textID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textID, 0);

	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
}

void FBO::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
}

void FBO::unBind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::bindTexture() {
	glBindTexture(GL_TEXTURE_2D, textID);
}

void FBO::unBindTexture() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

void FBO::deleteFBO() {
	glDeleteTextures(1, &textID);
	glDeleteFramebuffers(1, &ID);
}