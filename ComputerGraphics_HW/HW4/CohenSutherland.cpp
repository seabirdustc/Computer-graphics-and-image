#include <GL/glut.h>


class wcPt3D {
public:
    GLfloat x, y, z;
};

inline GLint round(const GLfloat a) {
    return GLint(a + 0.5);
}

const GLint winLeftBitCode = 0x1;
const GLint winRightBitCode = 0x2;
const GLint winBottomBitCode = 0x4;
const GLint winTopBitCode = 0x8;
const GLint winFrontBitCode = 0x16;
const GLint winBehindBitCode = 0x32;

inline GLint inside(GLint code) {
    return GLint(!code);
}

inline GLint reject(GLint code1, GLint code2) {
    return GLint(code1 & code2);
}

inline GLint accept(GLint code1, GLint code2) {
    return GLint(!(code1 | code2));
}

GLubyte encode(wcPt3D pt, wcPt3D winMin, wcPt3D winMax) {
    GLubyte code = 0x00;
    winMin
    if (pt.x < winMin.x)
        code = code | winLeftBitCode;
    if (pt.x > winMax.x)
        code = code | winRightBitCode;
    if (pt.y < winMin.y)
        code = code | winBottomBitCode;
    if (pt.y > winMax.y)
        code = code | winTopBitCode;
    if (pt.z < winMin.z)
        code = code | winFrontBitCode;
    if (pt.z > winMax.z)
        code = code | winBehindBitCode;
    return (code);
}

void swapPts(wcPt3D *p1, wcPt3D *p2) {
    wcPt3D tmp;
    tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
}

void swapCodes(GLubyte *c1, GLubyte *c2) {
    GLubyte tmp;
    tmp = *c1;
    *c1 = *c2;
    *c2 = tmp;
}

void lineClipCohSuth(wcPt3D winMin, wcPt3D winMax, wcPt3D p1, wcPt3D p2) {
    GLubyte code1, code2;
    GLint done = false, plotline = false;
    GLfloat myx, mzx;
    GLfloat mxy, mzy;
    GLfloat mxz, myz;

    while (!done) {
        code1 = encode(p1, winMin, winMax);
        code2 = encode(p2, winMin, winMax);
        if (accept(code1, code2)) {
            done = true;
            plotline = true;
        } else if (reject(code1, code2))
            done = true;
        else {
            if (inside(code1)) {
                swapPts(&p1, &p2);
                swapCodes(&code1, &code2);
            }
            if (p2.x != p1.x) {
                myx = (p2.y - p1.y) / (p2.x - p1.x);
                mzx = (p2.z - p1.z) / (p2.x - p1.x);
            }
            if (code1 & winLeftBitCode) {
                p1.y += (winMin.x - p1.x) * myx;
                p1.z += (winMin.x - p1.x) * mzx;
                p1.x = winMin.x;
            } else if (code1 & winRightBitCode) {
                p1.y += (winMax.x - p1.x) * myx;
                p1.z += (winMax.x - p1.x) * mzx;
                p1.x = winMax.x;
            } else if (code1 & winBottomBitCode) {
                if (p2.x != p1.x) {
                    mxy = 1 / myx;
                    p1.x += (winMin.y - p1.y) * mxy;
                }
                if (p2.z != p1.z) {
                    mzy = (p2.z - p1.z) / (p2.y - p1.y);
                    p1.z += (winMin.y - p1.y) * mzy;
                }
                p1.y = winMin.y;
            } else if (code1 & winTopBitCode) {
                if (p2.y != p1.y) {
                    mxy = (p1.x - p2.x) / (p1.y - p2.y);
                    p1.x += (winMax.y - p1.y) * mxy;
                    mzy = (p2.z - p1.z) / (p2.y - p1.y);
                    p1.z += (winMax.y - p1.y) * mzy;
                }
                p1.y = winMax.y;
            } else if (code1 & winFrontBitCode) {
                if (p2.z != p1.z) {
                    mxz = (p1.x - p2.x) / (p1.z - p2.z);
                    p1.x += (winMin.z - p1.z) * mxz;
                    myz = (p2.z - p1.z) / (p2.y - p1.y);
                    p1.y += (winMin.z - p1.z) * myz;
                }
                p1.z = winMin.z;
            } else if (code1 & winBehindBitCode) {
                if (p2.z != p1.z) {
                    mxz = (p1.x - p2.x) / (p1.z - p2.z);
                    p1.x += (winMax.z - p1.z) * mxz;
                    myz = (p2.z - p1.z) / (p2.y - p1.y);
                    p1.y += (winMax.z - p1.z) * myz;
                }
                p1.z = winMax.z;
            }

        }
    }
    if (plotline)
        lineBres(round(p1.x), round(p1.y), round(p1.z), round(p2.x), round(p2.y), round(p2.z));
}
