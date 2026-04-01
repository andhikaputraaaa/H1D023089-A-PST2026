# Percobaan 2A: Perulangan (for)

## Jawaban Pertanyaan Praktikum

### 1. Gambar Skematik Rangkaian 6 LED Running

Rangkaian 5 LED running menggunakan 5 pin digital Arduino sebagai output.
Setiap LED disusun dengan resistor (biasanya 220Ω atau 330Ω) untuk membatasi arus.

```cpp
Pin 2  ----[220Ω]----|>|---- GND   (LED 1)
Pin 3  ----[220Ω]----|>|---- GND   (LED 2)
Pin 4  ----[220Ω]----|>|---- GND   (LED 3)
Pin 5  ----[220Ω]----|>|---- GND   (LED 4)
Pin 6  ----[220Ω]----|>|---- GND   (LED 5)

Keterangan:
[220Ω] = Resistor pembatas arus
|>|    = LED (Anoda di sisi resistor, katoda ke GND)
GND    = Ground Arduino
```
**Penjelasan Rangkaian**
* Anoda LED terhubung ke pin Arduino melalui resistor.
* Katoda LED terhubung ke GND.
* Resistor digunakan agar LED tidak rusak karena arus berlebih.

---

### 2. Bagaimana program membuat efek LED berjalan dari kiri ke kanan?

Efek LED berjalan dari kiri ke kanan dibuat oleh perulangan berikut:

```cpp
for (int ledPin = 2; ledPin < 5; ledPin++)
```

Penjelasan:
* Variabel `ledPin` dimulai dari pin 2.
* Selama `ledPin < 5`, nilai pin akan terus bertambah (`ledPin++`).
* Artinya LED dinyalakan berurutan dari pin 2 → pin 3 → pin 4.

Di dalam loop:
* LED dinyalakan dengan `digitalWrite(ledPin, HIGH)`
* LED dibiarkan menyala selama `delay(timer)`
* Setelah itu LED dimatikan dengan `digitalWrite(ledPin, LOW)`

Hasilnya, LED terlihat menyala satu per satu dari kiri ke kanan.

---

### 3. Bagaimana program membuat LED kembali dari kanan ke kiri?

Efek LED kembali dari kanan ke kiri dibuat oleh perulangan berikut:

```cpp
for (int ledPin = 4; ledPin >= 2; ledPin--)
```

Penjelasan:
* Variabel `ledPin` dimulai dari pin 4.
* Selama `ledPin >= 2`, nilai pin akan terus berkurang (`ledPin++`).
* Artinya LED dinyalakan berurutan dari pin 4 → pin 3 → pin 2.

Isi loop sama seperti sebelumnya:
* LED dinyalakan 
* delay
* LED dimatikan

Hasilnya LED terlihat bergerak kembali dari kanan ke kiri.

---

### 4. Program: LED Menyala Tiga Kanan dan Tiga Kiri Bergantian

#### Tujuan Program
Membuat pola nyala LED seperti berikut secara berulang:

* 3 LED kiri menyala, 3 LED kanan mati
* lalu 3 LED kanan menyala, 3 LED kiri mati

Misalkan digunakan 6 LED dengan konfigurasi pin:

* LED kiri: pin 2, 3, 4
* LED kanan: pin 5, 6, 7


#### Kode Program

```cpp
int timer = 500;                 // Menentukan waktu delay (ms) untuk pergantian pola LED

void setup() {
  // Mengatur pin 2 sampai pin 7 sebagai output (total 6 LED)
  for (int ledPin = 2; ledPin <= 7; ledPin++) {
    pinMode(ledPin, OUTPUT);     // Menjadikan setiap pin LED sebagai output
  }
}

void loop() {

  // ===== Pola 1: Tiga LED kiri menyala (pin 2,3,4) =====
  for (int ledPin = 2; ledPin <= 4; ledPin++) {
    digitalWrite(ledPin, HIGH);  // Menyalakan LED kiri
  }

  for (int ledPin = 5; ledPin <= 7; ledPin++) {
    digitalWrite(ledPin, LOW);   // Mematikan LED kanan
  }

  delay(timer);                  // Menunggu agar pola terlihat jelas


  // ===== Pola 2: Tiga LED kanan menyala (pin 5,6,7) =====
  for (int ledPin = 2; ledPin <= 4; ledPin++) {
    digitalWrite(ledPin, LOW);   // Mematikan LED kiri
  }

  for (int ledPin = 5; ledPin <= 7; ledPin++) {
    digitalWrite(ledPin, HIGH);  // Menyalakan LED kanan
  }

  delay(timer);                  // Menunggu agar pola terlihat jelas
}
```

#### Penjelasan Cara Kerja Program

1. Program menggunakan for loop pada setup() untuk menginisialisasi pin 2 sampai 7 sebagai output.
2. Pada bagian `loop()`, program membuat 2 pola utama:
    * Pola pertama menyalakan LED kiri (pin 2–4) dan mematikan LED kanan (pin 5–7).
    * Pola kedua mematikan LED kiri dan menyalakan LED kanan.
3. `delay(timer)` digunakan agar perpindahan pola terlihat jelas oleh mata.
4. Karena berada di dalam `loop()`, pola ini akan terus berulang tanpa henti.
