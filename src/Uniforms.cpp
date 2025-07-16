#include "headers/Uniforms.h"

void Uniforms::init(GLuint program) {
	this->program = program;

	camPosLocation = glGetUniformLocation(program, "camPos");
	modelLocation = glGetUniformLocation(program, "model");
	resLocation = glGetUniformLocation(program, "res");
	focusLocation = glGetUniformLocation(program, "focus");
	spheresLengthLocation = glGetUniformLocation(program, "spheresLength");
	seedLocation = glGetUniformLocation(program, "seed");
}

void Uniforms::update(glm::vec3 camPos, glm::mat4 camModel, float screenW, float screenH, float camFocus, int spheresLength, int seed) {
	

	glUniform3fv(camPosLocation, 1, glm::value_ptr(camPos));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(camModel));
	glUniform2f(resLocation, screenW, screenH);
	glUniform1f(focusLocation, camFocus);
	glUniform1i(spheresLengthLocation, spheresLength);
	glUniform1ui(seedLocation, seed);
}
