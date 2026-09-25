// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <color.h>
#include <camera.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#define XYZ(vec) vec.x, vec.y, vec.z
#define XYZA(vec) vec.x, vec.y, vec.z, vec.a
#define ARRAY_SIZE(arr) sizeof(arr)/sizeof(arr[0])
const float PI = glm::pi<float>();

int texId2texUnit[10];

float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

struct Material {
  glm::vec3 ambient_color;
  glm::vec3 diffuse_color;
  glm::vec3 specular_color;
  float shininess;
  int diffuse_map = -1;
  int specular_map = -1;
};

Color paper(241, 240, 235, 255);
Color ink(46, 33, 27, 255);
Color stone("#7a7a7a");
Color wood(86, 49, 35, 255);
Color dirt("#704c14");
//Color sky(143, 160, 191, 255);
Color sky(0, 0, 0, 255);
Color pine("#22311D");
Color white("#ffffff");
Color yellow("#ecc955");
Color sunset("#F06553");

Material paper_mat = {
  glm::vec3(RGB(paper)) * 0.2f,
  glm::vec3(RGB(paper)) * 0.8f,
  glm::vec3(RGB(paper)) * 0.2f,
  2,
};

Material ink_mat = {
  glm::vec3(RGB(ink)) * 0.1f,
  glm::vec3(RGB(ink)) * 0.8f,
  glm::vec3(RGB(ink)) * 0.8f,
  256,
};

Material stone_mat = {
  glm::vec3(RGB(stone)) * 0.2f,
  glm::vec3(RGB(stone)) * 0.8f,
  glm::vec3(RGB(stone)) * 0.2f,
  2,
};

Material wood_mat = {
  glm::vec3(RGB(wood)) * 0.2f,
  glm::vec3(RGB(wood)) * 0.8f,
  glm::vec3(RGB(wood)) * 0.2f,
  2,
};

Material pine_mat = {
  glm::vec3(RGB(pine)) * 0.2f,
  glm::vec3(RGB(pine)) * 0.8f,
  glm::vec3(RGB(pine)) * 0.0f,
  2,
};

Material white_light = {
  glm::vec3(RGB(white)),
  glm::vec3(RGB(white)),
  glm::vec3(RGB(white)),
  16,
};

Material box_mat = {
  glm::vec3(RGB(wood)) * 0.2f,
  glm::vec3(RGB(wood)) * 0.8f,
  glm::vec3(RGB(wood)) * 0.0f,
  64,
};
struct Object {
  glm::vec3 position;
  glm::vec3 scale;
  Material &material;
  const char *name;
};

#define DIRECTIONAL 1
#define POINT       2
#define SPOT        3
struct Light {
    int type;
    glm::vec4 position;
    glm::vec4 direction;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float a;
    float b;
    float c;
    float cutoff_inner;
    float cutoff_outer;
    const char *name;
    bool enabled = true;
};

int n_objects = 9;
Object objects[] = {
  // sky box
  //{glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(50.0f, 50.0f, 50.0f), sky},

  // floor
  {glm::vec3(0.0f, -0.5f, 0.0f), glm::vec3(15.0f, 1.0f, 15.0f), stone_mat, "floor"},

  // left tree
  {glm::vec3(-2.0f, 1.0f, -2.0f), glm::vec3(1.0f, 2.0f, 1.0f), wood_mat, "trunk"},
  {glm::vec3(-2.0f, 2.0f, -2.0f), glm::vec3(3.0f, 1.0f, 3.0f), pine_mat, "leaf1"},
  {glm::vec3(-2.0f, 3.0f, -2.0f), glm::vec3(2.0f, 1.0f, 2.0f), pine_mat, "leaf2"},
  {glm::vec3(-2.0f, 4.0f, -2.0f), glm::vec3(1.0f, 1.0f, 1.0f), pine_mat, "leaf3"},

  // textured cube
  {glm::vec3(2.0f, 0.5f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f), box_mat, "cube"},

  // lamp shade
  {glm::vec4(-3.5f, 6.0f, 2.0f, 1.0f), glm::vec3(1.0f, 0.25f, 1.0f), ink_mat, "lamp_shade"},
  // lamp arm
  {glm::vec4(-4.0f, 6.0f, 2.0f, 1.0f), glm::vec3(2.0f, 0.5f, 0.5f), ink_mat, "lamp_arm"},
  // lamp post
  {glm::vec4(-5.0f, 3.0f, 2.0f, 1.0f), glm::vec3(0.5f, 6.0f, 0.5f), ink_mat, "lamp_post"},
};

#define MAX_LIGHTS 16
int n_lights = 3;
Light lights[MAX_LIGHTS] = {
  {
    DIRECTIONAL,
    glm::vec4(-8.0f, 0.8f, -2.8f, 1.0f),
    glm::vec4(1.0f, -0.1f, 0.35f, 1.0f),
    glm::vec3(RGB(sunset)),
    glm::vec3(RGB(sunset)),
    glm::vec3(RGB(sunset)),
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    "sun",
    false,
  },
  {
    POINT,
    glm::vec4(2.0f, 1.5f, -2.0f, 1.0f),
    glm::vec4(0.0f),
    glm::vec3(RGB(white)),
    glm::vec3(RGB(white)),
    glm::vec3(RGB(white)),
    0.017f,
    0.07f,
    1.0f,
    0.0f,
    0.0f,
    "white_cube",
    true,
  },
  {
    SPOT,
    glm::vec4(-3.5f, 5.75f, 2.0f, 1.0f),
    glm::vec4(0.0f, -1.0f, 0.0f, 1.0f),
    glm::vec3(RGB(yellow)),
    glm::vec3(RGB(yellow)),
    glm::vec3(RGB(yellow)),
    0.017f,
    0.07f,
    1.0f,
    40.0f,
    50.0f,
    "streetlight",
    true,
  },
};
// clang-format on

float currentFrame = glfwGetTime();
float deltaTime = currentFrame;
float lastFrame = currentFrame;

Camera camera(glm::vec3(0.0f, 3.0f, 8.0f));
float lastX;
float lastY;

#define DEBUG1f(msg, f) fprintf(stderr, "Debug: %s%f\n", msg, f)
#define DEBUG1i(msg, i) fprintf(stderr, "Debug: %s%d\n", msg, i)

bool isMouseCaptured = false;
void takeMouse(GLFWwindow *window) {
  isMouseCaptured = true;
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void releaseMouse(GLFWwindow *window) {
  isMouseCaptured = false;
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

int prevEscapeValue = GLFW_RELEASE;
void processInput(GLFWwindow *window) {
  int curEscapeValue = glfwGetKey(window, GLFW_KEY_ESCAPE);
  if (prevEscapeValue != curEscapeValue) {
    if (curEscapeValue == GLFW_PRESS)
      isMouseCaptured ? releaseMouse(window) : takeMouse(window);
    prevEscapeValue = curEscapeValue;
  }
  if (!isMouseCaptured)
    return;
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.ProcessKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.ProcessKeyboard(RIGHT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    camera.ProcessKeyboard(UP, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    camera.ProcessKeyboard(DOWN, deltaTime);
}

bool firstmouse = true;
void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
  if (firstmouse) {
    lastX = xpos;
    lastY = ypos;
    firstmouse = false;
  }
  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;
  lastX = xpos;
  lastY = ypos;

  const float sensitivity = 0.1f;
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  if (isMouseCaptured)
    camera.ProcessMouseMovement(xoffset, yoffset);
}

#define CHUNK_SIZE 2048
char *readFile(const char *path) {
  if (!path)
    return NULL;

  FILE *file = fopen(path, "r");
  if (!file) {
    perror("Failed to open file");
    return NULL;
  }

  char *data = (char *)calloc(CHUNK_SIZE, sizeof(char));
  size_t cap = CHUNK_SIZE;
  size_t sz = 0;
  char ch;
  while ((ch = fgetc(file)) != EOF) {
    if (sz == cap - 2) {
      cap += CHUNK_SIZE;
      data = (char *)realloc(data, cap * sizeof(char));
    }
    data[sz++] = ch;
  }
  if (ferror(file)) {
    perror("Error occurred while reading file");
    free(data);
    data = NULL;
  }
  if (feof(file)) {
    data[sz++] = '\0';
  }

  fclose(file);
  return data;
}

template <typename T>
unsigned int makeBuffer(T *data, unsigned int n, unsigned int buffer_type) {
  unsigned int BUF;
  glGenBuffers(1, &BUF);
  glBindBuffer(buffer_type, BUF);
  glBufferData(buffer_type, n * sizeof(T), data, GL_STATIC_DRAW);
  return BUF;
}

unsigned int makeTexture(unsigned char *data, int width, int height,
                         int num_channels) {
  unsigned int TEX;
  glGenTextures(1, &TEX);
  glBindTexture(GL_TEXTURE_2D, TEX);

  int pixel_format = GL_RGB;
  if (num_channels == 4) {
    pixel_format = GL_RGBA;
  }
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, pixel_format,
               GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  return TEX;
}

unsigned int makeShader(const char *const source, unsigned int shader_type) {
  if (!source)
    return 0;
  unsigned int shader = glCreateShader(shader_type);
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);
  int success;
  char infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    fprintf(stderr, "Shader compilation failed for %s:\n%s\n", source, infoLog);
    exit(1);
  }
  return shader;
}

unsigned int makeShaderProgram(unsigned int vs_shader, unsigned int fs_shader) {
  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();

  glAttachShader(shaderProgram, vs_shader);
  glAttachShader(shaderProgram, fs_shader);
  glLinkProgram(shaderProgram);

  int success;
  char infoLog[512];
  glGetShaderiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
    fprintf(stderr, "Shader linking failed:\n%s\n", infoLog);
    exit(1);
  }
  glUseProgram(shaderProgram);
  return shaderProgram;
}

#define SHADER_PATH_MAX 128
unsigned int loadShader(const char *name) {
  char vs_path[SHADER_PATH_MAX];
  char fs_path[SHADER_PATH_MAX];
  memset(vs_path, '\0', SHADER_PATH_MAX);
  strcat(vs_path, "shaders/");
  strcat(vs_path, name);
  strcpy(fs_path, vs_path);
  strcat(vs_path, ".vs");
  strcat(fs_path, ".fs");

  char *vs_source = readFile(vs_path);
  if (!vs_source)
    exit(1);
  unsigned int vs_shader = makeShader(vs_source, GL_VERTEX_SHADER);

  char *fs_source = readFile(fs_path);
  if (!fs_source)
    exit(1);
  unsigned int fs_shader = makeShader(fs_source, GL_FRAGMENT_SHADER);

  unsigned int shaderProgram = makeShaderProgram(vs_shader, fs_shader);
  glDeleteShader(vs_shader);
  free(vs_source);
  glDeleteShader(fs_shader);
  free(fs_source);

  return shaderProgram;
}

void setUniformMaterial(unsigned int shader, Material material) {
  glUniform3f(glGetUniformLocation(shader, "material.ambient_color"),
              XYZ(material.ambient_color));
  glUniform3f(glGetUniformLocation(shader, "material.diffuse_color"),
              XYZ(material.diffuse_color));
  glUniform3f(glGetUniformLocation(shader, "material.specular_color"),
              XYZ(material.specular_color));
  glUniform1f(glGetUniformLocation(shader, "material.shininess"),
              material.shininess);
  glUniform1i(glGetUniformLocation(shader, "useTextures"), 0);

  if (material.diffuse_map >= 0 && material.specular_map >= 0) {
    glUniform1i(glGetUniformLocation(shader, "material.diffuse_map"),
                texId2texUnit[material.diffuse_map]);
    glUniform1i(glGetUniformLocation(shader, "material.specular_map"),
                texId2texUnit[material.specular_map]);
    glUniform1i(glGetUniformLocation(shader, "useTextures"), 1);
  }
}

unsigned int loadTexture(const char *path) {
  int width, height, num_channels;
  unsigned char *data = stbi_load(path, &width, &height, &num_channels, 0);
  if (!data) {
    fprintf(stderr, "Error: failed to load texture\n");
    exit(1);
  }
  unsigned int texture = makeTexture(data, width, height, num_channels);
  glGetIntegerv(GL_ACTIVE_TEXTURE, &texId2texUnit[texture]);
  texId2texUnit[texture] -= GL_TEXTURE0;
  stbi_image_free(data);
  return texture;
}

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  GLFWmonitor *monitor = glfwGetPrimaryMonitor();

  const GLFWvidmode *mode = glfwGetVideoMode(monitor);
  glfwWindowHint(GLFW_RED_BITS, mode->redBits);
  glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
  glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
  glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
  GLFWwindow *window =
      glfwCreateWindow(mode->width, mode->height, "", monitor, NULL);
  if (window == NULL) {
    fprintf(stderr, "Failed to create GLFW window\n");
    glfwTerminate();
    return 1;
  }
  glfwMakeContextCurrent(window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    fprintf(stderr, "Failed to initialize GLAD\n");
    return 1;
  }
  takeMouse(window);

  glfwSetCursorPosCallback(window, mouse_callback);

  glViewport(0, 0, mode->width, mode->height);
  glEnable(GL_DEPTH_TEST);

  // setup IMGUI
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
  ImGui_ImplOpenGL3_Init();

  // Setup scaling
  ImGuiStyle& style = ImGui::GetStyle();
  style.FontScaleDpi = 2.0;


  unsigned int VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  VBO = makeBuffer(vertices, sizeof(vertices) / sizeof(vertices[0]),
                   GL_ARRAY_BUFFER);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  unsigned int phong = loadShader("phong");
  unsigned int lightcube = loadShader("lightcube");

  glBindVertexArray(VAO);
  stbi_set_flip_vertically_on_load(true);
  glActiveTexture(GL_TEXTURE0);
  unsigned int container_diffuse = loadTexture("resources/container2.png");
  box_mat.diffuse_map = container_diffuse;

  glActiveTexture(GL_TEXTURE1);
  unsigned int container_specular =
      loadTexture("resources/container2_specular.png");
  box_mat.specular_map = container_specular;

  glm::mat4 projection;
  projection = glm::perspective(
      glm::radians(45.0f), (float)mode->width / mode->height, 0.1f, 100.0f);

  glUseProgram(phong);
  glUniformMatrix4fv(glGetUniformLocation(phong, "projection"), 1, GL_FALSE,
                     glm::value_ptr(projection));

  glUseProgram(lightcube);
  glUniformMatrix4fv(glGetUniformLocation(lightcube, "projection"), 1, GL_FALSE,
                     glm::value_ptr(projection));

  while (!glfwWindowShouldClose(window)) {
    currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    glfwPollEvents();
    processInput(window);


    glClearColor(RGBA(sky));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = camera.GetViewMatrix();

    glm::mat4 view_no_translation = view;
    view_no_translation[3] = glm::vec4(0.0f);
    glUseProgram(phong);
    char uniform_name[32];
    for (int i = 0; i < n_lights; i++) {
      sprintf(uniform_name, "lights[%d].type", i);
      glUniform1i(glGetUniformLocation(phong, uniform_name), lights[i].type);
      glUniformMatrix4fv(glGetUniformLocation(phong, "view"), 1, GL_FALSE,
                         glm::value_ptr(view));
      sprintf(uniform_name, "lights[%d].position", i);
      glUniform4f(glGetUniformLocation(phong, uniform_name),
                  XYZA(glm::vec4(view * lights[i].position)));
      glm::vec4 lpv = view_no_translation * lights[i].direction;
      sprintf(uniform_name, "lights[%d].direction", i);
      glUniform4f(glGetUniformLocation(phong, uniform_name),
                  XYZA(lpv));
      sprintf(uniform_name, "lights[%d].ambient", i);
      glUniform3f(glGetUniformLocation(phong, uniform_name),
                  XYZ(lights[i].ambient));
      sprintf(uniform_name, "lights[%d].diffuse", i);
      glUniform3f(glGetUniformLocation(phong, uniform_name),
                  XYZ(lights[i].diffuse));
      sprintf(uniform_name, "lights[%d].specular", i);
      glUniform3f(glGetUniformLocation(phong, uniform_name),
                  XYZ(lights[i].specular));
      sprintf(uniform_name, "lights[%d].a", i);
      glUniform1f(glGetUniformLocation(phong, uniform_name), lights[i].a);
      sprintf(uniform_name, "lights[%d].b", i);
      glUniform1f(glGetUniformLocation(phong, uniform_name), lights[i].b);
      sprintf(uniform_name, "lights[%d].c", i);
      glUniform1f(glGetUniformLocation(phong, uniform_name), lights[i].c);
      sprintf(uniform_name, "lights[%d].cutoff_inner", i);
      glUniform1f(glGetUniformLocation(phong, uniform_name), glm::cos(glm::radians(lights[i].cutoff_inner)));
      sprintf(uniform_name, "lights[%d].cutoff_outer", i);
      glUniform1f(glGetUniformLocation(phong, uniform_name), glm::cos(glm::radians(lights[i].cutoff_outer)));
      sprintf(uniform_name, "lights[%d].enabled", i);
      glUniform1i(glGetUniformLocation(phong, uniform_name), lights[i].enabled);
    }
    for (int i = 0; i < n_objects; i++) {
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, objects[i].position);
      model = glm::scale(model, objects[i].scale);
      glUniformMatrix4fv(glGetUniformLocation(phong, "model"), 1, GL_FALSE,
                         glm::value_ptr(model));
      setUniformMaterial(phong, objects[i].material);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    for (int i = 0; i < n_lights; i++) {
      if (lights[i].enabled) {
        glUseProgram(lightcube);
        glUniformMatrix4fv(glGetUniformLocation(lightcube, "view"), 1, GL_FALSE,
                           glm::value_ptr(view));
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(XYZ(lights[i].position)));
        model = glm::scale(model, glm::vec3(0.1f));
        glUniformMatrix4fv(glGetUniformLocation(lightcube, "model"), 1, GL_FALSE,
                           glm::value_ptr(model));
        glUniform3f(glGetUniformLocation(lightcube, "lightColor"),
                    XYZ(lights[i].diffuse));
        glDrawArrays(GL_TRIANGLES, 0, 36);
      }
    }

    // Rendering
    // (Your code clears your framebuffer, renders your other stuff etc.)
    // Start the Dear ImGui frame
    if (!isMouseCaptured) {
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();
      const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
      ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y), ImGuiCond_Once);
      ImGui::SetNextWindowSize(ImVec2(550, mode->height), ImGuiCond_Once);
      ImGui::Begin("Scene");
      static ImGuiSliderFlags flags = ImGuiSliderFlags_None;
      if (ImGui::CollapsingHeader("Objects"))
      {
        for (int i = 0; i < n_objects; i++) {
          if (ImGui::TreeNodeEx(objects[i].name)) {
            ImGui::Text("position");
            ImGui::DragFloat("x_pos", &objects[i].position.x, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", flags);
            ImGui::DragFloat("y_pos", &objects[i].position.y, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", flags);
            ImGui::DragFloat("z_pos", &objects[i].position.z, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", flags);

            ImGui::Text("scale");
            ImGui::DragFloat("x_scale", &objects[i].scale.x, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", flags);
            ImGui::DragFloat("y_scale", &objects[i].scale.y, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", flags);
            ImGui::DragFloat("z_scale", &objects[i].scale.z, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", flags);

            ImGui::Text("material");
            ImGui::ColorEdit3("ambient", (float*)&objects[i].material.ambient_color.r);
            ImGui::ColorEdit3("diffuse", (float*)&objects[i].material.diffuse_color.r);
            ImGui::ColorEdit3("specular", (float*)&objects[i].material.specular_color.r);
            ImGui::DragFloat("shininess", (float*)&objects[i].material.shininess, 1.0f, 0, 256, "%f", flags);

            ImGui::TreePop();
            ImGui::Spacing();
          }
        }
      }
      if (ImGui::CollapsingHeader("Lights"))
      {
        for (int i = 0; i < 3; i++) {
          if (ImGui::TreeNodeEx(lights[i].name)) {
            ImGui::Text("position");
            ImGui::DragFloat("x_pos", &lights[i].position.x, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", flags);
            ImGui::DragFloat("y_pos", &lights[i].position.y, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", flags);
            ImGui::DragFloat("z_pos", &lights[i].position.z, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", flags);

            ImGui::Text("direction");
            ImGui::DragFloat("x_direction", &lights[i].direction.x, 0.005f, -1.0f, 1.0f, "%.3f", flags);
            ImGui::DragFloat("y_direction", &lights[i].direction.y, 0.005f, -1.0f, 1.0f, "%.3f", flags);
            ImGui::DragFloat("z_direction", &lights[i].direction.z, 0.005f, -1.0f, 1.0f, "%.3f", flags);

            ImGui::Text("color");
            ImGui::ColorEdit3("ambient", (float*)&lights[i].ambient.r);
            ImGui::ColorEdit3("diffuse", (float*)&lights[i].diffuse.r);
            ImGui::ColorEdit3("specular", (float*)&lights[i].specular.r);
            ImGui::Checkbox("enabled", &lights[i].enabled);

            ImGui::Text("attenuation: 1/(ax^2 + bx + c)");
            ImGui::DragFloat("a", &lights[i].a, 0.0001f, 0.5f, 1.0f, "%.3f", flags);
            ImGui::DragFloat("b", &lights[i].b, 0.0001f, 0.0014f, 0.7f, "%.3f", flags);
            ImGui::DragFloat("c", &lights[i].c, 0.0001f, 0.000007, 1.8f, "%.3f", flags);

            ImGui::Text("cutoff");
            ImGui::DragFloat("inner", &lights[i].cutoff_inner, 0.5f, 0, 90.0f, "%.1f", flags);
            ImGui::DragFloat("outer", &lights[i].cutoff_outer, 0.5f, 0, 90.0f, "%.1f", flags);

            ImGui::TreePop();
            ImGui::Spacing();
          }
        }
      }
      ImGui::End();
      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    glfwSwapBuffers(window);
  }

  glfwTerminate();
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  return 0;
}
