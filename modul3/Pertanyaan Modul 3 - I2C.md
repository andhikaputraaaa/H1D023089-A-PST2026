# Jawaban Pertanyaan Praktikum Modul 3: Protokol Komunikasi (Percobaan 3B)

## 1. Cara Kerja Komunikasi I2C antara Arduino dan LCD
Komunikasi I2C (Inter-Integrated Circuit) pada Arduino menggunakan prinsip **Master-Slave** melalui dua jalur kabel utama, yaitu **SDA (Serial Data)** dan **SCL (Serial Clock)**. 

Pada rangkaian ini:
* **Arduino bertindak sebagai Master:** Arduino memberikan sinyal *clock* (SCL di pin A5) untuk sinkronisasi waktu pengiriman bit, dan mengirimkan data karakter/perintah lewat jalur SDA (di pin A4).
* **LCD I2C bertindak sebagai Slave:** Modul I2C di belakang LCD mendengarkan jalur data. Ia memiliki alamat unik (biasanya `0x27` atau `0x3F`). Saat Arduino mengirimkan data dengan tujuan alamat `0x27`, hanya LCD tersebut yang akan memproses datanya (mengubah sinyal serial I2C menjadi perintah paralel untuk menyalakan piksel di layar), sementara sensor atau perangkat I2C lain (jika ada) akan mengabaikannya.

---

## 2. Pin Potensiometer Tertukar (Kiri dan Kanan)
Pin potensiometer pada dasarnya membagi tegangan listrik (*voltage divider*). Kaki tengah (wiper) adalah *output* yang masuk ke pin A0, sedangkan kaki kiri dan kanan dihubungkan ke VCC (5V) dan GND.

**Apa yang terjadi jika pin kiri dan kanan tertukar?**
Rangkaian **tetap aman dan tidak akan rusak**. Perubahannya hanya terjadi pada **arah putaran (logika nilainya berbalik)**. 
* Jika awalnya diputar ke kanan (searah jarum jam) nilai ADC membesar dari 0 ke 1023, maka jika pin VCC dan GND ditukar, putaran ke kanan akan membuat nilainya mengecil dari 1023 ke 0.

---

## 3. Modifikasi Program (Gabungan UART dan I2C Output)

Berikut adalah modifikasi program untuk menampilkan data tegangan dan persentase, baik di Serial Monitor maupun di LCD, beserta penjelasan per baris kodenya:

```cpp
#include <Wire.h>               // Library komunikasi I2C bawaan Arduino
#include <LiquidCrystal_I2C.h>  // Library khusus untuk mengontrol LCD via modul I2C

// Inisialisasi objek lcd dengan alamat I2C 0x27, untuk ukuran layar 16 kolom dan 2 baris
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Deklarasi pin A0 yang terhubung ke kaki tengah potensiometer
const int pinPot = A0;

void setup() {
  Serial.begin(9600);   // Memulai komunikasi serial (UART) ke komputer dengan baud rate 9600
  lcd.init();           // Melakukan inisialisasi awal pada LCD I2C
  lcd.backlight();      // Menyalakan lampu latar (backlight) LCD agar teks terlihat
}

void loop() {
  // Membaca nilai tegangan analog dari potensiometer (rentang 0 - 1023)
  int nilaiADC = analogRead(pinPot);

  // --- KALKULASI DATA ---
  // Mengubah nilai ADC (0-1023) menjadi tegangan Volt (0.00 - 5.00)
  float volt = nilaiADC * (5.0 / 1023.0);
  
  // Mengubah nilai ADC (0-1023) menjadi persentase (0 - 100%)
  int persen = map(nilaiADC, 0, 1023, 0, 100);
  
  // Memetakan nilai ADC untuk panjang blok bar di LCD (0 - 16 blok)
  int panjangBar = map(nilaiADC, 0, 1023, 0, 16);

  // --- OUTPUT KE SERIAL MONITOR (UART) ---
  // Mencetak format: ADC: [nilai] Volt: [nilai] V Persen: [nilai]%
  Serial.print("ADC: ");
  Serial.print(nilaiADC);
  Serial.print("\tVolt: ");
  Serial.print(volt, 2); // Mencetak nilai float dengan 2 angka di belakang koma
  Serial.print(" V\tPersen: ");
  Serial.print(persen);
  Serial.println("%");   // Pindah ke baris baru

  // --- OUTPUT KE LCD (I2C) ---
  // Baris 1: Menampilkan ADC dan Persen
  lcd.setCursor(0, 0);       // Menggeser kursor LCD ke kolom 0, baris 0 (atas kiri)
  lcd.print("ADC:");
  lcd.print(nilaiADC);
  lcd.print(" ");            // Spasi pemisah
  lcd.print(persen);
  lcd.print("%    ");        // Spasi kosong di akhir untuk membersihkan karakter sisa jika digit berkurang

  // Baris 2: Menampilkan Bar (Level)
  lcd.setCursor(0, 1);       // Menggeser kursor ke kolom 0, baris 1 (bawah kiri)
  for (int i = 0; i < 16; i++) { // Looping sebanyak 16 kolom
    if (i < panjangBar) {
      lcd.print((char)255);  // Menampilkan karakter kotak penuh (blok padat) jika i di bawah target
    } else {
      lcd.print(" ");        // Menampilkan spasi kosong jika blok belum terisi
    }
  }

  // Jeda 200 ms agar pembacaan dan tampilan layar tidak berkedip (flicker) terlalu cepat
  delay(200);
}
```

## 4. Tabel Pengamatan Hasil Konversi

Berdasarkan perhitungan konversi mikrokontroler 10-bit ADC ke skala 5V dan 100%, berikut adalah hasil pelengkap tabelnya:

| ADC | Volt (V) | Persen (%) |
| :---: | :---: | :---: |
| 1 | 0.00 | 0 |
| 21 | 0.10 | 2 |
| 49 | 0.24 | 5 |
| 74 | 0.36 | 7 |
| 96 | 0.47 | 9 |
