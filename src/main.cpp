#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstdio>
#include <cstdlib>

#include <argparser.h>
#include <Shader.hpp>

GLint w = 600;
GLint h = 400;

bool fullscreen      = false;
char shaderpath[256] = "default.frag";

static void key_callback(GLFWwindow* window, int key, int, int action, int) {
    switch (key) {
        case GLFW_KEY_ESCAPE:
            if (action == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, GL_TRUE);
            }
            break;
        case GLFW_KEY_F:
        case GLFW_KEY_F11:
            if (action == GLFW_PRESS) {
                if (fullscreen) {
                    glfwRestoreWindow(window);
                } else {
                    glfwMaximizeWindow(window);
                }
                fullscreen = !fullscreen;
            }
            break;
    }
}

static void window_size_callback(GLFWwindow*, int width, int height) {
    glViewport(0, 0, width, height);
}

static void cleanup(int status) {
    glfwTerminate();
    exit(status);
}

int main(int argc, char* argv[]) {
    argparser ap = argparser_create(argc, argv, PARSEMODE_LENIENT);

    argparser_add(&ap, "-W", "--width",      ARGTYPE_INT,    &w,          "Window width");
    argparser_add(&ap, "-H", "--height",     ARGTYPE_INT,    &h,          "Window height");
    argparser_add(&ap, "-f", "--fullscreen", ARGTYPE_BOOL,   &fullscreen, "Start window in fullscreen");
    argparser_add(&ap, "-s", "--shader",     ARGTYPE_STRING, &shaderpath, "Path to fragment shader");

    argparser_parse(&ap);

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    auto monitor = fullscreen ? glfwGetPrimaryMonitor() : nullptr;

    auto window = glfwCreateWindow(w, h, "Shadertool", monitor, nullptr);
    if (!window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        cleanup(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);

    gladLoadGL();

    glfwGetFramebufferSize(window, &w, &h);
    glViewport(0, 0, w, h);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    glfwSetKeyCallback(window, key_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);

    GLfloat vertices[] = {
         1.0f,  1.0f,
         1.0f, -1.0f,
        -1.0f, -1.0f,
        -1.0f,  1.0f,
    };

    GLuint indices[] = {
        0, 1, 3,
        1, 2, 3,
    };

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	Shader shader;
    shader.attachFromFile("default.vert")
          .attachFromFile(shaderpath)
          .link();

	GLint posAttrib = glGetAttribLocation(shader.get(), "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GL_FLOAT), (GLvoid*)0);

    auto iGlobalTime = 0.0f;
    auto iResolution = glm::vec3(w, h, (GLfloat)w/h);
    auto iMouse      = glm::vec4(0.0f);
    auto iFrame      = -1;

    auto xy = glm::vec2(0.0f);
    auto zw = glm::vec2(0.0f);

    int lastMouse = GLFW_RELEASE;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        iFrame++;

        iGlobalTime = glfwGetTime();

        double mx, my;
        glfwGetCursorPos(window, &mx, &my);
        my = h-my;  // Flip y-axis

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            xy = glm::vec2(mx, my);
            if (lastMouse == GLFW_RELEASE) {
                zw = glm::vec2(mx, my);
            }
            lastMouse = GLFW_PRESS;
        } else {
            zw = glm::vec2(0.0f, 0.0f);
            lastMouse = GLFW_RELEASE;
        }

        iMouse = glm::vec4(xy, zw);

        shader.bind(iGlobalTime, "iGlobalTime");
        shader.bind(iResolution, "iResolution");
        shader.bind(iMouse,      "iMouse");
        shader.bind(iFrame,      "iFrame");
        shader.use();

        glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
    }

    cleanup(EXIT_SUCCESS);
}
