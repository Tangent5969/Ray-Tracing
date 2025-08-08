#include "headers/UBO.h"

UBO::UBO() {
	glGenBuffers(1, &ID);
}

void UBO::build(GLuint program, std::vector<Sphere> spheres, std::vector<Material> materials) {	
	glBufferData(GL_UNIFORM_BUFFER, sizeof(Sphere) * MAX_SPHERES + sizeof(Material) * MAX_MATERIALS, NULL, GL_STATIC_DRAW);
	if (!spheres.empty()) glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Sphere) * spheres.size(), &spheres[0]);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Sphere) * MAX_SPHERES, sizeof(Material) * materials.size(), &materials[0]);
	GLuint location = glGetUniformBlockIndex(program, "objects");
	glUniformBlockBinding(program, location, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, ID);
}

void UBO::bind() {
	glBindBuffer(GL_UNIFORM_BUFFER, ID);
}

void UBO::unBind() {
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UBO::deleteUBO() {
	glDeleteBuffers(1, &ID);
}

