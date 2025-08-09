#include "headers/Uniforms.h"

void Uniforms::init(GLuint program) {
	this->program = program;

	rayCountLocation = glGetUniformLocation(program, "rayCount");
	maxBouncesLocation = glGetUniformLocation(program, "maxBounces");
	camPosLocation = glGetUniformLocation(program, "camPos");
	modelLocation = glGetUniformLocation(program, "model");
	resLocation = glGetUniformLocation(program, "res");
	focusLocation = glGetUniformLocation(program, "focus");
	spheresLengthLocation = glGetUniformLocation(program, "spheresLength");
	modelsLengthLocation = glGetUniformLocation(program, "modelsLength");
	accumulationLocation = glGetUniformLocation(program, "accumulationFrame");
	environmentLightLocation = glGetUniformLocation(program, "environmentLightFlag");
}

void Uniforms::update(int rayCount, int maxBounces, glm::vec3 camPos, glm::mat4 camModel, float screenW, float screenH, float camFocus, int spheresLength, int modelsLength, int accumulationFrame, float environmentLight) {
	glUniform1i(rayCountLocation, rayCount);
	glUniform1i(maxBouncesLocation, maxBounces);
	glUniform3fv(camPosLocation, 1, glm::value_ptr(camPos));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(camModel));
	glUniform2f(resLocation, screenW, screenH);
	glUniform1f(focusLocation, camFocus);
	glUniform1i(spheresLengthLocation, spheresLength);
	glUniform1i(modelsLengthLocation, modelsLength);
	glUniform1i(accumulationLocation, accumulationFrame);
	glUniform1f(environmentLightLocation, environmentLight);
}
