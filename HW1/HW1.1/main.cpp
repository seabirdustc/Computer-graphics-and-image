#include <iostream>
#include <math.h>
#include <GLFW/glfw3.h>

using namespace std;

void show(float xarray[], float yarray[], int length) {
    glfwInit();

    //create a window with its HW1.1 context
    GLFWwindow *window1 = glfwCreateWindow(640, 640, "line", NULL, NULL);

    if (window1 == NULL) {
        cout << "Window or HW1.1 context creation failed" << endl;
        glfwTerminate();
        return;
    }

    // Make the window's context current */
    glfwMakeContextCurrent(window1);

    while (!glfwWindowShouldClose(window1)) {
        // Keep running
        /* Draw a triangle */
        glBegin(GL_POINTS);
        glColor3f(1, 0.52, 0.0);    // Orange
        for (int i = 0; i < length; i++) {
            glVertex2f(xarray[i], yarray[i]);
        }
        glEnd();

        //When the frame is rendered, swap the buffer with one another
        glfwSwapBuffers(window1);

        /* Poll for and process events */
        glfwPollEvents();
    }

    if (glfwWindowShouldClose(window1))
        glfwDestroyWindow(window1);

    //done before exit
    glfwTerminate();
}

float normalize(int input) {
    return float(input) / 500;
}

int main() {
    int x0, x1, y0, y1;
    cout << "Please gives the x and y value of two points" << endl;
    cout << "with one space between them (like: 'x y')" << endl;
    cout << "Notice: the x and y value should be integers between -500 and 500" << endl << endl;

    cout << "the x and y value of the first point?" << endl;
    cin >> x0 >> y0;
    cout << "the coordinate of the first point: (" << x0 << ", " << y0 << ")." << endl;

    cout << "the x and y value of the second point?" << endl;
    cin >> x1 >> y1;
    cout << "the coordinate of the second point: (" << x1 << ", " << y1 << ")." << endl;

    int length;
    int xarray[1001], yarray[1001];
    int dx;
    int dy;
    int A, B;

    if (x0 == x1) {
        if (y0 > y1) {
            int tempy = y0;
            y0 = y1;
            y1 = tempy;
        }
        length = y1 - y0 + 1;
        for (int i = 0; i < length; i++) {
            xarray[i] = x0;
            yarray[i] = y0 + i;
        }
    } else {
        //let point 1 be on the right of point 0
        if (x0 > x1) {
            int tempx = x0;
            int tempy = y0;
            x0 = x1;
            y0 = y1;
            x1 = tempx;
            y1 = tempy;
        }

        dx = x1 - x0;
        dy = y1 - y0;
        A = -dy;
        B = dx;
        float m = float(dy) / float(dx);

        if (fabs(m) <= 1) {
            length = x1 - x0 + 1;
            for (int i = 0; i < length; i++)
                xarray[i] = x0 + i;
            if (m > 0) {
                yarray[0] = y0;
                yarray[length - 1] = y1;
                int d0 = 2 * A + B;
                int dold = d0;
                int dnew;
                for (int i = 1; i < length - 1; i++) {
                    if (dold < 0) {
                        yarray[i] = yarray[i - 1] + 1;
                        dnew = dold + 2 * A + 2 * B;
                    } else {
                        yarray[i] = yarray[i - 1];
                        dnew = dold + 2 * A;
                    }
                    dold = dnew;
                }
            }
                //-1<=m<0
            else {
                yarray[0] = y0;
                yarray[length - 1] = y1;
                int d0 = 2 * A - B;
                int dold = d0;
                int dnew;
                for (int i = 1; i < length - 1; i++) {
                    if (dold < 0) {
                        yarray[i] = yarray[i - 1];
                        dnew = dold + 2 * A;
                    } else {
                        yarray[i] = yarray[i - 1] - 1;
                        dnew = dold + 2 * A - 2 * B;
                    }
                    dold = dnew;
                }
            }
        } else {
            //fabs(m)>1
            length = fabs(y1 - y0) + 1;
            for (int i = 0; i < length; i++)
                if (m > 0) yarray[i] = y0 + i;
                else yarray[i] = y0 - i;
            if (m > 1) {
                xarray[0] = x0;
                xarray[length - 1] = x1;
                int d0 = A + 2 * B;
                int dold = d0;
                int dnew;
                for (int i = 1; i < length - 1; i++) {
                    if (dold < 0) {
                        xarray[i] = xarray[i - 1];
                        dnew = dold + 2 * B;
                    } else {
                        xarray[i] = xarray[i - 1] + 1;
                        dnew = dold + 2 * A + 2 * B;
                    }
                    dold = dnew;
                }
            }
                //m<-1
            else {
                xarray[0] = x0;
                xarray[length - 1] = x1;
                int d0 = A - 2 * B;
                int dold = d0;
                int dnew;
                for (int i = 1; i < length - 1; i++) {
                    if (dold < 0) {
                        xarray[i] = xarray[i - 1] + 1;
                        dnew = dold + 2 * A - 2 * B;
                    } else {
                        xarray[i] = xarray[i - 1];
                        dnew = dold - 2 * B;
                    }
                    dold = dnew;
                }
            }
        }
    }

    float fxarray[1001];
    float fyarray[1001];
    for (int i = 0; i < length; i++) {
        fxarray[i] = normalize(xarray[i]);
        fyarray[i] = normalize(yarray[i]);
    }

    show(fxarray, fyarray, length);

    return 0;
}