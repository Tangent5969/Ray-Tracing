#include "headers/VAO.h"

VAO::VAO() {
	glGenVertexArrays(1, &ID);
}

void VAO::linkVBO(VBO VBO, GLuint layout) {
	VBO.bind();
	glVertexAttribPointer(layout, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(layout * 2 * sizeof(float)));
	glEnableVertexAttribArray(layout);
	VBO.unBind();
}

void VAO::bind() {
	glBindVertexArray(ID);
}

void VAO::unBind() {
	glBindVertexArray(0);
}

void VAO::deleteVAO() {
	glDeleteVertexArrays(1, &ID);
}