#include <windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h> // untuk penghitungan matematis (sin cos)
#include <time.h> // untuk fungsi random

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
void reshape(int width, int height) { //memungkinkan konten pada layar tetap proporsional, meskipun ukuran jendela diubah oleh pengguna.
    glViewport(0, 0, width, height); //area opengl,menggambar 0,0(sudut kiri bawah) > width,height(sudut kanan atas/lebar dan tinggi window)

    // Masuk ke mode proyeksi untuk mengatur ulang volume tampilan 
    glMatrixMode(GL_PROJECTION); //mengatur cara OpenGL memproyeksikan objek 3D ke layar 2D.
    glLoadIdentity(); //mengatur ulang matriks proyeksi ke bentuk identitas (menghapus transformasi sebelumnya)

    // Hitung aspek rasio window
    float aspect = (float)width / (float)height;

    // Mengatur volume tampilan ortografis (tampilan tanpa distorsi perspektif)
    if (aspect >= 1.0f) { //jika lebar lebih dari tinggi (yang kami pakai)
        glOrtho(-250.0 * aspect, 250.0 * aspect, -250.0, 250.0, -1.0, 1.0);
    }
    else { //jika tinggi lebih dari lebar
        glOrtho(-250.0, 250.0, -250.0 / aspect, 250.0 / aspect, -1.0, 1.0);
    }

    // Kembali ke mode modelview
    glMatrixMode(GL_MODELVIEW); //Beralih kembali ke mode modelview, di mana transformasi model dan kamera diterapkan.
    glLoadIdentity(); //mengatur ulang matriks proyeksi ke bentuk identitas (menghapus transformasi sebelumnya)
}

//------------------------------------------ Pohon --------------------------------------------------
void daunPohon(float x, float y, float length, float angleOffset, int segments, float thickness) { //membuat daun pohon
    //x,y = koordinat awal daun; length = panjang daun; angleOffset = sudut awal daun relatif terhadap sumbu horizontal; segments = jumlah segmen untuk membentuk lengkungan; thickness = ketebalan daun
    // Tambahkan beberapa garis sejajar untuk menambah ketebalan
    for (float offset = -thickness; offset <= thickness; offset += thickness / 3) { //Iterasi beberapa garis dengan offset horizontal dan vertikal untuk menciptakan efek tebal
        glBegin(GL_LINE_STRIP); //menggambar garis
        for (int i = 0; i <= segments; i++) { //iterasi setiap segmen
            float t = (float)i / segments; //proporsi panjang daun
            float angle = angleOffset + (1.0f - t) * 0.5f * sin(leafAngle * 3.14159 / 180.0f); // mengatur arah melengkung daun dengan efek "melambai" yang kecil
            float xPos = x + t * length * cos(angle);
            float yPos = y + t * length * sin(angle);
            //xPos,yPos = Posisi setiap titik dihitung menggunakan trigonometri berdasarkan panjang daun dan sudutnya.
            glVertex2f(xPos + offset * sin(angle), yPos - offset * cos(angle)); // Offset untuk menambah ketebalan
        }
        glEnd();//selesai menggambar daun
    }
}

void pohon(float x, float y, float scale, float trunkColor[4], float leafColor[4]) { //membuat pohon (beserta daunnya)
    //x,y = titik berdiri pohon; scale = skala pohon; trunkColor = warna batang pohon; leafColor = warna daun
    // Batang pohon
    glColor4fv(trunkColor); // Warna batang
    glBegin(GL_QUADS); //menggambar persegi
    glVertex2f(x - 35 * scale, y - 103 * scale); // Kiri bawah
    glVertex2f(x + 35 * scale, y - 103 * scale); // Kanan bawah
    glVertex2f(x + 20 * scale, y + 20 * scale);  // Kanan atas
    glVertex2f(x - 20 * scale, y + 20 * scale);  // Kiri atas
    glEnd(); //selesai menggambar persegi

    glBegin(GL_QUADS); 
    glVertex2f(x - 20 * scale, y + 20 * scale);
    glVertex2f(x + 20 * scale, y + 20 * scale);
    glVertex2f(x + 20 * scale, y + 250 * scale);
    glVertex2f(x - 20 * scale, y + 250 * scale);
    glEnd();

    // Daun
    glColor4fv(leafColor); // Warna daun
    int leafCount = 18; // Jumlah daun
    for (int i = 0; i < leafCount; i++) { //itersai untuk setiap jumlah daun
        float angleOffset = (i * 360.0f / leafCount - 90.0f) * 3.14159 / 180.0f; // Lebar sudut dihitung dari (0-360)
        daunPohon(x, y + 100, 70.0f, angleOffset, 40, 3.0f); //pemanggilan daunPohon
        daunPohon(x, y + 100, 60.0f, angleOffset + 0.1f, 40, 2.0f);
        //x,y = koordinat awal daun; length = panjang daun; angleOffset = sudut awal daun relatif terhadap sumbu horizontal; segments = jumlah segmen untuk membentuk lengkungan; thickness = ketebalan 
    }
}

//------------------------------------------ Tumbleweed --------------------------------------------------
void tumbleweed(float x, float y, float radius, int numSpokes, float lineWidth) { //menciptakan tumbleweed (rumput mati)
    // x, y = koordinat titik pusat; radius = panjang jari-jari; numSpokes = jumlah jari-jari (garis acak); lineWidth = tebal jari-jari 
    srand(42); //generator angka acak, karena jari-jari yang akan dibuat akan beracak-acak
    glLineWidth(lineWidth); //Mengatur ketebalan garis

    // Lingkaran utama tumbleweed
    glBegin(GL_LINE_LOOP); //menggambar lingkaran utama
    for (int i = 0; i < 360; i += 5) { //Iterasi dari 0 hingga 360 derajat dengan langkah 5 derajat:
        float theta = i * 3.14159f / 180.0f; //konversi derajat ke radian
        float cx = x + radius * cos(theta); //menciptakan titik x baru
        float cy = y + radius * sin(theta); //menciptakan titik y baru
        glVertex2f(cx, cy); //menciptakan titik koordinat baru pembentukan keliling lingkaran
    }
    glEnd();//selesai gambar lingkaran

    // Jari-jari tumbleweed dengan variasi acak
    glBegin(GL_LINES); //menggambar garis
    for (int i = 0; i < numSpokes; i++) { //iterasi untuk setiap jumlah jari-jari
        float angle = (float)(rand() % 360) * 3.14159f / 180.0f; // Sudut acak antara 0-360, acaknya dari  fungsi rand()
        float spokeRadius = radius + (rand() % 20 - 10); // Panjang garis dihitung dari radius lingkaran dengan panjang acak (±10)
        float spokeX = x + spokeRadius * cos(angle); //koordinat x titik akhir garis          
        float spokeY = y + spokeRadius * sin(angle); //koordinat y titik akhir garis

        // Titik awal selalu di sekitar lingkaran (bukan hanya di pusat)
        float baseRadius = radius / 2 + (rand() % (int)(radius / 2)); 
        //baseRadius = Jarak titik awal garis dari pusat lingkaran (acak)
        float baseX = x + baseRadius * cos(angle + (rand() % 20 - 5) * 0.1f); //Koordinat titik awal x garis (acak)
        float baseY = y + baseRadius * sin(angle + (rand() % 20 - 5) * 0.1f); //Koordinat titik awal y garis (acak)

        // Gambar garis dari titik acak di dalam lingkaran ke ujung acak
        glVertex2f(baseX, baseY);
        glVertex2f(spokeX, spokeY);
    }
    glEnd(); //selesai gambar garis
}

//------------------------------------------ Tanah --------------------------------------------------
void kotak(float left, float right, float bottom, float top) { //menciptakan tanah
    //left = panjang kiri; right = panjang kanan; bottom = panjang kanan; top = panjang atas
    glBegin(GL_POLYGON); //menggambar poligon (segi banyak/bebas)
    glVertex2f(left, bottom); //menggambar garis
    glVertex2f(right, bottom);
    glVertex2f(right, top);
    glVertex2f(left, top);
    glEnd(); //selesai gambar poligon
}

//------------------------------------------ Matahari --------------------------------------------------
void matahari(double x, double y) { //membuat matahari
    //x,y koordinat pusat; double merupakan tipe data untuk bilangan pecahan
    glBegin(GL_TRIANGLE_FAN); //menggambar sebuah bentuk yang terdiri dari beberapa segitiga yang berbagi titik pusat
    //Titik pusat adalah pusat lingkaran matahari
    //Titik-titik lainnya membentuk keliling lingkaran
    for (i = 0; i < 360; i++) { //iterasi 360 derajat
        x = x + cos((i * 3.14) / 180) * s; //menghitung koordinat x baru
        y = y + sin((i * 3.14) / 180) * s; //menghitung koordinat y baru
        glVertex2d(x, y); //titik baru untuk membuat keliling lingkaran
    }
    glEnd();//selesai menggambar matahari
}

//------------------------------------------ Awan --------------------------------------------------
void bulat(double x, double y) { //komponen untuk membuat awan
    glBegin(GL_TRIANGLE_FAN);
    for (i = 0; i < 360; i++) {
        x = x + cos((i * 3.14) / 180) * r;
        y = y + sin((i * 3.14) / 180) * r;
        glVertex2d(x, y);
    }
    glEnd();//selesai menggambar awan
}

void awan(float tx, float ty) { //membuat awan
    glColor3ub(255, 255, 255); //warna rgb (putih)
    bulat(tx + 0, ty + 250); //tx posisi x awan; ty posisi y awan
    bulat(tx + 15, ty + 245);
    bulat(tx + 10, ty + 240);
    bulat(tx + -2, ty + 243);
}

//------------------------------------------ Gunung --------------------------------------------------
void gunung(float tx, float ty, float t, float l, float scale) { //membuat gunung
    //tx = titik x puncak gunung; ty = titik y puncak gunung; t = tinggi gunung; l = lebar gunung; scale = skala gunung
    glBegin(GL_TRIANGLES); //menggambar segitiga
    //menerapkan skala pada tinggi dan lebar
    t *= scale;
    l *= scale;
    //menghitung koordinat titik segitiga
    float xA = tx - l / 2, yA = ty - t; //kiri bawah
    float xB = tx, yB = ty; //puncak
    float xC = tx + l / 2, yC = ty - t; //kanan bawah

    //menambakan titik lalu menyambungkannya (menjadi garis)
    glVertex2f(xA, yA);
    glVertex2f(xB, yB);
    glVertex2f(xC, yC);

    glEnd(); //selesai menggambar gunung
}

//------------------------------------------ Danau --------------------------------------------------
void danau(float centerX, float centerY, float radiusX, float radiusY) { //membuat danau
    //centerX,centerY = titik pusat; radiusX,radiusY = jari-jari elips pada sumbu X dan Y
    glBegin(GL_POLYGON); //menggambar poligon (segi banyak/bebas)
    for (float angle = 0.0f; angle <= 2 * 3.14159f; angle += 0.1f) { //iterasi menggambar elips
        float x = centerX + radiusX * cos(angle); //koordinat x baru
        float y = centerY + radiusY * sin(angle); //koordinat y baru
        glVertex2f(x, y);//menambakan titik lalu menyambungkannya (menjadi garis)
    }
    glEnd(); //selesai gambar danau
}

//------------------------------------------ Display --------------------------------------------------
void display() { //mulai menggambar
    glClearColor(135.0 / 255.0, 206.0 / 255.0, 235.0 / 255.0, 1.0); //Mengatur warna latar belakang (rgba) (warna biru langit)
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // Clear buffer
    //glClear(mask) = Membersihkan atau mereset isi dari buffer tertentu, seperti buffer warna, buffer kedalaman (depth buffer), atau buffer stencil
    // dalam hal ini ada dua buffer dan digabungkan menjadi satu karena adanya operator bitwise OR (color buffer dan stencil buffer dibersihkan dalam satu pemanggilan)
    
    //GL_COLOR_BUFFER_BIT = Membersihkan color buffer (buffer warna), sehingga seluruh tampilan diganti dengan warna latar belakang yang telah ditentukan oleh glClearColor
    //GL_STENCIL_BUFFER_BIT = Membersihkan stencil buffer, yang digunakan untuk operasi seperti masking atau pengaturan area rendering tertentu

    // deklarasi warna batang pohon dan daun pohon
    float trunkColor[4] = { 140 / 255.0f, 69 / 255.0f, 18 / 255.0f, 1.0f }; // Warna batang coklat (rgba)
    float leafColor[4] = { 34 / 255.0f, 139 / 255.0f, 34 / 255.0f, 1.0f };  // Warna daun hijau (rgba)

    // Matahari
    glColor3ub(255, 215, 0); //warna rgb matahari
    matahari(0, 200); //memanggil fungsi  matahari dengan x = 0 dan y = 200

    // Awan
    glPushMatrix(); //Menyimpan (push) kondisi transformasi matriks saat ini ke dalam laisan yang bertumpuk (stack).
    glTranslatef(tx, 0, 0); //menerapkan transformasi translasi (pergeseran posisi) terhadap objek yang akan digambar
    awan(-450.0f, -20.0f); //memanggil fungsi awan dengan x sekian dan y sekian
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
    glPopMatrix(); //Mengembalikan kondisi transformasi matriks ke kondisi yang terakhir disimpan dengan glPushMatrix
    tx += 0.1; //tx mengatur pergeseran posisi x awan (animasi)
    if (tx > 250)
        tx = -250; //apabila sudah mencapai suatu posisi tertentu, kembali ke posisi awal

    // Gunung
    glColor3ub(170, 153, 111); //warna rgb gunung
    gunung(190.0f, 200.0f, 230.0f, 400.0f, 0.5f);
    //tx = titik x puncak gunung; ty = titik y puncak gunung; t = tinggi gunung; l = lebar gunung; scale = skala gunung

    glColor3ub(194, 178, 128);
    gunung(-50.0f, 200.0f, 230.0f, 400.0f, 0.5f);
    gunung(60.0f, 200.0f, 230.0f, 400.0f, 0.5f);

    glColor3ub(170, 153, 111);
    gunung(-200.0f, 200.0f, 230.0f, 400.0f, 0.5f);

    // Alas
    glColor3ub(210, 192, 145); //rgb tanah
    kotak(-250 * 2, 250 * 2, -250, 85); // Diperluas untuk mencakup seluruh jendela 
    //left = sisi kiri; right = sisi kanan; bottom = sisi bawah; top = sisi atas

    // Danau
    glColor3ub(118, 249, 249); //warna biru danau
    float danauCenterX = 0.0f, danauCenterY = -90.0f; //titik pusat
    float danauRadiusX = 300.0f, danauRadiusY = 100.0f; //jari-jari elips
    danau(danauCenterX, danauCenterY, danauRadiusX, danauRadiusY);

    // Set Stencil Buffer untuk Refleksi
    glEnable(GL_STENCIL_TEST); //mengaktifkan stencil test pada open gl yang memungkinkan kontrol area spesifik di layar tempat gambar yang dapat dirender
    glStencilFunc(GL_ALWAYS, 1, 0xFF); //GL_ALWAYS = test selalu berhasil; 1 = nilai referensi yang ditulis ke stencil buffer apabila tes berhasil;
    //0xFF = Mask yang menentukan bahwa semua bit dari referensi akan digunakan
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); // Jangan ubah nilai stencil saat tes gagal; Tulis ke stencil buffer jika tes berhasil
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    // Menonaktifkan penggambaran warna ke layar sehingga hanya stencil buffer yang diperbarui. Ini memastikan bentuk danau hanya memengaruhi stencil buffer.

    // Gambarkan bentuk danau ke stencil buffer
    glBegin(GL_POLYGON); //menggambar poligon (danau)
    for (float angle = 0.0f; angle <= 2 * 3.14159f; angle += 0.1f) {
        float x = danauCenterX + danauRadiusX * cos(angle);
        float y = danauCenterY + danauRadiusY * sin(angle);
        glVertex2f(x, y);
    }
    glEnd(); //selesai gambar danau

//=================================== REFLEKSI =================================================
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); // Mengaktifkan kembali penggambaran warna ke layar, setelah sebelumnya dinonaktifkan
    glStencilFunc(GL_EQUAL, 1, 0xFF); // GL_EQUAL = Tes berhasil jika nilai stencil saat ini sama dengan referensi (1).
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); // Jangan ubah nilai stencil buffer

    // Refleksi Awan
    glPushMatrix();
    glScalef(1.0f, -1.0f, 1.0f); //menerapkan transformasi skala (perubahan ukuran) terhadap objek yang akan digambar
    //(tidak ada perubahan pada sumbu x, membalikkan sumbu y untuk menciptakan efek pantulan, tidak ada perubahan pada sumbu z (tidak relevan di 2d))
    //oleh karena transformasi skala ini bisa digunakan untuk menciptakan refleksi
    glTranslatef(tx, 0, 0); // transformasi horizontal
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
    
    //tidak perlu refleksi matahari karena tidak masuk refleksi

    // Refleksi Tanah
    glPushMatrix();
    glScalef(1.0f, -1.0f, 1.0f); //transformasi skala refleksi trhdp sumbu x (balik vertikal)
    glTranslatef(0, 170.0f, 0); // Sesuaikan posisi y refleksi
    glColor4f(210 / 255.0f, 192 / 255.0f, 145 / 255.0f, 1.0f); //rgba
    kotak(-250 * 2, 250 * 2, -250, -85);
    glPopMatrix();

    //Tumbleweed (refleksi)
    glColor4f(105 / 255.0f, 92 / 255.0f, 59 / 255.0f, 1.0f); //(rgba)
    glPushMatrix(); 
    glScalef(1.0f, -1.0f, 1.0f); //transformasi skala refleksi trhdp sumbu x (balik vertikal)
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

    glDisable(GL_STENCIL_TEST); //matikan stencil buffer (crop danau untuk objek setelah kode ini dimatikan)
//=================================== ORIGINAL =================================================

    // Tumbleweed
    glColor4f(105 / 255.0f, 92 / 255.0f, 59 / 255.0f, 1.0f); //rgba
    glPushMatrix();
    glTranslatef(bx, by, 0); //posisi tumbleweed
    glRotatef(angle, 0.0f, 0.0f, 1.0f);
    tumbleweed(0.0f, 0.0f, 25.0f, 200, 1.5f); //(x,y,radius,jumlah jari-jari,ketebalan)
    glPopMatrix();

    // Pohon
    pohon(220, 30, 0.4f, trunkColor, leafColor);

    // Update posisi tumbleweed
    angle -= 0.5;
    bx += 0.5;
    jumpPhase += jumpSpeed; // Perbarui fase lompatan
    by = 42 + jumpAmplitude * fabs(sin(jumpPhase)); // Gerakan y dengan lompatan sinus

    if (bx > 600)
        bx = -570;
    if (angle <= -360.0f)
        angle += 360.0f;

    // Gerakan daun
    if (leafDirection)
        leafAngle += 0.5f; //kecepatan gerakan daun
    else
        leafAngle -= 0.5f; //kecepatan gerakan daun

    if (leafAngle > 15.0f)
        leafDirection = false;
    else if (leafAngle < -15.0f)
        leafDirection = true;

    glutPostRedisplay(); //Fungsi ini memberi tahu OpenGL Utility Toolkit(GLUT) bahwa window perlu diperbarui atau digambar ulang.
    glFlush(); //Fungsi ini memastikan bahwa semua perintah OpenGL yang berada dalam antrian command buffer akan segera dijalankan oleh GPU.
}

void init() { //mengatur parameter awal openGL
    glClearColor(135.0 / 255.0, 206.0 / 255.0, 235.0 / 255.0, 1.0); //waran latar belakang biru langit
    glOrtho(-250.0, 250.0, -250.0, 250.0, -1.0, 1.0); //Mengatur proyeksi ortografis
    glEnable(GL_STENCIL_TEST); // Aktifkan stencil buffer
    glEnable(GL_BLEND);        // Aktifkan blending untuk format warna rgba
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Aktifkan blending untuk format wrna rgba
}

int main(int argc, char** argv) { //gatur GLUT (OpenGL Utility Toolkit) dan memulai loop utama
    srand(time(0)); // Inisialisasi random untuk pengacak
    glutInit(&argc, argv); //Menginisialisasi GLUT dengan parameter dari baris perintah
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL); // mode tampilan
    //GLUT_SINGLE = buffer tunggal; GLUT_RGB = warna basis rgb; GLUT_DEPTH: Mengaktifkan depth buffer (meskipun tidak digunakan di grafik 2D).
    //GLUT_STENCIL = mengaktifkan buffer stencil
    glutInitWindowSize(1600, 900); //ukuran window saat dirun
    glutCreateWindow("KELOMPOK 20 'GURUN OASIS'");

    init(); //panggil fungsi init
    glutDisplayFunc(display); //mendaftarkan fungsi display untuk menjalankan program utama
    glutReshapeFunc(reshape); //mendaftarkan fungsi reshape untuk masalah resolusi
    glutMainLoop(); //memulai loop utama

    return 0; //akhir fungsi
}