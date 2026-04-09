# Jawaban Pertanyaan Praktikum Modul 2: Pemrograman GPIO (Percobaan 2B)

## 1. Skematik Rangkaian yang Digunakan

<img width="850" alt="image" src="https://github.com/user-attachments/assets/cd880fb6-96fd-4963-9e6e-9d7d1f7257da" />

---

## 2. Mengapa menggunakan mode `INPUT_PULLUP`? Dan apa keuntungannya?

Mode `INPUT_PULLUP` digunakan untuk mengaktifkan resistor *pull-up* internal yang sudah ada di dalam mikrokontroler ATmega pada Arduino Uno. 

**Keuntungannya dibandingkan rangkaian biasa (tanpa pull-up internal):**
* **Mencegah status *Floating*:** Tanpa resistor *pull-up* atau *pull-down*, pin input yang tidak terhubung (tombol tidak ditekan) akan berada pada status *floating* (mengambang), sehingga pembacaan nilai digital bisa berubah-ubah secara acak antara `HIGH` dan `LOW` karena gangguan elektromagnetik sekitar.
* **Menghemat Komponen:** Kita tidak perlu memasang resistor eksternal di breadboard untuk setiap tombol.
* **Menyederhanakan Rangkaian:** Rangkaian kabel (*wiring*) menjadi lebih rapi dan sederhana. Salah satu kaki tombol cukup dihubungkan ke pin digital Arduino, dan kaki lainnya langsung ke GND.

---

## 3. Kemungkinan penyebab satu LED segmen tidak menyala

Jika hanya satu segmen LED pada Seven Segment yang tidak menyala, masalahnya bisa terjadi di sisi perangkat keras (Hardware) maupun perangkat lunak (Software):

**Dari sisi Hardware:**
* **Kabel jumper longgar atau putus:** Koneksi dari Arduino ke breadboard atau ke pin segmen tersebut tidak terhubung sempurna.
* **Resistor rusak/terlalu besar:** Resistor 220 Ohm yang terhubung ke segmen tersebut rusak atau nilainya tidak sesuai (terlalu besar sehingga arus tertahan).
* **Pin Seven Segment mati:** LED internal pada segmen spesifik tersebut memang sudah rusak/putus.
* **Pin Arduino bermasalah:** Port digital pada mikrokontroler yang mengontrol segmen tersebut mengalami kerusakan fisik.

**Dari sisi Software:**
* **Kesalahan pada pola bit (`digitPattern`):** Terdapat kesalahan penulisan angka `0` (mati) pada indeks array yang seharusnya `1` (menyala).
* **Kesalahan *Mapping* Pin:** Nilai pin pada array `segmentPins` salah ketik, sehingga sinyal dikirim ke pin yang tidak terhubung dengan benar.
* **Inisialisasi `pinMode` terlewat:** Pin untuk segmen tersebut tidak dideklarasikan sebagai `OUTPUT` pada fungsi `setup()`.

---

## 4. Modifikasi Rangkaian dan Program (Dua Push Button: Increment & Decrement)

Untuk memodifikasi program ini, kita menambahkan satu tombol lagi (misalnya di **Pin 2**) untuk fungsi *decrement* (pengurangan). Berikut adalah kode program lengkap beserta penjelasan di setiap barisnya:

```cpp
#include <Arduino.h> // Memasukkan library standar Arduino

// Mendeklarasikan array konstan untuk pin yang terhubung ke segmen a, b, c, d, e, f, g, dp
const int segmentPins[8] = {7, 6, 5, 11, 10, 8, 9, 4};

// Mendeklarasikan pin untuk push button
const int btnUp = 3;   // Pin 3 digunakan untuk tombol penambahan (Increment)
const int btnDown = 2; // Pin 2 digunakan untuk tombol pengurangan (Decrement)

// Array 2D berisi pola bit untuk menyalakan LED (1 = nyala, 0 = mati) membentuk angka 0-9 dan A-F
byte digitPattern[16][8] = {
    {1,1,1,1,1,1,0,0}, // 0
    {0,1,1,0,0,0,0,0}, // 1
    {1,1,0,1,1,0,1,0}, // 2
    {1,1,1,1,0,0,1,0}, // 3
    {0,1,1,0,0,1,1,0}, // 4
    {1,0,1,1,0,1,1,0}, // 5
    {1,0,1,1,1,1,1,0}, // 6
    {1,1,1,0,0,0,0,0}, // 7
    {1,1,1,1,1,1,1,0}, // 8
    {1,1,1,1,0,1,1,0}, // 9
    {1,1,1,0,1,1,1,0}, // A
    {0,0,1,1,1,1,1,0}, // b
    {1,0,0,1,1,1,0,0}, // C
    {0,1,1,1,1,0,1,0}, // d
    {1,0,0,1,1,1,1,0}, // E
    {1,0,0,0,1,1,1,0}  // F
};

// Variabel global untuk melacak status angka saat ini dan status tombol sebelumnya
int currentDigit = 0;           // Menyimpan nilai angka yang sedang ditampilkan (dimulai dari 0)
bool lastUpState = HIGH;        // Menyimpan status sebelumnya dari tombol btnUp (HIGH karena pull-up)
bool lastDownState = HIGH;      // Menyimpan status sebelumnya dari tombol btnDown (HIGH karena pull-up)

// Fungsi untuk menampilkan angka ke seven segment
void displayDigit(int num) 
{
    for (int i = 0; i < 8; i++) // Melakukan perulangan untuk ke-8 pin segmen
    {
        // Mengirimkan sinyal HIGH/LOW ke masing-masing pin sesuai pola di array digitPattern
        digitalWrite(segmentPins[i], digitPattern[num][i]); 
    }
}

// Fungsi inisialisasi yang dijalankan sekali
void setup()
{
    for (int i = 0; i < 8; i++) // Perulangan untuk mengatur ke-8 pin segmen
    {
        pinMode(segmentPins[i], OUTPUT); // Mengatur pin segmen sebagai OUTPUT
    }

    pinMode(btnUp, INPUT_PULLUP);   // Mengatur pin btnUp sebagai INPUT dengan resistor pull-up internal
    pinMode(btnDown, INPUT_PULLUP); // Mengatur pin btnDown sebagai INPUT dengan resistor pull-up internal
    
    displayDigit(currentDigit); // Menampilkan angka awal (0) ke layar saat Arduino baru menyala
}

// Fungsi utama yang berjalan berulang-ulang
void loop()
{
    // --- LOGIKA INCREMENT (TOMBOL UP) ---
    bool upState = digitalRead(btnUp); // Membaca status tombol btnUp saat ini
    
    // Mendeteksi transisi tombol dari tidak ditekan (HIGH) menjadi ditekan (LOW) -> Edge Detection
    if (lastUpState == HIGH && upState == LOW) 
    {
        currentDigit++; // Menambah nilai currentDigit sebanyak 1
        
        if (currentDigit > 15) // Jika nilai melewati 15 (karakter F)
        {
            currentDigit = 0; // Kembalikan nilai ke 0 (Siklus ulang)
        }
        
        displayDigit(currentDigit); // Tampilkan angka yang baru ke seven segment
        delay(50); // Jeda kecil untuk debouncing agar tidak terbaca ditekan berkali-kali secara tidak sengaja
    }
    lastUpState = upState; // Memperbarui status tombol btnUp sebelumnya dengan status saat ini

    // --- LOGIKA DECREMENT (TOMBOL DOWN) ---
    bool downState = digitalRead(btnDown); // Membaca status tombol btnDown saat ini
    
    // Mendeteksi transisi tombol dari tidak ditekan (HIGH) menjadi ditekan (LOW)
    if (lastDownState == HIGH && downState == LOW) 
    {
        currentDigit--; // Mengurangi nilai currentDigit sebanyak 1
        
        if (currentDigit < 0) // Jika nilai kurang dari 0
        {
            currentDigit = 15; // Kembalikan nilai ke batas maksimum yaitu 15 (karakter F)
        }
        
        displayDigit(currentDigit); // Tampilkan angka yang baru ke seven segment
        delay(50); // Jeda kecil untuk debouncing
    }
    lastDownState = downState; // Memperbarui status tombol btnDown sebelumnya dengan status saat ini
}
