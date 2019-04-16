#include <iostream>
#include <math.h>
#include <GLFW/glfw3.h>

using namespace std;


int main() {
    float x0 = -10, y0 = 0, x1 = -0.5, y1 = 99.75, x2 = 0, y2 = 100;
    float xarray1[9976], yarray1[9976];
    float xarray2[51], yarray2[51];

    for (int i = 0; i < 9975; i++)
        yarray1[i] = y0 + 0.01 * i;
    yarray1[9975] = y1;
    xarray1[0] = x0;
    xarray1[9975] = -0.5;
    float d0 = 0.01 * x0 + 0.010025;
    float dold = d0;
    float dnew;
    for (int i = 1; i < 9976; i++) {
        if (dold < 0) {
            xarray1[i] = xarray1[i - 1];
            dnew = dold + 0.01;
        } else {
            xarray1[i] = xarray1[i - 1] + 0.01;
            dnew = dold + 0.02 * xarray1[i - 1] + 0.0102;
        }
        dold = dnew;
    }
    for (int i = 0; i < 9976; i++) {
        xarray1[i] = xarray1[i] / 20;
        yarray1[i] = yarray1[i] / 200;
//        cout << xarray1[i] << "   " << yarray1[i] << endl;
    }

    for (int i = 0; i < 51; i++)
        xarray2[i] = x1 + 0.01 * i;
    yarray2[0] = y1;
    yarray2[50] = y2;
    d0 = 0.02 * x1 + 0.0051;
    dold = d0;
    for (int i = 1; i < 51; i++) {
        if (dold < 0) {
            yarray2[i] = yarray2[i - 1] + 0.01;
            dnew = dold + 0.02 * xarray2[i - 1] + 0.0103;
        } else {
            yarray2[i] = yarray2[i - 1];
            dnew = dold + 0.02 * xarray1[i - 1] + 0.0003;
        }
        dold = dnew;
    }
    for (int i = 0; i < 51; i++) {
        xarray2[i] = xarray2[i] / 20;
        yarray2[i] = yarray2[i] / 200;
//        cout << xarray2[i] << "   " << yarray2[i] << endl;
    }

    glfwInit();

//create a window with its HW1.1 context
    GLFWwindow *window1 = glfwCreateWindow(640, 640, "line", NULL, NULL);
    if (window1 == NULL) {
        cout << "Window or HW1.1 context creation failed" <<
             endl;

        glfwTerminate();

        return -1;
    }

// Make the window's context current */
    glfwMakeContextCurrent(window1);
    while (!glfwWindowShouldClose(window1)) {
        // Keep running
        glBegin(GL_POINTS);
        glColor3f(1, 0.52, 0.0);    // Orange
        for (int i = 0; i < 9976; i++) {
            glVertex2f(xarray1[i], yarray1[i]);
            glVertex2f(-xarray1[i], yarray1[i]);
        }
        for (int i = 0; i < 51; i++) {
            glVertex2f(xarray2[i], yarray2[i]);
            glVertex2f(-xarray2[i], yarray2[i]);
        }
        glEnd();

        //When the frame is rendered, swap the buffer with one another
        glfwSwapBuffers(window1);

        /* Poll for and process events */
        glfwPollEvents();
    }

    if (glfwWindowShouldClose(window1)) glfwDestroyWindow(window1);

    //done before exit
    glfwTerminate();

    return 0;
}