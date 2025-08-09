#include "headers/UBO.h"

UBO::UBO() {
	glGenBuffers(1, &ID);
}

void UBO::build(GLuint program, std::vector<Sphere> spheres, std::vector<Material> materials, std::vector<Model> models) {
	// reserve buffer size
	glBufferData(GL_UNIFORM_BUFFER, sizeof(Sphere) * MAX_SPHERES + sizeof(Material) * MAX_MATERIALS + sizeof(Model) * MAX_MODELS, NULL, GL_STATIC_DRAW);

	// set data
	if (!spheres.empty()) glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Sphere) * spheres.size(), &spheres[0]);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Sphere) * MAX_SPHERES, sizeof(Material) * materials.size(), &materials[0]);
	if (!models.empty()) glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Sphere) * MAX_SPHERES + sizeof(Material) * MAX_MATERIALS, sizeof(Model) * models.size(), &models[0]);

	// bind
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

