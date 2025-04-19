// AlgeEngine.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/fwd.hpp>

#include "shader.h"

// TODO: Reference additional headers your program requires here.
#include <fstream>
#include <sstream>
#include <string>
#include <vector>


class TriangleMesh {
public:
	TriangleMesh();
	void draw();
	~TriangleMesh();

private:
	unsigned int EBO, VAO, element_count;
	std::vector<unsigned int> VBOs;
};