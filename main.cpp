// #pragma warning(suppress : 4996)
// #pragma warning(disable : 4996)
// #pragma warning( disable : 4244 ) 
// #pragma warning( disable : 26451 )


#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void myPanic(const char* message) {
    printf(message);
    exit(EXIT_FAILURE);
}

struct Camera {
    glm::vec3 x;
    glm::vec3 y;
    glm::vec3 z;
    glm::vec3 position;
    double yaw;
    double pitch;
    double moveOX;
    double moveOZ;
    double moveOY;
} cam = {
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, -1},
    {0, 0, 3},
    -90.f, 0, 0, 0, 0
};

struct Light {
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 KaColor;
    glm::vec3 KdColor;
    glm::vec3 move;
} light = {
    { 1, 1, 1 },
    { -1, -1, -1 },
    { 1, 1, 1 },
    { 1, 1, 1 },
    { 0, 0, 0 }
};

struct Material {
    glm::vec3 Ka;
    glm::vec3 Kd;
    glm::vec3 Ks;
    float Ns;
} mat = {
    {0.3f, 0.3f, 0.3f},
    {1.f, 1.f, 1.f}
};


void load_obj(
        std::vector < glm::vec3 >& vertices,
        std::vector < glm::vec2 >& UVs,
        std::vector < glm::vec3 >& normals,
        std::vector< uint32_t >& ind) {
    std::vector< unsigned int > vertexIndices, texIndices, normalIndices;
    std::vector<glm::vec3> tmp_vertices, tmp_normals;
    std::vector<glm::vec2> tmp_UVs;
    glm::vec3 v, t, n;
 
    FILE* f = fopen("../Resources/monkey.obj", "r");
    if(f == NULL)
        myPanic("There is no file: \"../Resources/monkey.obj\"!\n");
    FILE* m = fopen("../Resources/monkey.mtl", "r");
    if(m == NULL)
        myPanic("There is no file: \"../Resources/monkey.mtl\"!\n");
    
    uint32_t vert_index = 0;

    int l;
    int line_index = 0;
    const char mat_name = 'a';
    const char mtl_file_name = 'a';
    char line[128];
    while (fscanf(f, "%s", line) == 1) {
        if (strcmp(line, "v") == 0) {
            glm::vec3 vertex;
            fscanf(f, "%f %f %f", &vertex.x, &vertex.y, &vertex.z);
            tmp_vertices.push_back(vertex);
        }
        else if (strcmp(line, "vt") == 0) {
            glm::vec2 uv;
            fscanf(f, "%f %f", &uv.x, &uv.y);
            tmp_UVs.push_back(uv);
        }
        else if (strcmp(line, "vn") == 0) {
            glm::vec3 normal;
            fscanf(f, "%f %f %f", &normal.x, &normal.y, &normal.z);
            tmp_normals.push_back(normal);
        }
        else if (strcmp(line, "f") == 0) {
            std::string vertex1, vertex2, vertex3;
            int vIn, tIn, nIn;
            for (int i = 0; i < 3; i++) {
                l = -1;
                fscanf(f, "%d/%d/%d", &vIn, &tIn, &nIn);

                for (int j = 0; j < vertexIndices.size(); j++) {
                    if (vIn == vertexIndices[j] && tIn == texIndices[j] && nIn == normalIndices[j]) {
                        l = j;
                        break;
                    }
                }
                if (l == -1) {
                    ind.push_back(vert_index++);
                    vertexIndices.push_back(vIn);
                    texIndices.push_back(tIn);
                    normalIndices.push_back(nIn);
                }
                else
                    ind.push_back(l);
            }
        }
        else if (strcmp(line, "mtllib") == 0)
            fscanf(f, "%s", &mtl_file_name);
        else if (strcmp(line, "usemtl") == 0)
            fscanf(f, "%s", &mat_name);
        line_index++;
    }
    printf("load_obj() has // read %d lines from obj.\n", line_index); // DEBUG
   
    for (unsigned int i = 0; i < vertexIndices.size(); i++) {
        unsigned int vIn = vertexIndices[i];
        glm::vec3 vertex = tmp_vertices[vIn - 1];
        vertices.push_back(vertex);
    }
    for (unsigned int i = 0; i < texIndices.size(); i++) {
        unsigned int tIn = texIndices[i];
        glm::vec2 tex = tmp_UVs[tIn - 1];
        UVs.push_back(tex);
    }
    for (unsigned int i = 0; i < normalIndices.size(); i++) {
        unsigned int nIn = normalIndices[i];
        glm::vec3 normal = tmp_normals[nIn - 1];
        normals.push_back(normal);
    }

    fclose(f);

    while (fscanf(m, "%s", line) == 1) {
        if (strcmp(line, "Ns") == 0) 
            fscanf(m, "%f", &mat.Ns);
        else if (strcmp(line, "Ka") == 0) 
            fscanf(m, "%f %f %f", &mat.Ka.x, &mat.Ka.y, &mat.Ka.z);
        else if (strcmp(line, "Kd") == 0) 
            fscanf(m, "%f %f %f", &mat.Kd.x, &mat.Kd.y, &mat.Kd.z);
        else if (strcmp(line, "Ks") == 0)
            fscanf(m, "%f %f %f", &mat.Ks.x, &mat.Ks.y, &mat.Ks.z);
    }
    fclose(m);

    std::string texName;
    texName = "../Resources/metal-texture.jpg";
    char* charTexName = &texName[0];
    int texture_width, texture_height, comp;
    unsigned char* texture_data = stbi_load(charTexName, &texture_width, &texture_height, &comp, 0);
    if (texture_data == NULL)
        myPanic("Texture file not found!\n");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width, texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(texture_data);
    printf("OBJ loaded successfully!\n");
}

static void error_callback(int error, const char* description) {
    fprintf(stderr, "Blad: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    if (key == GLFW_KEY_W || key == GLFW_KEY_S) {
        if (action == GLFW_PRESS) {
            if (key == GLFW_KEY_W) {cam.moveOZ = -0.05f;}
            else {cam.moveOZ = 0.05f;}
        }
        if (action == GLFW_RELEASE) {cam.moveOZ = 0;}
    }
    if (key == GLFW_KEY_A || key == GLFW_KEY_D) { 
        if (action == GLFW_PRESS) {
            if (key == GLFW_KEY_A) { cam.moveOX = -0.05f;}
            else {cam.moveOX = 0.05f;}
        }
        if (action == GLFW_RELEASE) {cam.moveOX = 0;}
    }
    if (key == GLFW_KEY_SPACE || key == GLFW_KEY_C) {
        if (action == GLFW_PRESS) {
            if (key == GLFW_KEY_SPACE) {cam.moveOY = 0.05f;}
            else {cam.moveOY = -0.05f;}
        }
        if (action == GLFW_RELEASE) {cam.moveOY = 0;}
    }
}

static void cursor_callback(GLFWwindow* window, double xPos, double yPos) {
    static double xPrev = 0;
    static double yPrev = 0;
    double xOffset = xPos - xPrev;
    double yOffset = yPos - yPrev;
    xPrev = xPos;
    yPrev = yPos;

    cam.yaw += xOffset * 0.5;
    cam.pitch -= yOffset * 0.5;
    if (cam.pitch > 89.0f)
        cam.pitch = 89.0f;
    if (cam.pitch < -89.0f)
        cam.pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(cam.yaw)) * cos(glm::radians(cam.pitch));
    front.y = sin(glm::radians(cam.pitch));
    front.z = sin(glm::radians(cam.yaw)) * cos(glm::radians(cam.pitch));
    cam.z = glm::normalize(front);
}

char* load_shader(const char* source) {
    FILE* f;
    f = fopen64(source, "rb");
    if (!f)
        return NULL;
    fseek(f, 0, SEEK_END);
    long l = ftell(f);
    char* buf = (char*)malloc(l + 1);
    fseek(f, 0, SEEK_SET);
    fread(buf, l, 1, f);
    fclose(f);
    buf[l] = 0;
    return buf;
}
int main(void) {
    printf("Program started.\n"); // DEBUG
    GLFWwindow* window;
    GLuint VBO, VAO, EBO, vertex_shader, fragment_shader, program;
    GLuint aPos_location, aNormal_location;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(window, 0, 0);

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_callback);

    glfwMakeContextCurrent(window);
    gladLoadGL();
    glfwSwapInterval(1);

    glGenVertexArrays(1, &VAO);

    GLuint Texture;
    glGenTextures(1, &Texture);
    glBindTexture(GL_TEXTURE_2D, Texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    std::vector< glm::vec3 > vertices, normals;
    std::vector< glm::vec2 > texs;
    std::vector<uint32_t> ind;

    load_obj(vertices, texs, normals, ind);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Texture);

    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * sizeof(uint32_t), &ind[0], GL_STATIC_DRAW);

    GLuint normalbuffer;
    glGenBuffers(1, &normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(1);

    GLuint uvbuffer;
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, texs.size() * sizeof(glm::vec2), &texs[0], GL_STATIC_DRAW);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
    glEnableVertexAttribArray(2);

    int success;
    char infoLog[512];

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    const char* shader_source = load_shader("../Resources/Shaders/texture_diffuse.vert");
    glShaderSource(vertex_shader, 1, &shader_source, NULL);
    glCompileShader(vertex_shader);

    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
        printf("%s\n", infoLog);
    }

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    shader_source = load_shader("../Resources/Shaders/texture_diffuse.frag");
    glShaderSource(fragment_shader, 1, &shader_source, NULL);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
        printf("%s\n", infoLog);
    }

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    GLint MVP_location, VP_location, lightDir_location, V_location, lightKaColor_location, lightKdColor_location;

    MVP_location = glGetUniformLocation(program, "MVP");
    VP_location = glGetUniformLocation(program, "VP");
    V_location = glGetUniformLocation(program, "V");
    aPos_location = glGetAttribLocation(program, "aPos");
    aNormal_location = glGetAttribLocation(program, "aNormal");
    lightDir_location = glGetUniformLocation(program, "lightDir");
    lightKaColor_location = glGetUniformLocation(program, "aLightColor");
    lightKdColor_location = glGetUniformLocation(program, "dLightColor");
    GLint Ka_location = glGetUniformLocation(program, "Ka");
    GLint Kd_location = glGetUniformLocation(program, "Kd");

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        std::cout << "Frame ";
        float ratio;
        int width, height;
        glm::mat4x4 m, p, v, vp, mvp;

        glfwGetFramebufferSize(window, &width, &height);

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        p = glm::mat4x4(1.f);
        v = glm::mat4x4(1.f);

        m = glm::perspective(1.f, width / (float)height, 0.1f, 100.f);

        cam.position -= glm::normalize(cam.z) * (float)cam.moveOZ;
        cam.position += glm::normalize(glm::cross(cam.z, cam.y)) * (float)cam.moveOX;
        cam.position += glm::normalize(cam.y) * (float)cam.moveOY;

        cam.y = glm::normalize(glm::cross(glm::cross(cam.z, glm::vec3(0.f, 1.f, 0.f)), cam.z));

        v = glm::lookAt(cam.position, cam.position + cam.z, cam.y);

        vp = v * p;
        mvp = m * vp;

        glUseProgram(program);
        glBindVertexArray(VAO);
        glUniformMatrix4fv(MVP_location, 1, GL_FALSE, &mvp[0][0]);
        glUniformMatrix4fv(VP_location, 1, GL_FALSE, &vp[0][0]);
        glUniformMatrix4fv(V_location, 1, GL_FALSE, &v[0][0]);

        glUniform3fv(Ka_location, 1, &mat.Ka[0]);
        glUniform3fv(Kd_location, 1, &mat.Kd[0]);
        glUniform3fv(lightDir_location, 1, &light.direction[0]);
        glUniform3fv(lightKaColor_location, 1, &light.KaColor[0]);
        glUniform3fv(lightKdColor_location, 1, &light.KdColor[0]);
        glDrawElements(GL_TRIANGLES, ind.size(), GL_UNSIGNED_INT, 0);

        glEnable(GL_DEPTH_TEST);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    exit(EXIT_SUCCESS);
}
