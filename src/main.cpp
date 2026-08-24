#include <GLFW/glfw3.h>
#include <glad/gl.h>

int main() {
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(1024, 720, "Maze Explorer", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    gladLoadGL(glfwGetProcAddress);

    glfwSwapInterval(1); // vsync

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(1, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
