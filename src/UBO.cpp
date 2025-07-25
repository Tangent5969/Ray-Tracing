#include "headers/UBO.h"

UBO::UBO() {
	glGenBuffers(1, &ID);
}

void UBO::build(GLuint program, Sphere spheres[], int spheresLength) {	
	glBufferData(GL_UNIFORM_BUFFER, sizeof(Sphere) * spheresLength, spheres, GL_DYNAMIC_DRAW);
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

