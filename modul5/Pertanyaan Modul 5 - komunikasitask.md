# Jawaban Pertanyaan Praktikum Modul 5: Real-Time Operating System (Percobaan 5B)

## 1. Mekanisme Eksekusi Task: Bersamaan atau Bergantian?

**Jawaban:** Kedua *task* (`read_data` dan `display`) berjalan secara **bergantian (konkuren)**, bukan bersamaan secara fisik.

**Penjelasan Mekanisme:**
* Mikrokontroler (seperti Arduino Uno/Mega) hanya memiliki satu *core* prosesor, sehingga hanya bisa menjalankan satu instruksi pada satu waktu.
* FreeRTOS menggunakan penjadwal (*Scheduler*) untuk mengatur pergantian CPU secara cepat antar *task*.
* Pada program ini, eksekusi sangat bergantung pada mekanisme **Blocking** dari *Queue*.
  1. *Task* `display` akan menunggu (berada dalam state *Blocked*) di baris `xQueueReceive(...)` selama *queue* masih kosong (`portMAX_DELAY`).
  2. Saat *task* `read_data` mengisi *queue* menggunakan `xQueueSend(...)`, *task* `display` otomatis dibangunkan (*Unblocked*) dan berpindah ke state *Ready* atau *Running* untuk memproses dan mencetak data tersebut.
  3. Setelah selesai mengirim, `read_data` memanggil `vTaskDelay()`, yang memaksanya masuk ke state *Blocked* dan memberikan ruang bagi CPU untuk mengeksekusi `display`.

---

## 2. Potensi Terjadinya Race Condition

**Jawaban:** **TIDAK**, program ini **tidak berpotensi** mengalami *race condition*.

**Penjelasan:**
* *Race condition* biasanya terjadi ketika dua *task* atau lebih mencoba mengakses dan memodifikasi variabel/memori global (*shared resource*) yang sama secara bersamaan tanpa mekanisme pengaman.
* Pada program ini, data tidak dibagikan menggunakan variabel global, melainkan dikirim melalui **FreeRTOS Queue**.
* Fitur *Queue* di FreeRTOS secara bawaan sudah **thread-safe**. Di balik layar, *queue* menggunakan mekanisme *Critical Section* atau mematikan *interrupt* sementara saat proses baca/tulis (*send/receive*) berlangsung. Data struct `readings` dikirim menggunakan metode *pass-by-copy* (salinan), sehingga `read_data` dan `display` tidak pernah mengakses alamat memori yang persis sama pada waktu yang sama.

---

## 3. Modifikasi Menggunakan Sensor DHT

Berikut adalah modifikasi program untuk mengintegrasikan pembacaan sensor DHT fisik sehingga data yang dilempar ke *Queue* bersifat dinamis.

### Kebutuhan Tambahan
* Library: `DHT sensor library` oleh Adafruit (bisa diunduh via Library Manager).
* Perangkat Keras: Sensor DHT11 atau DHT22.

### Kode Program (main.cpp/sketch.ino)

```cpp
#include <Arduino_FreeRTOS.h>
#include <queue.h>
#include <DHT.h>

// Definisi pin dan tipe DHT
#define DHTPIN 2       // Pin data DHT terhubung ke pin digital 2
#define DHTTYPE DHT11  // Gunakan DHT22 jika menggunakan sensor DHT22

DHT dht(DHTPIN, DHTTYPE);

// Struktur data diubah menjadi float untuk menyesuaikan tipe data bacaan DHT
struct readings {
    float temp;
    float h;
};

QueueHandle_t my_queue;

void read_data(void *pvParameters);
void display(void *pvParameters);

void setup() {
    Serial.begin(9600);
    dht.begin();
    
    // Membuat Queue dengan kapasitas 1 item sebesar ukuran struct readings
    my_queue = xQueueCreate(1, sizeof(struct readings));

    if (my_queue != NULL) {
        // Membuat task untuk membaca sensor dan menampilkan data
        xTaskCreate(read_data, "Read_Sensor", 128, NULL, 1, NULL);
        xTaskCreate(display, "Display_Data", 128, NULL, 1, NULL);
        
        vTaskStartScheduler();
    }
}

void loop() {
    // Kosong. Scheduler RTOS mengambil alih kontrol eksekusi.
}

void read_data(void *pvParameters) {
    struct readings x;
    for (;;) {
        // Membaca kelembaban dan suhu
        float humidity = dht.readHumidity();
        float temperature = dht.readTemperature();

        // Validasi pembacaan (jika sensor gagal terbaca, kembalikan NaN)
        if (!isnan(humidity) && !isnan(temperature)) {
            x.temp = temperature;
            x.h = humidity;
            
            // Mengirim data struct ke dalam queue
            xQueueSend(my_queue, &x, portMAX_DELAY);
        } else {
            Serial.println("Gagal membaca dari sensor DHT!");
        }
        
        // Sensor DHT11 membutuhkan delay sekitar 2 detik antar pembacaan
        vTaskDelay(2000 / portTICK_PERIOD_MS); 
    }
}

void display(void *pvParameters) {
    struct readings x;
    for (;;) {
        // Menunggu data masuk ke dalam queue tanpa batas waktu (portMAX_DELAY)
        if (xQueueReceive(my_queue, &x, portMAX_DELAY) == pdPASS) {
            Serial.print("Suhu: ");
            Serial.print(x.temp);
            Serial.print(" *C\t | \t");
            Serial.print("Kelembaban: ");
            Serial.print(x.h);
            Serial.println(" %");
        }
    }
}
```

**Hasil dan Analisis Program**
1. Pembacaan Dinamis: Data statis (54 dan 30) digantikan oleh bacaan real-time lingkungan sekitar dari sensor DHT. Data suhu dan kelembaban disimpan dalam struct bertipe float.

2. Sinkronisasi Waktu yang Efisien: Pada fungsi read_data(), digunakan vTaskDelay(2000 / portTICK_PERIOD_MS) karena keterbatasan hardware sensor DHT11 yang hanya sanggup melakukan pengambilan sampel data baru paling cepat setiap 2 detik.

3. Penerimaan Otomatis: Task display tidak membutuhkan fungsi delay tambahan di dalam loop-nya. Task tersebut akan "tertidur" dan hanya memakan siklus CPU secara aktif tepat pada saat data dari queue dikirimkan oleh read_data. Ini merupakan praktik efisiensi komputasi yang sangat baik pada implementasi RTOS.
