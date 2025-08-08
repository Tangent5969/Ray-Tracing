#include "headers/SSBO.h"

SSBO::SSBO() {
	glGenBuffers(1, &ID);
}

void SSBO::build(std::vector<Triangle> triangles) {
	if (triangles.empty()) return;
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Triangle) * triangles.size(), &triangles[0], GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ID);
}

void SSBO::bind() {
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ID);
}

void SSBO::unBind() {
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void SSBO::deleteSSBO() {
	glDeleteBuffers(1, &ID);
}

