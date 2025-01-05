#include <windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <time.h> // Untuk fungsi random

double r = .2, s = .3;
int i;
float tx = 10;
float bx = -270;
float angle = 0.0f;

//------------------------------------------ Tumbleweed --------------------------------------------------
void tumbleweed(float centerX, float centerY, float radius, int segments, float lineWidth) {
    glLineWidth(lineWidth); // Set ketebalan garis
    glBegin(GL_LINES);

    for (int i = 0; i < segments; i++) {
        float angle = (i * 2.0f * 3.14159f) / segments; // Sudut dasar setiap jari-jari
        float xStart = centerX;
        float yStart = centerY;

        float xCurrent = xStart;
        float yCurrent = yStart;

        int subSegments = 6; // Membagi garis menjadi lebih banyak segmen patahan
        for (int j = 0; j < subSegments; j++) {
            float segmentLength = radius / subSegments;

            float offsetAngle = ((float)rand() / RAND_MAX - 0.5f) * (3.14159f / 4); // Deviasi sudut
            angle += offsetAngle;

            float xNext = xCurrent + cos(angle) * segmentLength;
            float yNext = yCurrent + sin(angle) * segmentLength;

            glVertex2f(xCurrent, yCurrent);
            glVertex2f(xNext, yNext);

            xCurrent = xNext;
            yCurrent = yNext;
        }
    }

    glEnd();
    glLineWidth(1.0f); // Kembalikan ketebalan garis ke default
}

//------------------------------------------ Alas --------------------------------------------------
void kotak() {
    glBegin(GL_POLYGON);
    glVertex2i(-250, -250);
    glVertex2i(250, -250);
    glVertex2i(250, 85);
    glVertex2i(-250, 85);
    glEnd();
}

//------------------------------------------ Matahari --------------------------------------------------
void matahari(double x, double y) {
    glBegin(GL_TRIANGLE_FAN);
    for (i = 0; i < 360; i++) {
        x = x + cos((i * 3.14) / 180) * s;
        y = y + sin((i * 3.14) / 180) * s;
        glVertex2d(x, y);
    }
    glEnd();
}

//------------------------------------------ Awan --------------------------------------------------
void bulat(double x, double y) {
    glBegin(GL_TRIANGLE_FAN);
    for (i = 0; i < 360; i++) {
        x = x + cos((i * 3.14) / 180) * r;
        y = y + sin((i * 3.14) / 180) * r;
        glVertex2d(x, y);
    }
    glEnd();
}

void awan(float tx, float ty) {
    glColor3ub(255, 255, 255);
    bulat(tx + 0, ty + 250);
    bulat(tx + 15, ty + 245);
    bulat(tx + 10, ty + 240);
    bulat(tx + -2, ty + 243);
}

//------------------------------------------ Gunung --------------------------------------------------
void gunung(float tx, float ty, float t, float l, float scale) {
    glBegin(GL_TRIANGLES);
    t *= scale;
    l *= scale;
    float xA = tx - l / 2, yA = ty - t;
    float xB = tx, yB = ty;
    float xC = tx + l / 2, yC = ty - t;

    glVertex2f(xA, yA);
    glVertex2f(xB, yB);
    glVertex2f(xC, yC);

    glEnd();
}

//------------------------------------------ Danau --------------------------------------------------
void danau(float centerX, float centerY, float radiusX, float radiusY) {
    glBegin(GL_POLYGON);
    for (float angle = 0.0f; angle <= 2 * 3.14159f; angle += 0.1f) {
        float x = centerX + radiusX * cos(angle);
        float y = centerY + radiusY * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
}

//------------------------------------------ Display --------------------------------------------------
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(135.0 / 255.0, 206.0 / 255.0, 235.0 / 255.0, 1.0);

    glColor3ub(255, 215, 0);
    matahari(0, 200);

    glPushMatrix();
    glTranslatef(tx, 0, 0);
    awan(-230.0f, -30.0f);
    awan(-150.0f, -70.0f);
    awan(-50.0f, -50.0f);
    awan(50.0f, -30.0f);
    awan(100.0f, -50.0f);
    awan(210.0f, -50.0f);
    glPopMatrix();
    tx += 0.1;
    if (tx > 250)
        tx = -250;

    glColor3ub(170, 153, 111);
    gunung(190.0f, 200.0f, 230.0f, 400.0f, 0.5f);

    glColor3ub(194, 178, 128);
    gunung(-50.0f, 200.0f, 230.0f, 400.0f, 0.5f);
    gunung(60.0f, 200.0f, 230.0f, 400.0f, 0.5f);

    glColor3ub(170, 153, 111);
    gunung(-200.0f, 200.0f, 230.0f, 400.0f, 0.5f);

    glColor3ub(210, 192, 145);
    kotak();

    glColor3ub(118, 249, 249);
    danau(0.0f, -70.0f, 150.0f, 100.0f);

    // Tumbleweed atas
    glColor3ub(0, 0, 0); //warna tumbleweed
    glPushMatrix();
    glTranslatef(bx, 55, 0);
    glRotatef(angle, 0.0f, 0.0f, 1.0f);
    tumbleweed(0.0f, 0.0f, 20.0f, 40, 1.5f); // Tumbleweed atas (???, ???, ???, ketebalan)
    glPopMatrix();

    // Tumbleweed bawah (refleksi)
    glColor3ub(105, 92, 59); //warna tumbleweed
    glPushMatrix();
    glScalef(1.0f, -1.0f, 1.0f); // Refleksi pada sumbu Y
    glTranslatef(bx, -10, 0);
    glRotatef(angle, 0.0f, 0.0f, 1.0f);
    tumbleweed(0.0f, 0.0f, 20.0f, 40, 1.5f); // Tumbleweed bawah (???, ???, ???, ketebalan)
    glPopMatrix();

    angle -= 0.01; // Rotasi lebih lambat
    bx += 0.1;

    if (bx > 600)
        bx = -270;

    if (angle <= -360.0f)
        angle += 360.0f;

    glutPostRedisplay();
    glFlush();
}

void init() {
    glClearColor(135.0 / 255.0, 206.0 / 255.0, 235.0 / 255.0, 1.0);
    glOrtho(-250.0, 250.0, -250.0, 250.0, -1.0, 1.0);
}

int main(int argc, char** argv) {
    srand(time(0)); // Inisialisasi random seed
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(1354, 760);
    glutCreateWindow("Tumbleweed");

    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
