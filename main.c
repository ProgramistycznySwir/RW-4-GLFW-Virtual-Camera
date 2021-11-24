#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "linmath.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
// #include <algorithm> // For clamp

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

struct Camera {
    float Yaw;
    float Pitch;
    float move_Z;
    float move_X;
    float move_fov;
    vec3 x_d;
    vec3 y_d;
    vec3 z_d;
    vec3 Pos;

} camera =
{
    0, 0, 0, 0, 0,
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1},
    {0, 0, 2},
};

static void Key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    
    if (key == GLFW_KEY_KP_SUBTRACT || key == GLFW_KEY_KP_ADD) { //zwiększanie i zmniejszanie kątów widzenia
        if (action == GLFW_PRESS)
            camera.move_fov = key == GLFW_KEY_KP_ADD ? -0.05f : 0.05f;
        if (action == GLFW_RELEASE)
            camera.move_fov = 0;
    }

    if (key == GLFW_KEY_A || key == GLFW_KEY_D) { //w lewo, w prawo
        if (action == GLFW_PRESS)
            camera.move_X = key == GLFW_KEY_A ? -0.05f : 0.05f;
        if (action == GLFW_RELEASE)
            camera.move_X = 0;
    }

    if (key == GLFW_KEY_S || key == GLFW_KEY_W) { //s do tyłu, w do przodu
        if (action == GLFW_PRESS)
            camera.move_Z = key == GLFW_KEY_W ? -0.05f : 0.05f;
        if (action == GLFW_RELEASE)
            camera.move_Z = 0;
    }
}

static void Mouse_callback(GLFWwindow* window, double x_pos, double y_pos)
{
    static double x_prev_pos = 0;
    static double y_prev_pos = 0;

    double x_dif = x_pos - x_prev_pos;
    double y_dif = y_pos - y_prev_pos;

    x_prev_pos = x_pos;
    y_prev_pos = y_pos;

    if (fabs(x_dif) > 0)
        camera.Yaw += x_dif * 0.005;
    if (fabs(y_dif) > 0)
        camera.Pitch -= y_dif * 0.005;
}
const float fov_min = 0.174f; // 10 stopni
const float fov_max = 2.094f; // 120 stopni
const float Camera_NearClippingPlane = .1f;
const float Camera_FarClippingPlane = 100.f;

void LookAt(mat4x4 v, struct Camera* camera) {
    mat4x4 world_cam, translate;
    mat4x4_identity(world_cam);

    mat4x4_rotate_Y(world_cam, world_cam, (float)camera->Yaw);
    mat4x4_rotate_X(world_cam, world_cam, (float)camera->Pitch);
    mat4x4_translate(translate, camera->Pos[0], camera->Pos[1], camera->Pos[2]);

    mat4x4_mul(world_cam, translate, world_cam);
    mat4x4_invert(v, world_cam);

    camera->x_d[0] = world_cam[0][0];
    camera->x_d[1] = world_cam[0][1];
    camera->x_d[2] = world_cam[0][2];
    camera->y_d[0] = world_cam[1][0];
    camera->y_d[1] = world_cam[1][1];
    camera->y_d[2] = world_cam[1][2];
    camera->z_d[0] = world_cam[2][0];
    camera->z_d[1] = world_cam[2][1];
    camera->z_d[2] = world_cam[2][2];
}



int main(void)
{
    /// Scale shape:
    // float scale = 0.25f;
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

    window = glfwCreateWindow(640, 480, "GLFW Virtual Camera", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

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

    /// Player position and rotation:
    float fov = 1.0f;
    // vec3 player_pos = { 0.f, 0.f, 2.f};
     // X is for Yaw, Y is for Pitch
    /// Register for actions related to player:
    // glfwSetKeyCallback(window, GatherWASD_KeyCallback);

    /// Setup input:
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(window, 0, 0);

    glfwSetKeyCallback(window, Key_callback);
    glfwSetCursorPosCallback(window, Mouse_callback);

    time_t lastFrameTime = time(NULL);
    while (!glfwWindowShouldClose(window))
    {
        /// Calculating deltaTime:
        time_t currentFrameTime = time(NULL);
        float deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        /// Applying player input:
        float playerSpeed = 0.5f;
        float mouseSensitivity = 0.2f;
        // vec3 player_forward = {1 * sinf(player_rot.yaw), 1 * cosf(player_rot.yaw)};
        /// Player movement...
        vec3 temp;
        // vec3_scale(temp, { 0.f, 0.f, 1.f }, isKeyPressed.w * playerSpeed * deltaTime);
        // vec3_add(player_pos, player_pos, temp);
        // vec3_add(player_pos, player_pos, { 0.f, 0.f, isKeyPressed.w * playerSpeed * deltaTime });
        // vec3_add(player_pos, player_pos, { 0.f, 0.f, - isKeyPressed.s * playerSpeed * deltaTime });
        // vec3_add(player_pos, player_pos, { - isKeyPressed.a * playerSpeed * deltaTime , 0.f, 0.f });
        // vec3_add(player_pos, player_pos, { isKeyPressed.s * playerSpeed * deltaTime , 0.f, 0.f });

        fov += (float)camera.move_fov;
        fov = Clamp(fov, fov_min, fov_max);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float ratio;
        int width, height;
        mat4x4 m, v, p;

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mat4x4_identity(p);
        mat4x4_perspective(m, fov, ratio, Camera_NearClippingPlane, Camera_FarClippingPlane);

        vec3 direction;
        vec3_scale(direction, camera.x_d, (float)camera.move_X);
        vec3_add(camera.Pos, camera.Pos, direction);
        vec3_scale(direction, camera.z_d, (float)camera.move_Z);
        vec3_add(camera.Pos, camera.Pos, direction);
        /// Transforms:
        // mat4x4_translate(m,  (float) glfwGetTime() * 0.05f, 0.f, 0.f);
        // mat4x4_rotate_X(m, m, (float) glfwGetTime());
        // mat4x4_rotate_Y(m, m, (float) glfwGetTime() / 2.0f);

        // mat4x4_perspective(m, 0.5, ratio, .1f, 100.f);
        LookAt(v, &camera);

        mat4x4 mvp;
        mat4x4_mul(mvp, m, v);
        mat4x4_mul(mvp, mvp, m);

        /// Rendering:
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
