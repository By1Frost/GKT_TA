//#include <windows.h>
//#include <GL/glut.h>
//#include <stdlib.h>
//#include <math.h>
//#include <time.h> 
//
//double r = .2, s = .3; //r = jari-jari komponen awan; s = jari-jari lingkaran matahari(radius)
//int i;
//float tx = 10; //Variabel untuk awan bergerak ke kanan
//float bx = -570; //variabel untuk tumbleweed bergerak ke kanan
//float angle = 0.0f; //varibel untuk menyimpan sudut
//float leafAngle = 0.0f; //variabel untuk menyimpan sudut daun pohon
//bool leafDirection = true; //varibel untuk menentukan arah gerakan daun
//
//// Variabel untuk gerakan tumbleweed melompat
//float by = 42;           // Posisi y awal tumbleweed
//float jumpPhase = 0.0f;  // Fase gerakan melompat
//float jumpAmplitude = 40.0f; // Amplitudo lompatan (tinggi lompatan)
//float jumpSpeed = 0.01f;  // Kecepatan perubahan fase
//
////----------------------------------------------- Resolusi ------------------------------------------------------
//void reshape(int width, int height) {
//    glViewport(0, 0, width, height);
//
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//
//    float aspect = (float)width / (float)height;
//
//    if (aspect >= 1.0f) {
//        glOrtho(-250.0 * aspect, 250.0 * aspect, -250.0, 250.0, -1.0, 1.0);
//    }
//    else {
//        glOrtho(-250.0, 250.0, -250.0 / aspect, 250.0 / aspect, -1.0, 1.0);
//    }
//
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//}
//
////------------------------------------------ Pohon --------------------------------------------------
//void daunPohon(float x, float y, float length, float angleOffset, int segments, float thickness) {
//    for (float offset = -thickness; offset <= thickness; offset += thickness / 3) {
//        glBegin(GL_LINE_STRIP);
//        for (int i = 0; i <= segments; i++) {
//            float t = (float)i / segments;
//            float angle = angleOffset + (1.0f - t) * 0.5f * sin(leafAngle * 3.14159 / 180.0f);
//            float xPos = x + t * length * cos(angle);
//            float yPos = y + t * length * sin(angle);
//            glVertex2f(xPos + offset * sin(angle), yPos - offset * cos(angle));
//        }
//        glEnd();
//    }
//}
//
//void pohon(float x, float y, float scale, float trunkColor[4], float leafColor[4]) {
//    glColor4fv(trunkColor);
//    glBegin(GL_QUADS);
//    glVertex2f(x - 35 * scale, y - 103 * scale);
//    glVertex2f(x + 35 * scale, y - 103 * scale);
//    glVertex2f(x + 20 * scale, y + 20 * scale);
//    glVertex2f(x - 20 * scale, y + 20 * scale);
//    glEnd();
//
//    glBegin(GL_QUADS);
//    glVertex2f(x - 20 * scale, y + 20 * scale);
//    glVertex2f(x + 20 * scale, y + 20 * scale);
//    glVertex2f(x + 20 * scale, y + 250 * scale);
//    glVertex2f(x - 20 * scale, y + 250 * scale);
//    glEnd();
//
//    glColor4fv(leafColor);
//    int leafCount = 18;
//    for (int i = 0; i < leafCount; i++) {
//        float angleOffset = (i * 360.0f / leafCount - 90.0f) * 3.14159 / 180.0f;
//        daunPohon(x, y + 100, 70.0f, angleOffset, 40, 3.0f);
//        daunPohon(x, y + 100, 60.0f, angleOffset + 0.1f, 40, 2.0f);
//    }
//}
//
////------------------------------------------ Tumbleweed --------------------------------------------------
//void tumbleweed(float x, float y, float radius, int numSpokes, float lineWidth) {
//    srand(42);
//    glLineWidth(lineWidth);
//
//    glBegin(GL_LINE_LOOP);
//    for (int i = 0; i < 360; i += 5) {
//        float theta = i * 3.14159f / 180.0f;
//        float cx = x + radius * cos(theta);
//        float cy = y + radius * sin(theta);
//        glVertex2f(cx, cy);
//    }
//    glEnd();
//
//    glBegin(GL_LINES);
//    for (int i = 0; i < numSpokes; i++) {
//        float angle = (float)(rand() % 360) * 3.14159f / 180.0f;
//        float spokeRadius = radius + (rand() % 20 - 10);
//        float spokeX = x + spokeRadius * cos(angle);         
//        float spokeY = y + spokeRadius * sin(angle);
//
//        float baseRadius = radius / 2 + (rand() % (int)(radius / 2));
//        float baseX = x + baseRadius * cos(angle + (rand() % 20 - 5) * 0.1f);
//        float baseY = y + baseRadius * sin(angle + (rand() % 20 - 5) * 0.1f);
//
//        glVertex2f(baseX, baseY);
//        glVertex2f(spokeX, spokeY);
//    }
//    glEnd();
//}
//
////------------------------------------------ Tanah --------------------------------------------------
//void kotak(float left, float right, float bottom, float top) {
//    glBegin(GL_POLYGON);
//    glVertex2f(left, bottom);
//    glVertex2f(right, bottom);
//    glVertex2f(right, top);
//    glVertex2f(left, top);
//    glEnd();
//}
//
////------------------------------------------ Matahari --------------------------------------------------
//void matahari(double x, double y) {
//    glBegin(GL_TRIANGLE_FAN);
//    for (i = 0; i < 360; i++) {
//        x = x + cos((i * 3.14) / 180) * s;
//        y = y + sin((i * 3.14) / 180) * s;
//        glVertex2d(x, y);
//    }
//    glEnd();
//}
//
////------------------------------------------ Awan --------------------------------------------------
//void bulat(double x, double y) {
//    glBegin(GL_TRIANGLE_FAN);
//    for (i = 0; i < 360; i++) {
//        x = x + cos((i * 3.14) / 180) * r;
//        y = y + sin((i * 3.14) / 180) * r;
//        glVertex2d(x, y);
//    }
//    glEnd();
//}
//
//void awan(float tx, float ty) {
//    glColor3ub(255, 255, 255);
//    bulat(tx + 0, ty + 250);
//    bulat(tx + 15, ty + 245);
//    bulat(tx + 10, ty + 240);
//    bulat(tx + -2, ty + 243);
//}
//
////------------------------------------------ Gunung --------------------------------------------------
//void gunung(float tx, float ty, float t, float l, float scale) {
//    glBegin(GL_TRIANGLES);
//    t *= scale;
//    l *= scale;
//    float xA = tx - l / 2, yA = ty - t;
//    float xB = tx, yB = ty;
//    float xC = tx + l / 2, yC = ty - t;
//
//    glVertex2f(xA, yA);
//    glVertex2f(xB, yB);
//    glVertex2f(xC, yC);
//
//    glEnd();
//}
//
////------------------------------------------ Danau --------------------------------------------------
//void danau(float centerX, float centerY, float radiusX, float radiusY) {
//    glBegin(GL_POLYGON);
//    for (float angle = 0.0f; angle <= 2 * 3.14159f; angle += 0.1f) {
//        float x = centerX + radiusX * cos(angle);
//        float y = centerY + radiusY * sin(angle);
//        glVertex2f(x, y);
//    }
//    glEnd();
//}
//
////------------------------------------------ Display --------------------------------------------------
//void display() {
//    glClearColor(135.0 / 255.0, 206.0 / 255.0, 235.0 / 255.0, 1.0);
//    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
//
//    float trunkColor[4] = { 140 / 255.0f, 69 / 255.0f, 18 / 255.0f, 1.0f };
//    float leafColor[4] = { 34 / 255.0f, 139 / 255.0f, 34 / 255.0f, 1.0f };
//
//    glColor3ub(255, 215, 0);
//    matahari(0, 200);
//
//    glPushMatrix();
//    glTranslatef(tx, 0, 0); //gerakan awan ke kanan
//    awan(-450.0f, -20.0f);
//    awan(-370.0f, -65.0f);
//    awan(-310.0f, 0.0f);
//    awan(-230.0f, -30.0f);
//    awan(-150.0f, -70.0f);
//    awan(-50.0f, -50.0f);
//    awan(50.0f, -30.0f);
//    awan(100.0f, -50.0f);
//    awan(210.0f, -50.0f);
//    awan(250.0f, -20.0f);
//    awan(310.0f, 0.0f);
//    awan(370.0f, -65.0f);
//    awan(430.0f, -40.0f);
//    glPopMatrix();
//    tx += 0.1;
//    if (tx > 250)
//        tx = -250;
//
//    glColor3ub(170, 153, 111);
//    gunung(190.0f, 200.0f, 230.0f, 400.0f, 0.5f);
//
//    glColor3ub(194, 178, 128);
//    gunung(-50.0f, 200.0f, 230.0f, 400.0f, 0.5f);
//    gunung(60.0f, 200.0f, 230.0f, 400.0f, 0.5f);
//
//    glColor3ub(170, 153, 111);
//    gunung(-200.0f, 200.0f, 230.0f, 400.0f, 0.5f);
//
//    glColor3ub(210, 192, 145);
//    kotak(-250 * 2, 250 * 2, -250, 85);
//
//    glColor3ub(118, 249, 249);
//    float danauCenterX = 0.0f, danauCenterY = -90.0f;
//    float danauRadiusX = 300.0f, danauRadiusY = 100.0f;
//    danau(danauCenterX, danauCenterY, danauRadiusX, danauRadiusY);
//
//    glEnable(GL_STENCIL_TEST);
//    glStencilFunc(GL_ALWAYS, 1, 0xFF);
//    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
//    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
//
//    glBegin(GL_POLYGON);
//    for (float angle = 0.0f; angle <= 2 * 3.14159f; angle += 0.1f) {
//        float x = danauCenterX + danauRadiusX * cos(angle);
//        float y = danauCenterY + danauRadiusY * sin(angle);
//        glVertex2f(x, y);
//    }
//    glEnd();
//
//    //=================================== REFLEKSI =================================================
//    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); // Mengaktifkan kembali penggambaran warna ke layar, setelah sebelumnya dinonaktifkan
//    glStencilFunc(GL_EQUAL, 1, 0xFF); // GL_EQUAL = Tes berhasil jika nilai stencil saat ini sama dengan referensi (1).
//    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); // Jangan ubah nilai stencil buffer
//
//    // Refleksi Awan
//    glPushMatrix();
//    glScalef(1.0f, -1.0f, 1.0f); //menerapkan transformasi skala (perubahan ukuran) terhadap objek yang akan digambar
//    //(tidak ada perubahan pada sumbu x, membalikkan sumbu y untuk menciptakan efek pantulan, tidak ada perubahan pada sumbu z (tidak relevan di 2d))
//    //oleh karena transformasi skala ini bisa digunakan untuk menciptakan refleksi
//    glTranslatef(tx, 0, 0); // transformasi horizontal
//    awan(-370.0f, -65.0f);
//    awan(-150.0f, -70.0f);
//    awan(370.0f, -65.0f);
//    glPopMatrix();
//
//    // Refleksi Gunung
//    glPushMatrix();
//    glScalef(1.0f, -1.0f, 1.0f); //menerapkan transformasi skala (perubahan ukuran), dan lebih tepatnya merefleksikan, terhadap objek yang akan digambar
//    glTranslatef(0, 400.0f, 0); //translasi y (sesuaikan posisinya)
//    glColor4f(170 / 255.0f, 153 / 255.0f, 111 / 255.0f, 1.0f); //rgba
//    gunung(190.0f, -200.0f, 230.0f, 400.0f, 0.5f);
//    //tx = titik x puncak gunung; ty = titik y puncak gunung; t = tinggi gunung; l = lebar gunung; scale = skala gunung
//
//    glColor4f(194 / 255.0f, 178 / 255.0f, 128 / 255.0f, 1.0f);
//    gunung(-50.0f, -200.0f, 230.0f, 400.0f, 0.5f);
//    gunung(60.0f, -200.0f, 230.0f, 400.0f, 0.5f);
//
//    glColor4f(170 / 255.0f, 153 / 255.0f, 111 / 255.0f, 1.0f);
//    gunung(-200.0f, -200.0f, 230.0f, 400.0f, 0.5f);
//    glPopMatrix();
//
//    // Refleksi Tanah
//    glPushMatrix();
//    glScalef(1.0f, -1.0f, 1.0f); //transformasi skala refleksi trhdp sumbu x (balik vertikal)
//    glTranslatef(0, 170.0f, 0); // Sesuaikan posisi y refleksi
//    glColor4f(210 / 255.0f, 192 / 255.0f, 145 / 255.0f, 1.0f); //rgba
//    kotak(-250 * 2, 250 * 2, -250, -85);
//    glPopMatrix();
//
//    //Tumbleweed (refleksi)
//    glColor4f(105 / 255.0f, 92 / 255.0f, 59 / 255.0f, 1.0f); //(rgba)
//    glPushMatrix();
//    glScalef(1.0f, -1.0f, 1.0f); //transformasi skala refleksi trhdp sumbu-x (balik vertikal)
//    glTranslatef(bx, by, 0); //trasnformasi translasi posisi tumbleweed (akan berubah posisi, animasi)
//    glRotatef(angle, 0.0f, 0.0f, 1.0f); //transformasi rotasi
//    tumbleweed(0.0f, 0.0f, 25.0f, 200, 1.5f); //(x,y,radius,jumlah jari-jari,ketebalan)
//    glPopMatrix();
//
//    // Pohon (Refleksi)
//    glPushMatrix();
//    glScalef(1.0f, -1.0f, 1.0f); //transformasi skala refleksi trhdp sumbu x (balik vertikal)
//    glTranslatef(220, 60, 0);
//    pohon(0, 0, 0.4f, trunkColor, leafColor); //x,y,skala,warna batang, warna daun
//    glPopMatrix();
//
//    // Danau
//    glPushMatrix();
//    glColor4f(118 / 255.0f, 249 / 255.0f, 249 / 255.0f, 0.5f); //rgba
//    danau(danauCenterX, danauCenterY, danauRadiusX, danauRadiusY); //memanggil danau
//    glPopMatrix();
//
//    glDisable(GL_STENCIL_TEST); //matikan stencil buffer (crop danau untuk objek setelah kode ini dimatikan)
//    //=================================== ORIGINAL =================================================
//
//    // Tumbleweed
//    glColor4f(105 / 255.0f, 92 / 255.0f, 59 / 255.0f, 1.0f);
//    glPushMatrix();
//    glTranslatef(bx, by, 0); // gerakan tumbleweed vertikal horizontal
//    glRotatef(angle, 0.0f, 0.0f, 1.0f);
//    tumbleweed(0.0f, 0.0f, 25.0f, 200, 1.5f);
//    glPopMatrix();
//
//    pohon(220, 30, 0.4f, trunkColor, leafColor);
//
//    // Update posisi tumbleweed
//    angle -= 0.5; //mengatur kecepatan putar gerak tumbleweed
//    bx += 0.5; //mengatur kecepatan ke kanan gerak tumbleweed
//    jumpPhase += jumpSpeed; // Perbarui fase lompatan
//    by = 42 + jumpAmplitude * fabs(sin(jumpPhase));
//
//    if (bx > 600)
//        bx = -570;
//    if (angle <= -360.0f)
//        angle += 360.0f;
//
//    // Gerakan daun
//    if (leafDirection)
//        leafAngle += 0.5f; //kecepatan gerakan daun
//    else
//        leafAngle -= 0.5f; //kecepatan gerakan daun
//
//    if (leafAngle > 15.0f) //membalikkan arah rotasi
//        leafDirection = false;
//    else if (leafAngle < -15.0f) //membalikkan arah rotasi lagi
//        leafDirection = true;
//
//    glutPostRedisplay(); //Fungsi ini memberi tahu OpenGL Utility Toolkit(GLUT) bahwa window perlu diperbarui atau digambar ulang.
//    glFlush(); //Fungsi ini memastikan bahwa semua perintah OpenGL yang berada dalam antrian command buffer akan segera dijalankan oleh GPU.
//}
//
//void init() {
//    glClearColor(135.0 / 255.0, 206.0 / 255.0, 235.0 / 255.0, 1.0);
//    glOrtho(-250.0, 250.0, -250.0, 250.0, -1.0, 1.0);
//    glEnable(GL_STENCIL_TEST);
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//}
//
//int main(int argc, char** argv) {
//    srand(time(0));
//    glutInit(&argc, argv);
//    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
//
//    glutInitWindowSize(1600, 900);
//    glutCreateWindow("KELOMPOK 20 'GURUN OASIS'");
//
//    init();
//    glutDisplayFunc(display);
//    glutReshapeFunc(reshape);
//    glutMainLoop();
//
//    return 0;
//}

#include <windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

double r = .2, s = .3; //r = jari-jari komponen awan; s = jari-jari lingkaran matahari(radius)
int i;
float tx = 10; //Variabel untuk awan bergerak ke kanan
float bx = -570; //variabel untuk tumbleweed bergerak ke kanan
float angle = 0.0f; //varibel untuk menyimpan sudut
float leafAngle = 0.0f; //variabel untuk menyimpan sudut daun pohon
bool leafDirection = true; //varibel untuk menentukan arah gerakan daun

// Variabel untuk gerakan tumbleweed melompat
float by = 42;           // Posisi y awal tumbleweed
float jumpPhase = 0.0f;  // Fase gerakan melompat
float jumpAmplitude = 40.0f; // Amplitudo lompatan (tinggi lompatan)
float jumpSpeed = 0.01f;  // Kecepatan perubahan fase

//----------------------------------------------- Resolusi ------------------------------------------------------
void reshape(int width, int height) {
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect = (float)width / (float)height;

    if (aspect >= 1.0f) {
        glOrtho(-250.0 * aspect, 250.0 * aspect, -250.0, 250.0, -1.0, 1.0);
    }
    else {
        glOrtho(-250.0, 250.0, -250.0 / aspect, 250.0 / aspect, -1.0, 1.0);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//------------------------------------------ Pohon --------------------------------------------------
void daunPohon(float x, float y, float length, float angleOffset, int segments, float thickness) {
    for (float offset = -thickness; offset <= thickness; offset += thickness / 3) {
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i <= segments; i++) {
            float t = (float)i / segments;
            float angle = angleOffset + (1.0f - t) * 0.5f * sin(leafAngle * 3.14159 / 180.0f);
            float xPos = x + t * length * cos(angle);
            float yPos = y + t * length * sin(angle);
            glVertex2f(xPos + offset * sin(angle), yPos - offset * cos(angle));
        }
        glEnd();
    }
}

void pohon(float x, float y, float scale, float trunkColor[4], float leafColor[4]) {
    glColor4fv(trunkColor);
    glBegin(GL_QUADS);
    glVertex2f(x - 35 * scale, y - 103 * scale);
    glVertex2f(x + 35 * scale, y - 103 * scale);
    glVertex2f(x + 20 * scale, y + 20 * scale);
    glVertex2f(x - 20 * scale, y + 20 * scale);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(x - 20 * scale, y + 20 * scale);
    glVertex2f(x + 20 * scale, y + 20 * scale);
    glVertex2f(x + 20 * scale, y + 250 * scale);
    glVertex2f(x - 20 * scale, y + 250 * scale);
    glEnd();

    glColor4fv(leafColor);
    int leafCount = 18;
    for (int i = 0; i < leafCount; i++) {
        float angleOffset = (i * 360.0f / leafCount - 90.0f) * 3.14159 / 180.0f;
        daunPohon(x, y + 100, 70.0f, angleOffset, 40, 3.0f);
        daunPohon(x, y + 100, 60.0f, angleOffset + 0.1f, 40, 2.0f);
    }
}

//------------------------------------------ Tumbleweed --------------------------------------------------
void tumbleweed(float x, float y, float radius, int numSpokes, float lineWidth) {
    srand(42);
    glLineWidth(lineWidth);

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 360; i += 5) {
        float theta = i * 3.14159f / 180.0f;
        float cx = x + radius * cos(theta);
        float cy = y + radius * sin(theta);
        glVertex2f(cx, cy);
    }
    glEnd();

    glBegin(GL_LINES);
    for (int i = 0; i < numSpokes; i++) {
        float angle = (float)(rand() % 360) * 3.14159f / 180.0f;
        float spokeRadius = radius + (rand() % 20 - 10);
        float spokeX = x + spokeRadius * cos(angle);
        float spokeY = y + spokeRadius * sin(angle);

        float baseRadius = radius / 2 + (rand() % (int)(radius / 2));
        float baseX = x + baseRadius * cos(angle + (rand() % 20 - 5) * 0.1f);
        float baseY = y + baseRadius * sin(angle + (rand() % 20 - 5) * 0.1f);

        glVertex2f(baseX, baseY);
        glVertex2f(spokeX, spokeY);
    }
    glEnd();
}

//------------------------------------------ Tanah --------------------------------------------------
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
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    float trunkColor[4] = { 140 / 255.0f, 69 / 255.0f, 18 / 255.0f, 1.0f };
    float leafColor[4] = { 34 / 255.0f, 139 / 255.0f, 34 / 255.0f, 1.0f };

    glColor3ub(255, 215, 0);
    matahari(0, 200);

    glPushMatrix();
    glTranslatef(tx, 0, 0); //gerakan awan ke kanan
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
    //==== Mulai gerak awan ====
    tx += 0.1;
    if (tx > 250)
        tx = -250;
    //==== Selesai Gerak awan ====

    glColor3ub(170, 153, 111);
    gunung(190.0f, 200.0f, 230.0f, 400.0f, 0.5f);

    glColor3ub(194, 178, 128);
    gunung(-50.0f, 200.0f, 230.0f, 400.0f, 0.5f);
    gunung(60.0f, 200.0f, 230.0f, 400.0f, 0.5f);

    glColor3ub(170, 153, 111);
    gunung(-200.0f, 200.0f, 230.0f, 400.0f, 0.5f);

    glColor3ub(210, 192, 145);
    kotak(-250 * 2, 250 * 2, -250, 85);

    glColor3ub(118, 249, 249);
    float danauCenterX = 0.0f, danauCenterY = -90.0f;
    float danauRadiusX = 300.0f, danauRadiusY = 100.0f;
    danau(danauCenterX, danauCenterY, danauRadiusX, danauRadiusY);

    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

    glBegin(GL_POLYGON);
    for (float angle = 0.0f; angle <= 2 * 3.14159f; angle += 0.1f) {
        float x = danauCenterX + danauRadiusX * cos(angle);
        float y = danauCenterY + danauRadiusY * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();

    //=================================== REFLEKSI =================================================
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); // Mengaktifkan kembali penggambaran warna ke layar, setelah sebelumnya dinonaktifkan
    glStencilFunc(GL_EQUAL, 1, 0xFF); // GL_EQUAL = Tes berhasil jika nilai stencil saat ini sama dengan referensi (1).
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); // Jangan ubah nilai stencil buffer

    //==== Mulai Refleksi ====
    // Refleksi Awan
    glPushMatrix();
    glScalef(1.0f, -1.0f, 1.0f); //menerapkan transformasi skala (perubahan ukuran) terhadap objek yang akan digambar
    //(tidak ada perubahan pada sumbu x, membalikkan sumbu y untuk menciptakan efek pantulan, tidak ada perubahan pada sumbu z (tidak relevan di 2d))
    //oleh karena transformasi skala ini bisa digunakan untuk menciptakan refleksi
    glTranslatef(tx, 0, 0); //transformasi horizontal
    awan(-370.0f, -65.0f);
    awan(-150.0f, -70.0f);
    awan(370.0f, -65.0f);
    glPopMatrix();

    // Refleksi Gunung
    glPushMatrix();
    glScalef(1.0f, -1.0f, 1.0f); //menerapkan transformasi skala (perubahan ukuran), dan lebih tepatnya merefleksikan, terhadap objek yang akan digambar
    glTranslatef(0, 400.0f, 0); //translasi y (sesuaikan posisinya)
    glColor4f(170 / 255.0f, 153 / 255.0f, 111 / 255.0f, 1.0f); //rgba
    gunung(190.0f, -200.0f, 230.0f, 400.0f, 0.5f); 
    //tx = titik x puncak gunung; ty = titik y puncak gunung; t = tinggi gunung; l = lebar gunung; scale = skala gunung

    glColor4f(194 / 255.0f, 178 / 255.0f, 128 / 255.0f, 1.0f);
    gunung(-50.0f, -200.0f, 230.0f, 400.0f, 0.5f);
    gunung(60.0f, -200.0f, 230.0f, 400.0f, 0.5f);

    glColor4f(170 / 255.0f, 153 / 255.0f, 111 / 255.0f, 1.0f);
    gunung(-200.0f, -200.0f, 230.0f, 400.0f, 0.5f);
    glPopMatrix();

    // Refleksi Tanah
    glPushMatrix();
    glScalef(1.0f, -1.0f, 1.0f); //transformasi skala refleksi trhdp sumbu x (balik vertikal)
    glTranslatef(0, 170.0f, 0); //Sesuaikan posisi y refleksi
    glColor4f(210 / 255.0f, 192 / 255.0f, 145 / 255.0f, 1.0f); //rgba
    kotak(-250 * 2, 250 * 2, -250, -85);
    glPopMatrix();

    //Tumbleweed (refleksi)
    glColor4f(105 / 255.0f, 92 / 255.0f, 59 / 255.0f, 1.0f); //(rgba)
    glPushMatrix();
    glScalef(1.0f, -1.0f, 1.0f); //transformasi skala refleksi trhdp sumbu-x (balik vertikal)
    glTranslatef(bx, by, 0); //trasnformasi translasi posisi tumbleweed (akan berubah posisi, animasi)
    glRotatef(angle, 0.0f, 0.0f, 1.0f); //transformasi rotasi
    tumbleweed(0.0f, 0.0f, 25.0f, 200, 1.5f); //(x,y,radius,jumlah jari-jari,ketebalan)
    glPopMatrix();

    // Pohon (Refleksi)
    glPushMatrix();
    glScalef(1.0f, -1.0f, 1.0f); //transformasi skala refleksi trhdp sumbu x (balik vertikal)
    glTranslatef(220, 60, 0);
    pohon(0, 0, 0.4f, trunkColor, leafColor); //x,y,skala,warna batang, warna daun
    glPopMatrix();

    // Danau
    glPushMatrix();
    glColor4f(118 / 255.0f, 249 / 255.0f, 249 / 255.0f, 0.5f); //rgba
    danau(danauCenterX, danauCenterY, danauRadiusX, danauRadiusY); //memanggil danau
    glPopMatrix();

    //==== Selesai Refleksi ====

    glDisable(GL_STENCIL_TEST); //matikan stencil buffer (crop danau untuk objek setelah kode ini dimatikan)
    //=================================== ORIGINAL =================================================

    // Tumbleweed
    glColor4f(105 / 255.0f, 92 / 255.0f, 59 / 255.0f, 1.0f);
    glPushMatrix();
    glTranslatef(bx, by, 0); // gerakan tumbleweed vertikal horizontal
    glRotatef(angle, 0.0f, 0.0f, 1.0f);
    tumbleweed(0.0f, 0.0f, 25.0f, 200, 1.5f);
    glPopMatrix();

    pohon(220, 30, 0.4f, trunkColor, leafColor);

    //==== Mulai gerak tumbleweed ====
    angle -= 0.5; //mengatur kecepatan putar gerak tumbleweed
    bx += 0.5; //mengatur kecepatan ke kanan gerak tumbleweed
    jumpPhase += jumpSpeed; // Perbarui fase lompatan
    by = 42 + jumpAmplitude * fabs(sin(jumpPhase));

    if (bx > 600)
        bx = -570;
    if (angle <= -360.0f)
        angle += 360.0f;
    //==== Selesai gerak tumbleweed ====

    //==== Mulai gerak daun ====
    if (leafDirection)
        leafAngle += 0.5f; //kecepatan gerakan daun
    else
        leafAngle -= 0.5f; //kecepatan gerakan daun

    if (leafAngle > 15.0f) //membalikkan arah rotasi
        leafDirection = false;
    else if (leafAngle < -15.0f) //membalikkan arah rotasi lagi
        leafDirection = true;
     //==== Mulai gerak tumbleweed ====

    glutPostRedisplay(); //Fungsi ini memberi tahu OpenGL Utility Toolkit(GLUT) bahwa window perlu diperbarui atau digambar ulang.
    glFlush(); //Fungsi ini memastikan bahwa semua perintah OpenGL yang berada dalam antrian command buffer akan segera dijalankan oleh GPU.
}

void init() {
    glClearColor(135.0 / 255.0, 206.0 / 255.0, 235.0 / 255.0, 1.0);
    glOrtho(-250.0, 250.0, -250.0, 250.0, -1.0, 1.0);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int main(int argc, char** argv) {
    srand(time(0));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);

    glutInitWindowSize(1600, 900);
    glutCreateWindow("KELOMPOK 20 'GURUN OASIS'");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();

    return 0;
}