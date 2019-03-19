#include <iostream>
#include <math.h>
#include <GLFW/glfw3.h>

using namespace std;
int screen_width = 640;
int screen_height = 640;

void show(float xarray[], float yarray[], float aarray[], int length) {
    glfwInit();                                                     // 初始化GLFW

    // 创建窗口(宽、高、窗口名称)
    auto window = glfwCreateWindow(screen_width, screen_height, "Triangle", nullptr, nullptr);
    if (window == nullptr) {                                        // 如果窗口创建失败，输出Failed to Create OpenGL Context
        std::cout << "Failed to Create OpenGL Context" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);                                 // 将窗口的上下文设置为当前线程的主上下文


    while (!glfwWindowShouldClose(window)) {
        // Keep running
        /* Draw a triangle */
        glBegin(GL_POINTS);

        for (int i = 0; i < length; i++) {
//            glColor4f(1, 0.52, 0.0, aarray[i]);    // Orange
            glColor4f(1, 1, 1, aarray[i]);      // White
            glVertex2f(xarray[i], yarray[i]);
        }
        glEnd();

        //When the frame is rendered, swap the buffer with one another
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    if (glfwWindowShouldClose(window))
        glfwDestroyWindow(window);

    //done before exit
    glfwTerminate();
}

float normalize(int input) {
    return float(input) / 500;
}

void initdotpair(int &x0, int &y0, int &x1, int &y1) {
    if (x0 == x1) {
        if (y0 > y1) {
            int tempy = y0;
            y0 = y1;
            y1 = tempy;
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
    }
}

int main() {
    int x0, x1, y0, y1;
    cout << "输入第一个点(x0,y0)   (|x0|,|y0|<=500）" << endl;
    cin >> x0 >> y0;
    cout << "输入第二个点(x1,y1)   (|x1|,|y1|<=500）" << endl;
    cin >> x1 >> y1;

    int length;
    int xarray[1001], yarray[1001];
    float aarray[1001];
    int dx;
    int dy;
    int A, B;

    initdotpair(x0, y0, x1, y1);
    if (x0 == x1) {
        length = y1 - y0 + 1;
        for (int i = 0; i < length; i++) {
            xarray[i] = x0;
            yarray[i] = y0 + i;
            aarray[i] = 1;
        }
    } else {
        dx = x1 - x0;
        dy = y1 - y0;
        double m = float(dy) / float(dx);
        double b = y0 - m * x0;
        float p;
        A = -dy;
        B = dx;


        if (fabs(m) <= 1) {
            length = x1 - x0 + 1;
            for (int i = 0; i < length; i++)
                xarray[i] = x0 + i;
            if (m > 0) {
                // 0 < m <= 1
                yarray[0] = y0;
                for (int i = 1; i < length; i++) {
                    p = m * xarray[i - 1] + b - yarray[i - 1] + 0.5;
                    aarray[i - 1] = p;
                    if (p < 1 - m) {
                        yarray[i] = yarray[i - 1];
                    } else {
                        yarray[i] = yarray[i - 1] + 1;
                    }
                }
                yarray[length - 1] = y1;
                aarray[length - 1] = m * xarray[length - 1] + b - yarray[length - 1] + 0.5;
            } else {
                // -1 <= m <= 0
                yarray[0] = y0;
                for (int i = 1; i < length; i++) {
                    p = m * xarray[i - 1] + b - yarray[i - 1] + 0.5;
                    aarray[i - 1] = p;
                    if (p < -m) {
                        yarray[i] = yarray[i - 1] - 1;
                    } else {
                        yarray[i] = yarray[i - 1];
                    }
                }
                yarray[length - 1] = y1;
                aarray[length - 1] = m * xarray[length - 1] + b - yarray[length - 1] + 0.5;
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
                    p = (yarray[i - 1] - b) / m - xarray[i - 1] + 0.5;
                    aarray[i - 1] = p;
                    if (dold < 0) {
                        xarray[i] = xarray[i - 1];
                        dnew = dold + 2 * B;
                    } else {
                        xarray[i] = xarray[i - 1] + 1;
                        dnew = dold + 2 * A + 2 * B;
                    }
                    dold = dnew;
                }
                aarray[length-1] = (yarray[length - 1] - b) / m - xarray[length - 1] + 0.5;
            }
                //m<-1
            else {
                xarray[0] = x0;
                xarray[length - 1] = x1;
                int d0 = A - 2 * B;
                int dold = d0;
                int dnew;
                for (int i = 1; i < length - 1; i++) {
                    p = (yarray[i - 1] - b) / m - xarray[i - 1] + 0.5;
                    aarray[i - 1] = p;
                    if (dold < 0) {
                        xarray[i] = xarray[i - 1] + 1;
                        dnew = dold + 2 * A - 2 * B;
                    } else {
                        xarray[i] = xarray[i - 1];
                        dnew = dold - 2 * B;
                    }
                    dold = dnew;
                }
                aarray[length-1] = (yarray[length - 1] - b) / m - xarray[length - 1] + 0.5;
            }
        }
    }

    float fxarray[1001];
    float fyarray[1001];
    for (int i = 0; i < length; i++) {
        fxarray[i] = normalize(xarray[i]);
        fyarray[i] = normalize(yarray[i]);
    }
    show(fxarray, fyarray, aarray, length);

    return 0;
}
