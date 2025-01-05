#include <windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <time.h> // Untuk fungsi random

double r = .2, s = .3;
int i;
float tx = 10;
float bx = -570;
float angle = 0.0f;
float leafAngle = 0.0f;
bool leafDirection = true;

//----------------------------------------------- Resolusi ------------------------------------------------------
void reshape(int width, int height) {
    glViewport(0, 0, width, height);

    // Masuk ke mode proyeksi untuk mengatur ulang volume tampilan
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Hitung aspek rasio jendela
    float aspect = (float)width / (float)height;

    // Sesuaikan volume proyeksi ortografis
    if (aspect >= 1.0f) {
        // Lebar lebih besar dari tinggi
        glOrtho(-250.0 * aspect, 250.0 * aspect, -250.0, 250.0, -1.0, 1.0);
    }
    else {
        // Tinggi lebih besar dari lebar
        glOrtho(-250.0, 250.0, -250.0 / aspect, 250.0 / aspect, -1.0, 1.0);
    }

    // Kembali ke mode modelview
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//------------------------------------------ Pohon Kelapa --------------------------------------------------
void daunKelapa(float x, float y, float length, float angleOffset, int segments, float thickness) {
    // Tambahkan beberapa garis sejajar untuk menambah ketebalan
    for (float offset = -thickness; offset <= thickness; offset += thickness / 3) {
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i <= segments; i++) {
            float t = (float)i / segments;
            float angle = angleOffset + (1.0f - t) * 0.5f * sin(leafAngle * 3.14159 / 180.0f); // Efek melambai
            float xPos = x + t * length * cos(angle);
            float yPos = y + t * length * sin(angle);
            glVertex2f(xPos + offset * sin(angle), yPos - offset * cos(angle)); // Offset untuk menambah ketebalan
        }
        glEnd();
    }
}

void pohon(float x, float y, float scale) {
    // Batang pohon
    glColor3f(0.55f, 0.27f, 0.07f); // Warna coklat
    glBegin(GL_QUADS);
    glVertex2f(x - 35 * scale, y - 103 * scale); // Kiri bawah
    glVertex2f(x + 35 * scale, y - 103 * scale); // Kanan bawah
    glVertex2f(x + 20 * scale, y + 20 * scale);  // Kanan atas
    glVertex2f(x - 20 * scale, y + 20 * scale);  // Kiri atas
    glEnd();

    glColor3f(0.55f, 0.27f, 0.07f);
    glBegin(GL_QUADS);
    glVertex2f(x - 20 * scale, y + 20 * scale);  // Kiri bawah
    glVertex2f(x + 20 * scale, y + 20 * scale);  // Kanan bawah
    glVertex2f(x + 20 * scale, y + 250 * scale); // Kanan atas
    glVertex2f(x - 20 * scale, y + 250 * scale); // Kiri atas
    glEnd();

    // Daun
    glColor3ub(34, 139, 34);
    int leafCount = 18; // Jumlah daun utama lebih banyak
    for (int i = 0; i < leafCount; i++) {
        float angleOffset = (i * 360.0f / leafCount - 90.0f) * 3.14159 / 180.0f; // Sebar sudut lebih merata
        daunKelapa(x, y + 100, 70.0f, angleOffset, 40, 3.0f); // Daun lebih panjang dan tebal
        daunKelapa(x, y + 100, 60.0f, angleOffset + 0.1f, 40, 2.0f); // Tambahan daun lebih pendek dan lebih tipis //x, y + posisiY, panjangDaun
    }
}

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
void kotak(float left, float right, float bottom, float top) {
    glBegin(GL_POLYGON);
    glVertex2f(left, bottom);
    glVertex2f(right, bottom);
    glVertex2f(right, top);
    glVertex2f(left, top);
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
    glClearColor(135.0 / 255.0, 206.0 / 255.0, 235.0 / 255.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // Clear buffer

    // Matahari
    glColor3ub(255, 215, 0);
    matahari(0, 200);

    // Awan
    glPushMatrix();
    glTranslatef(tx, 0, 0);
    awan(-450.0f, -20.0f);
    awan(-370.0f, -65.0f);
    awan(-310.0f, 0.0f);
    awan(-230.0f, -30.0f);
    awan(-150.0f, -70.0f);
    awan(-50.0f, -50.0f);
    awan(50.0f, -30.0f);
    awan(100.0f, -50.0f);
    awan(210.0f, -50.0f);
    awan(250.0f, -20.0f);
    awan(310.0f, 0.0f);
    awan(370.0f, -65.0f);
    awan(430.0f, -40.0f);

    glPopMatrix();
    tx += 0.1;
    if (tx > 250)
        tx = -250;

    // Gunung
    glColor3ub(170, 153, 111);
    gunung(190.0f, 200.0f, 230.0f, 400.0f, 0.5f);

    glColor3ub(194, 178, 128);
    gunung(-50.0f, 200.0f, 230.0f, 400.0f, 0.5f);
    gunung(60.0f, 200.0f, 230.0f, 400.0f, 0.5f);

    glColor3ub(170, 153, 111);
    gunung(-200.0f, 200.0f, 230.0f, 400.0f, 0.5f);

    // Alas
    glColor3ub(210, 192, 145);
    kotak(-250 * 2, 250 * 2, -250, 85); // Diperluas untuk mencakup seluruh jendela

    // Danau
    glColor3ub(118, 249, 249);
    float danauCenterX = 0.0f, danauCenterY = -90.0f;
    float danauRadiusX = 300.0f, danauRadiusY = 100.0f;
    danau(danauCenterX, danauCenterY, danauRadiusX, danauRadiusY);

    // Set Stencil Buffer untuk Refleksi
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 1, 0xFF); // Selalu tulis ke stencil buffer
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); // Tulis ke stencil buffer jika tes berhasil
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); // Nonaktifkan penggambaran ke layar

    // Gambarkan bentuk danau ke stencil buffer
    glBegin(GL_POLYGON);
    for (float angle = 0.0f; angle <= 2 * 3.14159f; angle += 0.1f) {
        float x = danauCenterX + danauRadiusX * cos(angle);
        float y = danauCenterY + danauRadiusY * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); // Aktifkan kembali penggambaran ke layar
    glStencilFunc(GL_EQUAL, 1, 0xFF); // Gambar hanya di area yang ditentukan oleh stencil
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); // Jangan ubah nilai stencil buffer


    // Tumbleweed bawah (refleksi)
    glColor3ub(105, 92, 59); //warna rgb
    glPushMatrix();
    glScalef(1.0f, -1.0f, 1.0f);
    glTranslatef(bx, 20, 0); //posisi tumbleweed
    glRotatef(angle, 0.0f, 0.0f, 1.0f);
    tumbleweed(0.0f, 0.0f, 25.0f, 40, 1.5f); //(?,?,?,ketebalan)
    glPopMatrix();

    // Pohon Kelapa (Refleksi)
    glPushMatrix();
    glScalef(1.0f, -1.0f, 1.0f);
    glTranslatef(220, 60, 0);
    pohon(0, 0, 0.4f);
    glPopMatrix();

    glDisable(GL_STENCIL_TEST); // Nonaktifkan stencil buffer (refleksi untuk objek setelah kode ini)

    // Tumbleweed
    glColor3ub(0, 0, 0); //warna rgb
    glPushMatrix();
    glTranslatef(bx, 42, 0); //posisi tumbleweed
    glRotatef(angle, 0.0f, 0.0f, 1.0f);
    tumbleweed(0.0f, 0.0f, 25.0f, 40, 1.5f); //(?,?,?,ketebalan)
    glPopMatrix();

    // Pohon Kelapa
    pohon(220, 30, 0.4f); //posisi pohon kelapa berdiri

    // Update posisi tumbleweed
    angle -= 0.01;
    bx += 1;
    if (bx > 600)
        bx = -570;
    if (angle <= -360.0f)
        angle += 360.0f;

    // Gerakan daun
    if (leafDirection)
        leafAngle += 0.5f;
    else
        leafAngle -= 0.5f;

    if (leafAngle > 15.0f)
        leafDirection = false;
    else if (leafAngle < -15.0f)
        leafDirection = true;

    glutPostRedisplay();
    glFlush();
}

void init() {
    glClearColor(135.0 / 255.0, 206.0 / 255.0, 235.0 / 255.0, 1.0);
    glOrtho(-250.0, 250.0, -250.0, 250.0, -1.0, 1.0);
    glEnable(GL_STENCIL_TEST); // Aktifkan stencil buffer
}

int main(int argc, char** argv) {
    srand(time(0)); // Inisialisasi random seed
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(1600, 900);
    glutCreateWindow("Pohon Kelapa dan Tumbleweed");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape); // Tambahkan callback reshape
    glutMainLoop();

    return 0;
}