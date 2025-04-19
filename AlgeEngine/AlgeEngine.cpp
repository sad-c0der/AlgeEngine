// AlgeEngine.cpp : Defines the entry point for the application.
//
#include "AlgeEngine.h"
#include "material.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

unsigned int make_module(const std::string& filepath, unsigned int module_type);
unsigned int make_shader(const std::string& vertex_filepath, const std::string& fragment_filepath);

// Settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

int main(int argc, char* argv[])
{
	/* GLFW Initialize and Configure */
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* GLFW Window Creation */
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Learn OpenGL", NULL, NULL);

	/* Check if GLFW Window was Created */
	if (window == NULL) {
		std::cout << "Failed to create GLFW Window!\n"  << std::endl;
		/* Terminate GLFW Library */
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	/* GLAD: Load all OpenGL Function Pointers */
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD!\n" << std::endl;
		return -1;
	}

	/* Load our Shaders */

	Shader shader("shaders/test.vert", "shaders/test.frag");

	TriangleMesh* triangle = new TriangleMesh();

	// Material
	Material* material = new Material("textures/container.jpg", "textures/awesomeface.png");


	glm::vec3 quad_position = { 0.3f, -0.4f, 0.0f };

	unsigned int model_loc = glGetUniformLocation(shader.ID, "model");
	unsigned int view_loc = glGetUniformLocation(shader.ID, "view");
	unsigned int projection_loc = glGetUniformLocation(shader.ID, "projection");

	if (model_loc == -1 || view_loc == -1) {
		std::cerr << "ERROR: Uniform 'model' or 'view' not found in shader!" << std::endl;
	}

	glm::vec3 camera_pos = { -0.5f, 0.0f, 3.0f };
	glm::vec3 camera_target = { 0.0f, 0.0f, 0.0f };
	glm::vec3 camera_up = { 0.0f, 1.0f, 0.0f };
	glm::mat4 view = glm::lookAt(camera_pos, camera_target, camera_up);
	glm::mat4 projection = glm::perspective(45.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10.0f);


	shader.use();
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);


	while (!glfwWindowShouldClose(window)) {

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// input
		processInput(window);

		// render
		shader.use();
		shader.setInt("texture2", 1); // or with shader class
		material->use();
		triangle->draw();

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, quad_position);
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
		shader.setMat4("model", model);

		// swap buffers and poll IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Delete all the objects
	glDeleteProgram(shader.ID);
	delete triangle;
	glfwTerminate();
	return 0;
}

unsigned int make_shader(const std::string& vertex_filepath, const std::string& fragment_filepath) {
	char infoLog[512];
	int success;
	std::vector<unsigned int> modules;
	modules.reserve(2);

	// Compile shaders
	unsigned int vertexShader = make_module(vertex_filepath, GL_VERTEX_SHADER);
	unsigned int fragmentShader = make_module(fragment_filepath, GL_FRAGMENT_SHADER);

	if (vertexShader == 0 || fragmentShader == 0) {
		std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n";
		return 0;
	}

	modules.push_back(vertexShader);
	modules.push_back(fragmentShader);

	// Create shader program
	unsigned int shaderProgram = glCreateProgram();
	for (unsigned int shaderModule : modules) {
		glAttachShader(shaderProgram, shaderModule);
	}
	glLinkProgram(shaderProgram);

	// Check linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
		glDeleteProgram(shaderProgram);
		return 0; 
	}

	// Clean up shaders after linking
	for (unsigned int shaderModule : modules) {
		glDeleteShader(shaderModule);
	}

	return shaderProgram;
}

unsigned int make_module(const std::string& filepath, unsigned int module_type) {
	std::ifstream file(filepath);

	if (!file) {
		std::cerr << "ERROR::SHADER::FILE_NOT_FOUND: " << filepath << std::endl;
		return 0; 
	}

	std::string shaderSource((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();

	// Create and compile shader
	unsigned int shaderModule = glCreateShader(module_type);
	const char* shaderSrc = shaderSource.c_str();
	glShaderSource(shaderModule, 1, &shaderSrc, NULL);
	glCompileShader(shaderModule);

	// Check for compilation errors
	int success;
	glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &success);

	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(shaderModule, sizeof(infoLog), NULL, infoLog);
		std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
		return 0;
	}

	return shaderModule;
}

/* Whenever the GLFW Window is resized (by User or OS) this callback function is executed */
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	// Make sure the viewport matches the new window dimensions
	// Note: Width and Height will be significantly larger than specified on retina displays 
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
}

/* Process all input: query GLFW whether the relevant keys are pressed/released this frame and react accordingly */
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}