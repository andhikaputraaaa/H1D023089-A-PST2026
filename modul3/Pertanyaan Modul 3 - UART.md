# Jawaban Pertanyaan Praktikum Modul 3: Protokol Komunikasi (Percobaan 3A)

## 1. Proses dari Input Keyboard hingga LED Menyala/Mati

Proses ini melibatkan beberapa tahapan komunikasi antara komputer dan mikrokontroler:

1. **Input Keyboard:** Pengguna mengetikkan karakter (misalnya '1' atau '0') di Serial Monitor (Arduino IDE) lalu menekan *Enter* atau klik *Send*.
2. **Transmisi PC ke Arduino (USB/UART):** Komputer mengubah karakter tersebut menjadi kode ASCII. Data ini kemudian dikirimkan melalui kabel USB ke IC konverter USB-to-Serial pada papan Arduino. 
3. **Penerimaan UART (Microcontroller):** IC konverter meneruskan data tersebut ke pin RX (Receive) pada mikrokontroler utama (ATmega328P) secara serial (bit demi bit).
4. **Masuk ke Serial Buffer:** Mikrokontroler menerima data tersebut dan menyimpannya sementara di dalam memori yang disebut *Serial Buffer* (penyangga data).
5. **Pembacaan oleh Program:** Fungsi `Serial.available()` mengecek apakah ada data di dalam buffer. Jika ada, fungsi `Serial.read()` mengambil satu byte data (karakter) dari buffer.
6. **Eksekusi Perintah:** Program menggunakan struktur kontrol `if-else` untuk mengevaluasi karakter yang dibaca. Jika karakter adalah `'1'`, mikrokontroler akan mengeksekusi `digitalWrite(PIN_LED, HIGH)` yang mengalirkan arus listrik ke pin 12, sehingga LED menyala.

---

## 2. Fungsi `Serial.available()`

Fungsi `Serial.available()` digunakan untuk memeriksa berapa banyak *byte* data yang saat ini antre di dalam *Serial Buffer* dan siap untuk dibaca.

**Apa yang terjadi jika baris tersebut dihilangkan?**
Jika `if(Serial.available() > 0)` dihilangkan, fungsi `Serial.read()` akan dieksekusi terus-menerus tanpa henti di dalam `void loop()`. Saat tidak ada data yang dikirim dari komputer (buffer kosong), `Serial.read()` akan mengembalikan nilai `-1`. Karena `-1` tidak sama dengan `'1'`, `'0'`, `'\n'`, atau `'\r'`, program akan terus-menerus masuk ke blok `else if` terakhir dan membanjiri layar Serial Monitor dengan teks **"Perintah tidak dikenal"** ribuan kali per detik.

---

## 3. Modifikasi Program (Fitur Blink dengan Input '2')

Untuk membuat LED terus berkedip saat menerima perintah '2' tanpa memblokir pembacaan perintah selanjutnya, kita menggunakan metode *non-blocking* dengan menyimpan "status/perintah saat ini" dalam sebuah variabel.

Berikut adalah kodenya beserta penjelasannya:

```cpp
#include <Arduino.h>

const int PIN_LED = 12;

// Variabel untuk menyimpan perintah terakhir yang aktif
// Default '0' agar saat Arduino menyala, LED dalam keadaan mati
char currentCommand = '0'; 

// Variabel untuk fitur blink menggunakan millis()
unsigned long previousMillis = 0; // Menyimpan waktu terakhir LED berubah state
const long interval = 500;        // Interval kedipan LED (500 ms)
bool ledState = LOW;              // Status LED saat ini (menyala/mati)

void setup() {
  Serial.begin(9600); // Inisialisasi komunikasi serial dengan baud rate 9600 bps
  Serial.println("Ketik '1' (ON), '0' (OFF), atau '2' (Blink)");
  pinMode(PIN_LED, OUTPUT); // Mengatur pin 12 sebagai output
}

void loop() {
  // 1. BLOK PEMBACAAN DATA SERIAL
  // Hanya dieksekusi JIKA ada data baru yang masuk
  if(Serial.available() > 0) {
    char incomingData = Serial.read(); // Membaca 1 karakter dari buffer
    
    // Memfilter data masukan. Hanya memproses jika input valid (0, 1, atau 2)
    if(incomingData == '0' || incomingData == '1' || incomingData == '2') {
      currentCommand = incomingData; // Perbarui status perintah saat ini
      
      // Jika perintah 0 atau 1, langsung eksekusi agar responsnya instan
      if(currentCommand == '0') {
        digitalWrite(PIN_LED, LOW);
        ledState = LOW; // Sinkronisasi variabel status
        Serial.println("LED OFF");
      } 
      else if(currentCommand == '1') {
        digitalWrite(PIN_LED, HIGH);
        ledState = HIGH;
        Serial.println("LED ON");
      }
      else if(currentCommand == '2') {
        Serial.println("LED BLINKING...");
      }
    } 
    // Menangkap input karakter lain yang bukan enter/newline
    else if(incomingData != '\n' && incomingData != '\r') {
      Serial.println("Perintah tidak dikenal");
    }
  }

  // 2. BLOK EKSEKUSI STATUS BLINK
  // Dieksekusi terus-menerus terlepas ada input baru atau tidak, selama status = '2'
  if(currentCommand == '2') {
    unsigned long currentMillis = millis(); // Ambil waktu sistem saat ini
    
    // Cek apakah selisih waktu sekarang dan waktu terakhir kedip sudah melebihi interval
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis; // Catat waktu kedip yang baru
      
      // Membalik (toggle) status LED
      if (ledState == LOW) {
        ledState = HIGH;
      } else {
        ledState = LOW;
      }
      
      // Terapkan status baru ke komponen hardware
      digitalWrite(PIN_LED, ledState);
    }
  }
}
```

## 4. Pemilihan Antara delay() vs millis()

Pada modifikasi program di atas, kita wajib menggunakan **millis()**.

Penjelasan dan Pengaruhnya terhadap Sistem:

- Jika menggunakan delay(): Fungsi ini bersifat blocking. Artinya, ketika Arduino sedang menjalankan delay(500) untuk membuat LED berkedip, seluruh proses mikrokontroler akan "berhenti/membeku" selama 500ms. Selama waktu tunggu tersebut, jika pengguna mengetikkan perintah '0' di Serial Monitor, Arduino akan lambat merespons (atau bahkan mengabaikan input) karena program tidak sedang berada di baris pengecekan Serial.available().
- Menggunakan millis(): Fungsi ini bersifat non-blocking. Arduino membaca waktu sistem saat ini secara terus-menerus tanpa menghentikan siklus loop(). Hal ini memungkinkan mikrokontroler mengecek masuknya data Serial baru dengan sangat cepat pada setiap putaran loop, sembari tetap menyalakan/mematikan LED secara akurat saat target waktu (interval) tercapai. Sistem menjadi multitasking secara semu dan sangat responsif terhadap input user.
