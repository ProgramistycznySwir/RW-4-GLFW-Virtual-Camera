#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "linmath.h"

#include <stdlib.h>
#include <stdio.h>


static const struct
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

#pragma region >>> Shaders <<<
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
#pragma endregion

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}


float Clamp(float value, float min, float max)
{
    if(value > max)
        return max;
    if(value < min)
        return min;
    return value;
}
struct Player {
    vec3 Position;
    float Yaw;
    float Pitch;
    float X_Change;
    float Z_Change;
    float fov_Change;
    vec3 VecX;
    vec3 VecY;
    vec3 VecZ;
} player =
{
    {0, 0, 5},
    0.25f, 0, 0, 0, 0,
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1},
};

const float MovementSpeed = 0.06f;
const float FovChangeSpeed = 0.025f;
static void Key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    if (action == GLFW_PRESS) {
        player.X_Change -= (key == GLFW_KEY_A) * MovementSpeed;
        player.X_Change += (key == GLFW_KEY_D) * MovementSpeed;
    }
    else if (action == GLFW_RELEASE && ((key == GLFW_KEY_A) || (key == GLFW_KEY_D)))
        player.X_Change = 0;

    if (action == GLFW_PRESS) {
        player.Z_Change -= (key == GLFW_KEY_W) * MovementSpeed;
        player.Z_Change += (key == GLFW_KEY_S) * MovementSpeed;
    }
    else if (action == GLFW_RELEASE && ((key == GLFW_KEY_W) || (key == GLFW_KEY_S)))
        player.Z_Change = 0;

    if (action == GLFW_PRESS) {
        player.fov_Change -= (key == GLFW_KEY_KP_ADD) * FovChangeSpeed;
        player.fov_Change += (key == GLFW_KEY_KP_SUBTRACT) * FovChangeSpeed;
    }
    else if (action == GLFW_RELEASE)
        player.fov_Change = 0;
}

const float MouseSensitivity = 0.0025f;
static void Mouse_callback(GLFWwindow* window, double x_pos, double y_pos)
{
    static float x_prev = 0;
    static float y_prev = 0;

    if (fabs(x_pos - x_prev) > 0)
        player.Yaw -= (x_pos - x_prev) * MouseSensitivity;
    if (fabs(y_pos - y_prev) > 0)
        player.Pitch -= (y_pos - y_prev) * MouseSensitivity;
    player.Pitch = Clamp(player.Pitch, -1.0f, 1.0f);

    x_prev = x_pos;
    y_prev = y_pos;
}
const float fov_min = 0.174f; // 10 stopni
const float fov_max = 2.094f; // 120 stopni
const float Camera_NearClippingPlane = .1f;
const float Camera_FarClippingPlane = 100.f;

void LookAt(mat4x4 v, struct Player* player) {
    mat4x4 world_cam;
    mat4x4_identity(world_cam);
    mat4x4_rotate_Y(world_cam, world_cam, (float)player->Yaw);
    mat4x4_rotate_X(world_cam, world_cam, (float)player->Pitch);

    mat4x4 translate;
    mat4x4_translate(translate, player->Position[0], player->Position[1], player->Position[2]);

    mat4x4_mul(world_cam, translate, world_cam);
    mat4x4_invert(v, world_cam);

    /// Apply world cam
    int i;
    for(i = 0; i < 3; i++)
    {
        player->VecX[i] = world_cam[0][i];
        player->VecY[i] = world_cam[1][i];
        player->VecZ[i] = world_cam[2][i];
    }
}


int main(void)
{
    // Scale shape:
    // float scale = 12.25f;
    // for(int i = 0; i < 48; i++)
    // {
    //     vertices[i].x *= scale;
    //     vertices[i].y *= scale;
    //     vertices[i].z *= scale;
    // }

    GLFWwindow* window;
    GLuint vertex_buffer, vertex_shader, fragment_shader, program;
    GLint mvp_location, vpos_location, vcol_location;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(640, 480, "Example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(window, 0, 0);

    glfwSetKeyCallback(window, Key_callback);
    glfwSetCursorPosCallback(window, Mouse_callback);

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
        sizeof(vertices[0]), (void*)0);
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
        sizeof(vertices[0]), (void*)(sizeof(float) * 3));
    // glEnable(GL_DEPTH_TEST);

    glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LESS);

    float fov = 1.0f;

    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;
        mat4x4 m, v, p, mvp;

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mat4x4_identity(p);

        fov += (float)player.fov_Change;
        fov = Clamp(fov, fov_min, fov_max);

        mat4x4_perspective(m, fov, ratio, Camera_NearClippingPlane, Camera_FarClippingPlane);

        vec3 direction;
        vec3_scale(direction, player.VecX, (float)player.X_Change);
        vec3_add(player.Position, player.Position, direction);
        vec3_scale(direction, player.VecZ, (float)player.Z_Change);
        vec3_add(player.Position, player.Position, direction);

        LookAt(v, &player);

        mat4x4_mul(mvp, m, v);
        mat4x4_mul(mvp, mvp, p);

        glUseProgram(program);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)mvp);
        glDrawArrays(GL_TRIANGLES, 0, 48);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
