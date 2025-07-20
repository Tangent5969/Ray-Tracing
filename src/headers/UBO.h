#ifndef UBO_H
#define UBO_H

#include <glad/glad.h>
#include <vector>
#include "Object.h"
#include <iostream>


class UBO {

public:
	GLuint ID;

	UBO();
	void build(GLuint program, Sphere spheres[], int spheresLength);
	void bind();
	void unBind();
	void deleteUBO();

};


#endif
