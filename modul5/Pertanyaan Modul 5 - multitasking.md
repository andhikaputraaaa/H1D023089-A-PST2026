# Jawaban Pertanyaan Praktikum Modul 5: Real-Time Operating System (Percobaan 5A)

## 1. Mekanisme Eksekusi Task (Bersamaan atau Bergantian?)

**Jawaban:** Ketiga *task* berjalan secara **bergantian (konkuren)**, bukan bersamaan secara harfiah (paralel).

**Penjelasan Mekanisme:**
Mikrokontroler pada Arduino standar memiliki *single-core* (satu inti prosesor), sehingga secara fisik hanya mampu mengeksekusi satu instruksi pada satu waktu (siklus *clock*). FreeRTOS menciptakan ilusi *multitasking* agar program terlihat berjalan bersamaan melalui mekanisme **Context Switching** yang diatur oleh **Scheduler** (penjadwal).

*   Karena ketiga *task* (`TaskBlink1`, `TaskBlink2`, `Taskprint`) memiliki parameter prioritas yang sama (yaitu `1`), Scheduler akan menerapkan sistem **Round Robin**. CPU akan membagi porsi waktunya untuk masing-masing *task*.
*   Ketika sebuah *task* mengeksekusi perintah `vTaskDelay()`, *task* tersebut melepaskan kontrol CPU (berubah status dari *Running* menjadi *Blocked*). Scheduler secara otomatis akan langsung menyerahkan kontrol CPU kepada *task* lain yang sedang berstatus *Ready*.
*   Pergantian kontrol ini terjadi sangat cepat dalam hitungan *tick* (milidetik), sehingga secara visual proses berkedipnya kedua LED dan pencetakan di Serial Monitor terlihat seperti tereksekusi bersamaan.

---

## 2. Cara Menambahkan Task Keempat

Untuk menambahkan task keempat pada program FreeRTOS, ada tiga langkah utama yang harus dilakukan:

1.  **Deklarasi Prototipe Fungsi Task**
    Tambahkan deklarasi fungsi (sebelum fungsi `setup()`) agar kompilator mengenali keberadaan *task* tersebut.
    ```cpp
    void TaskKeempat( void *pvParameters );
    ```
2. **Registrasi/Pembuatan Task di dalam setup()**
   Gunakan fungsi xTaskCreate sebelum pemanggilan vTaskStartScheduler().
   ```cpp
   xTaskCreate(
   TaskKeempat,      // Pointer ke fungsi task
   "task4",          // Nama task (untuk tujuan debugging)
   128,              // Ukuran memori stack (dalam word)
   NULL,             // Parameter yang dikirim ke task
   1,                // Tingkat Prioritas task
   NULL              // Task Handle
   );
   ```
3. **Membuat Definisi Fungsi Task**
    Buat instruksi kerjanya di bagian bawah program. Sebuah fungsi task umumnya harus memiliki perulangan tak terbatas (`while(1)`) dan wajib memiliki fungsi *delay* bawaan RTOS (`vTaskDelay`) agar tidak memonopoli CPU secara permanen (menghindari *starvation* pada *task* lain).
    ```cpp
    void TaskKeempat(void *pvParameters) {
      while(1) {
        Serial.println("Task4 dieksekusi");
        vTaskDelay( 400 / portTICK_PERIOD_MS );
      }
    }
    ```

## 3. Modifikasi Program dengan Sensor Potensiometer
Program di bawah ini dimodifikasi dengan mengintegrasikan sebuah sensor analog (potensiometer) pada pin A0. Nilai yang dibaca dari potensiometer digunakan untuk memanipulasi rentang waktu vTaskDelay secara dinamis pada TaskBlink1, yang berfungsi sebagai pengontrol kecepatan kedipan LED.

```cpp
#include <Arduino_FreeRTOS.h>

void TaskBlink1( void *pvParameters );
void TaskBlink2( void *pvParameters );
void Taskprint( void *pvParameters );

// Mendefinisikan pin yang digunakan
const int ledPin1 = 8;
const int ledPin2 = 7;
const int potPin = A0; // Pin untuk potensiometer

void setup() {
  Serial.begin(9600);

  xTaskCreate(TaskBlink1, "task1", 128, NULL, 1, NULL);
  xTaskCreate(TaskBlink2, "task2", 128, NULL, 1, NULL);
  xTaskCreate(Taskprint, "task3", 128, NULL, 1, NULL);

  vTaskStartScheduler();
}

void loop() {
  // Loop bawaan Arduino dibiarkan kosong karena scheduler RTOS yang mengambil alih
}

void TaskBlink1(void *pvParameters) {
  pinMode(ledPin1, OUTPUT);
  while(1) {
    // Membaca nilai dari potensiometer (rentang ADC: 0 - 1023)
    int potValue = analogRead(potPin);
    
    // Konversi nilai ADC menjadi nilai delay (misal: rentang 50ms hingga 1000ms)
    int delayTime = map(potValue, 0, 1023, 50, 1000);

    Serial.print("Task1 Berjalan - Delay dinamis: ");
    Serial.print(delayTime);
    Serial.println(" ms");

    digitalWrite(ledPin1, HIGH);
    vTaskDelay( delayTime / portTICK_PERIOD_MS );
    digitalWrite(ledPin1, LOW);
    vTaskDelay( delayTime / portTICK_PERIOD_MS );
  }
}

void TaskBlink2(void *pvParameters) {
  pinMode(ledPin2, OUTPUT);
  while(1) {
    Serial.println("Task2");
    digitalWrite(ledPin2, HIGH);
    vTaskDelay( 300 / portTICK_PERIOD_MS );
    digitalWrite(ledPin2, LOW);
    vTaskDelay( 300 / portTICK_PERIOD_MS );
  }
}

void Taskprint(void *pvParameters) {
  int counter = 0;
  while(1) {
    counter++;
    Serial.print("Counter: ");
    Serial.println(counter);
    vTaskDelay(500 / portTICK_PERIOD_MS); 
  }
}
```

**Hasil dan Analisis Eksekusi**

1. **Kecepatan LED Dinamis:**
   Ketika program dijalankan, kecepatan kedipan LED 1 (Pin 8) dapat dikendalikan secara langsung (real-time). Memutar kenop potensiometer akan mengubah nilai pembacaan analog dari pin A0.
   * Apabila tuas potensiometer diputar menuju 0, LED 1 akan berkedip sangat cepat (dengan siklus delay 50ms).
   * Apabila tuas diputar penuh menuju 1023, LED 1 akan berkedip lambat (dengan siklus delay 1000ms atau 1 detik).
2. **Kemandirian Task (Isolasi antar proses):**
   Meskipun nilai delay pada TaskBlink1 berubah-ubah secara konstan berdasarkan input sensor, kecepatan TaskBlink2 (Pin 7) tidak akan terpengaruh sama sekali dan tetap stabil di delay 300ms. Begitu juga dengan Taskprint yang tetap menghitung counter setiap 500ms. Hal ini membuktikan bahwa FreeRTOS sukses menjalankan tugasnya untuk mengisolasi eksekusi dari masing-masing proses tanpa saling memblokir satu sama lain.
