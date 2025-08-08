#ifndef SSBO_H
#define SSBO_H

#include <glad/glad.h>
#include <vector>
#include "Object.h"
#include <iostream>


class SSBO {

public:
	GLuint ID;

	SSBO();
	void build(std::vector<Triangle> triangles);
	void bind();
	void unBind();
	void deleteSSBO();

};


#endif