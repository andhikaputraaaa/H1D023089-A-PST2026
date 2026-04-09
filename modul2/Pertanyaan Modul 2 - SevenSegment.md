# Jawaban Pertanyaan Praktikum Modul 2: Pemrograman GPIO (Percobaan 2A)

## 1. Skematik Rangkaian yang Digunakan

<img width="500" alt="image" src="https://github.com/user-attachments/assets/350d3663-559c-4038-8cd1-364c48dda6c1" />

**Catatan:** Rangkaian ini menghubungkan pin digital Arduino Uno ke pin-pin penyusun Seven Segment sesuai dengan spesifikasi modul praktikum.

---

## 2. Apa yang terjadi jika nilai `num` lebih dari 15?
Jika nilai `num` yang dilewatkan ke fungsi `displayDigit(int num)` bernilai lebih dari 15, program akan mengalami **Array Out of Bounds** (mengakses indeks array di luar batas yang ditentukan. 

Array `digitPattern` dideklarasikan dengan ukuran 16 baris (indeks 0 hingga 15). Jika dipaksa mengakses indeks ke-16 atau lebih, mikrokontroler akan membaca data acak (*garbage memory*) yang ada di alamat memori setelah array tersebut. Akibatnya:
* Seven segment akan menampilkan pola nyala/mati LED yang acak atau tidak beraturan (glitch).
* Dalam beberapa kasus ekstrem pada pemrograman C/C++, hal ini bisa menyebabkan perilaku program yang tidak terprediksi (*undefined behavior*) hingga *crash* pada sistem mikrokontroler.

---

## 3. Jenis Konfigurasi Seven Segment (Common Cathode / Common Anode)
Program ini menggunakan konfigurasi **Common Cathode (Katoda Bersama). 

**Alasannya:**
Dapat dilihat pada array `digitPattern` dan fungsi `digitalWrite()`, program menggunakan logika `1` (atau `HIGH`) untuk **menyalakan** segmen LED. 
Contohnya pada representasi angka `0`: `{1,1,1,1,1,1,0,0}`. Segmen `a` sampai `f` diberi nilai `1` agar menyala, sedangkan segmen `g` (tengah) dan `dp` (titik) diberi nilai `0` agar mati.
Sebuah LED akan menyala jika diberi logika `HIGH` (sumber tegangan positif) hanya jika kutub negatifnya (katoda) terhubung bersama-sama ke Ground (GND). Oleh karena itu, ini adalah tipe Common Cathode.

---

## 4. Modifikasi Program (Tampilan berjalan dari F ke 0)

Untuk membuat tampilan berjalan mundur dari F ke 0, kita hanya perlu memodifikasi logika *looping* pada blok `void loop(). Berikut adalah kode program yang sudah dimodifikasi beserta penjelasan per barisnya:

```cpp
#include <Arduino.h>

// Deklarasi array konstan untuk menyimpan pemetaan pin Arduino ke segmen (a, b, c, d, e, f, g, dp)
const int segmentPins[8] = {7, 6, 5, 11, 10, 8, 9, 4};

// Array 2 dimensi berisi pola biner untuk membentuk karakter 0-9 dan A-F.
// Logika 1 = LED Menyala (Common Cathode), Logika 0 = LED Mati.
byte digitPattern[16][8] = {
    {1,1,1,1,1,1,0,0}, // Indeks 0: Menampilkan angka 0
    {0,1,1,0,0,0,0,0}, // Indeks 1: Menampilkan angka 1
    {1,1,0,1,1,0,1,0}, // Indeks 2: Menampilkan angka 2
    {1,1,1,1,0,0,1,0}, // Indeks 3: Menampilkan angka 3
    {0,1,1,0,0,1,1,0}, // Indeks 4: Menampilkan angka 4
    {1,0,1,1,0,1,1,0}, // Indeks 5: Menampilkan angka 5
    {1,0,1,1,1,1,1,0}, // Indeks 6: Menampilkan angka 6
    {1,1,1,0,0,0,0,0}, // Indeks 7: Menampilkan angka 7
    {1,1,1,1,1,1,1,0}, // Indeks 8: Menampilkan angka 8
    {1,1,1,1,0,1,1,0}, // Indeks 9: Menampilkan angka 9
    {1,1,1,0,1,1,1,0}, // Indeks 10: Menampilkan huruf A
    {0,0,1,1,1,1,1,0}, // Indeks 11: Menampilkan huruf b
    {1,0,0,1,1,1,0,0}, // Indeks 12: Menampilkan huruf C
    {0,1,1,1,1,0,1,0}, // Indeks 13: Menampilkan huruf d
    {1,0,0,1,1,1,1,0}, // Indeks 14: Menampilkan huruf E
    {1,0,0,0,1,1,1,0}  // Indeks 15: Menampilkan huruf F
};

// Fungsi kustom untuk menampilkan digit tertentu ke Seven Segment
void displayDigit(int num) 
{
    // Perulangan sebanyak 8 kali untuk menyetel masing-masing dari 8 segmen (termasuk dot/titik)
    for (int i = 0; i < 8; i++)
    {
        // Menuliskan sinyal digital (HIGH/LOW) ke pin sesuai pola yang ada di digitPattern
        digitalWrite(segmentPins[i], digitPattern[num][i]);
    }
}

// Fungsi inisialisasi yang dijalankan sekali saat Arduino pertama kali dihidupkan
void setup()
{
    // Perulangan untuk mengonfigurasi ke-8 pin GPIO Arduino yang digunakan
    for (int i = 0; i < 8; i++)
    {
        // Menetapkan setiap pin yang terhubung ke segmen sebagai pin OUTPUT
        pinMode(segmentPins[i], OUTPUT);
    }
}

// Fungsi utama yang akan dijalankan terus-menerus secara berulang
void loop()
{
    // MODIFIKASI: Perulangan for berjalan mundur.
    // Dimulai dari i = 15 (karakter F) dan terus menurun (i--) hingga i = 0 (angka 0)
    for (int i = 15; i >= 0; i--)
    {
        // Memanggil fungsi displayDigit untuk menampilkan pola berdasarkan nilai i saat ini
        displayDigit(i);
        
        // Memberikan jeda selama 1000 milidetik (1 detik) sebelum menampilkan karakter berikutnya
        delay(1000);
    }
}
