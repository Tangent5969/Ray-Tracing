#ifndef UNIFORMS_H
#define UNIFORMS_H

#include <glad/glad.h>
#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>

class Uniforms {
public:
	void init(GLuint program);
	void update(int rayCount, int maxBounces, glm::vec3 camPos, glm::mat4 camModel, float screenW, float screenH, float camFocus, int spheresLength, int accumulationFrame, float environmentLight);

private:
	GLuint program;
	GLuint rayCountLocation;
	GLuint maxBouncesLocation;
	GLuint camPosLocation;
	GLuint modelLocation;
	GLuint resLocation;
	GLuint focusLocation;
	GLuint spheresLengthLocation;
	GLuint accumulationLocation;
	GLuint environmentLightLocation;
};

#endif 
