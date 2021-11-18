#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "linmath.h"

#include <stdlib.h>
#include <stdio.h>

static struct
{
    float x, y, z;
    float r, g, b;
} vertices[48] =
{
    {1.0f,-1.0f,0.0f,1.0f,1.0f,1.0f},
    {1.0f,0.0f,0.0f,1.0f,1.0f,1.0f},
    {0.0f,1.0f,0.0f,1.0f,1.0f,1.0f},
    {-1.0f,-1.0f,0.0f,1.0f,1.0f,1.0f},
    {1.0f,-1.0f,0.0f,1.0f,1.0f,1.0f},
    {0.0f,1.0f,0.0f,1.0f,1.0f,1.0f},
    {-1.0f,0.0f,0.0f,1.0f,1.0f,1.0f},
    {-1.0f,-1.0f,0.0f,1.0f,1.0f,1.0f},
    {0.0f,1.0f,0.0f,1.0f,1.0f,1.0f},
    {0.0f,1.0f,1.0f,1.0f,0.0f,0.0f},
    {1.0f,0.0f,0.0f,1.0f,0.0f,0.0f},
    {0.0f,1.0f,0.0f,1.0f,0.0f,0.0f},
    {1.0f,0.0f,1.0f,1.0f,0.0f,0.0f},
    {0.0f,1.0f,1.0f,1.0f,0.0f,0.0f},
    {1.0f,0.0f,0.0f,1.0f,0.0f,0.0f},
    {1.0f,0.0f,1.0f,1.0f,0.5f,0.0f},
    {1.0f,-1.0f,0.0f,1.0f,0.5f,0.0f},
    {1.0f,0.0f,0.0f,1.0f,0.5f,0.0f},
    {1.0f,-1.0f,1.0f,1.0f,0.5f,0.0f},
    {1.0f,0.0f,1.0f,1.0f,0.5f,0.0f},
    {1.0f,-1.0f,0.0f,1.0f,0.5f,0.0f},
    {1.0f,-1.0f,1.0f,1.0f,1.0f,0.0f},
    {-1.0f,-1.0f,0.0f,1.0f,1.0f,0.0f},
    {1.0f,-1.0f,0.0f,1.0f,1.0f,0.0f},
    {-1.0f,-1.0f,1.0f,1.0f,1.0f,0.0f},
    {1.0f,-1.0f,1.0f,1.0f,1.0f,0.0f},
    {-1.0f,-1.0f,0.0f,1.0f,1.0f,0.0f},
    {-1.0f,-1.0f,1.0f,0.0f,1.0f,0.0f},
    {-1.0f,0.0f,0.0f,0.0f,1.0f,0.0f},
    {-1.0f,-1.0f,0.0f,0.0f,1.0f,0.0f},
    {-1.0f,0.0f,1.0f,0.0f,1.0f,0.0f},
    {-1.0f,-1.0f,1.0f,0.0f,1.0f,0.0f},
    {-1.0f,0.0f,0.0f,0.0f,1.0f,0.0f},
    {-1.0f,0.0f,1.0f,0.0f,0.5f,1.0f},
    {0.0f,1.0f,0.0f,0.0f,0.5f,1.0f},
    {-1.0f,0.0f,0.0f,0.0f,0.5f,1.0f},
    {0.0f,1.0f,1.0f,0.0f,0.5f,1.0f},
    {-1.0f,0.0f,1.0f,0.0f,0.5f,1.0f},
    {0.0f,1.0f,0.0f,0.0f,0.5f,1.0f},
    {0.0f,1.0f,1.0f,0.5f,0.0f,1.0f},
    {1.0f,0.0f,1.0f,0.5f,0.0f,1.0f},
    {1.0f,-1.0f,1.0f,0.5f,0.0f,1.0f},
    {0.0f,1.0f,1.0f,0.5f,0.0f,1.0f},
    {1.0f,-1.0f,1.0f,0.5f,0.0f,1.0f},
    {-1.0f,-1.0f,1.0f,0.5f,0.0f,1.0f},
    {0.0f,1.0f,1.0f,0.5f,0.0f,1.0f},
    {-1.0f,-1.0f,1.0f,0.5f,0.0f,1.0f},
    {-1.0f,0.0f,1.0f,0.5f,0.0f,1.0f}
};

static const char* vertex_shader_text =
"#version 110\n"
"uniform mat4 MVP;\n"
"attribute vec3 vCol;\n"
"attribute vec3 vPos;\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 1.0);\n"
"    color = vCol;\n"
"}\n";

static const char* fragment_shader_text =
"#version 110\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_FragColor = vec4(color, 1.0);\n"
"}\n";

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main(void)
{
    /// Scale shape:
    float scale = 0.25f;
    for(int i = 0; i < 48; i++)
    {
        vertices[i].x *= scale;
        vertices[i].y *= scale;
        vertices[i].z *= scale;
    }


    GLFWwindow* window;
    GLuint vertex_buffer, vertex_shader, fragment_shader, program;
    GLint mvp_location, vpos_location, vcol_location;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    // gladLoadGL(glfwGetProcAddress);
    gladLoadGL();
    glfwSwapInterval(1);

    // NOTE: OpenGL error checks have been omitted for brevity

    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    mvp_location = glGetUniformLocation(program, "MVP");
    vpos_location = glGetAttribLocation(program, "vPos");
    vcol_location = glGetAttribLocation(program, "vCol");

    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(vertices[0]), (void*) 0);
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(vertices[0]), (void*) (sizeof(float) * 3));
    // glEnable(GL_DEPTH_TEST);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float ratio;
        int width, height;
        mat4x4 m, p, mvp;

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mat4x4_identity(m);
        /// Transforms:
        mat4x4_translate(m,  (float) glfwGetTime() * 0.05f, 0.f, 0.f);
        mat4x4_rotate_X(m, m, (float) glfwGetTime());
        mat4x4_rotate_Y(m, m, (float) glfwGetTime() / 2.0f);

        mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        mat4x4_mul(mvp, p, m);

        glUseProgram(program);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
        glDrawArrays(GL_TRIANGLES, 0, 48);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
