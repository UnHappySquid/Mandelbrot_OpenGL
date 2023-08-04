#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <random>

// Shaders 
//  Vertex Shader (performs projection)
const std::string vs = R"glsl(
#version 330 core

layout(location = 0) in vec4 position;


void main()
{
   gl_Position = position;
}
)glsl";

//  fragment Shader 
const std::string fs = R"glsl(
#version 330 core
out vec4 color;

uniform vec2 iResolution;
uniform int iterations;
uniform vec2 pos;
uniform vec2 scale;

void main(){

    
    vec2 p = gl_FragCoord.xy / iResolution.xy - 0.5; 
    vec2 z = vec2(0, 0);
    vec2 c = p * scale + pos;
    int i;
    for (i = 0; i < iterations; i++) {
        z = vec2(z.x*z.x - z.y*z.y, 2*z.x*z.y) + c;
        if (length(z) > 2) break;
}
 
    color = vec4(vec3(float(i) / float(iterations)), 1);

}
)glsl";

// Constants
const double FPS = 60, frame_duration = 1 / FPS;

// Possible variables but probably constants as well
int WIDTH = 2560, HEIGHT = 1440, ITER = 50;
float aspect = float(WIDTH) / float(HEIGHT);


// Used as 3d position with w for good mesure
struct Position {
    float x, y, z, w;
};

// Operator overloads for printing and what not
std::ostream& operator<<(std::ostream& os, Position& p) {
    os << "{ " << p.x << ", " << p.y << ", " << p.z << ", " << p.w << " }";
    return os;
}
bool operator==(Position& p1, Position& p2) {
    return p1.x == p2.x && p1.y == p2.y && p1.z == p2.z;
}
bool operator!=(Position& p1, Position& p2) {
    return !(p1 == p2);
}

// Shader compiler
static unsigned int compile_shader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE) {
        int len;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
        char* message = (char*)alloca(len * sizeof(char));
        glGetShaderInfoLog(id, len, &len, message);
        std::cout << "Failed to compile shader" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

// initiate window
GLFWwindow* window_init() {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        throw std::runtime_error("Couldn't initialize glfw");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  /*  GLFWmonitor* mon = glfwGetPrimaryMonitor();
    
    const GLFWvidmode* return_struct = glfwGetVideoMode(mon);

    WIDTH = return_struct->width;
    HEIGHT = return_struct->height;
    aspect = float(WIDTH) / HEIGHT;*/
   
    /* Create a windowed mode window and its OpenGL context */
    //window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", glfwGetPrimaryMonitor(), NULL);
   
    window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        throw std::runtime_error("Couldnt create window");
    }

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);



    return window;
}

// Load open_GL
void load_OpenGL() {
    // Load openGL
    int version = gladLoadGL();
    if (version == 0) {
        throw std::runtime_error("Failed to initialize OpenGL context\n");
    }
}

// Creates program and links shaders
static unsigned int create_and_use_shaders(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int id = glCreateProgram();
    unsigned int vs = compile_shader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(id, vs);
    glAttachShader(id, fs);

    glLinkProgram(id);
    glValidateProgram(id);

    glDeleteShader(vs);
    glDeleteShader(fs);


    glUseProgram(id);

    return id;
}

// frees resources related to glfw
void terminate(GLFWwindow* window) {
    glfwDestroyWindow(window);
    glfwTerminate();
}
