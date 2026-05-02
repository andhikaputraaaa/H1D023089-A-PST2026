# Jawaban Pertanyaan Praktikum Modul 4: ADC dan PWM (Percobaan 4B)

## 1. Penjelasan analogWrite() dan Kecerahan LED
LED dapat diatur tingkat kecerahannya menggunakan fungsi `analogWrite()` karena fungsi ini menghasilkan sinyal **PWM (Pulse Width Modulation)** pada pin digital tertentu (pin PWM seperti 3, 5, 6, 9, 10, 11 pada Arduino Uno).

PWM bekerja dengan cara menyalakan dan mematikan LED secara cepat dalam satu periode waktu tertentu. Perbandingan lama kondisi HIGH (menyala) terhadap total periode disebut duty cycle.

* Duty cycle kecil → LED terlihat redup
* Duty cycle besar → LED terlihat terang

Walaupun LED sebenarnya berkedip sangat cepat, mata manusia melihatnya sebagai perubahan kecerahan, sehingga LED tampak seperti memiliki intensitas cahaya yang dapat diatur.

## 2. Hubungan antara Nilai ADC (0–1023) dan Nilai PWM (0–255)
Hubungan keduanya terletak pada perbedaan resolusi bit perangkat keras mikrokontroler:

* **ADC (Analog to Digital Converter)** pada Arduino Uno memiliki resolusi 10-bit, sehingga mampu menghasilkan $2^{10}$ atau 1024 variasi nilai diskrit (dari rentang 0 hingga 1023).
* **PWM (Pulse Width Modulation)** pada Arduino Uno memiliki resolusi 8-bit, sehingga hanya mampu menerima $2^8$ atau 256 variasi nilai (dari rentang 0 hingga 255).

Karena rentang input (ADC) lebih besar dari rentang output (PWM), maka diperlukan proses penskalaan (scaling). Nilai ADC bernilai 4 kali lipat lebih besar dari nilai PWM ($1024 \div 256 = 4$). Oleh karena itu, kita menggunakan fungsi map() untuk mengonversi rentang 0–1023 secara proporsional agar muat ke dalam rentang 0–255.

## 3. Modifikasi Program (Rentang PWM 50 hingga 200)

Untuk membatasi kecerahan LED pada rentang sedang, kita hanya perlu mengubah rentang parameter output pada fungsi map().

**Potongan Kode yang Dimodifikasi:**

```cpp
// ===================== PEMROSESAN DATA (SCALING) =====================
  // Ubah nilai ADC (0–1023) menjadi nilai PWM (50–200)
  pwm = map(nilaiADC,
            0,      // Nilai minimum ADC (Tetap)
            1023,   // Nilai maksimum ADC (Tetap)
            50,     // PWM minimum (Dimodifikasi dari 0)
            200);   // PWM maksimum (Dimodifikasi dari 255)
```
**Penjelasan Modifikasi Program**
Modifikasi dilakukan pada dua argumen terakhir dalam fungsi map(), yaitu batas bawah dan batas atas nilai output.

* Batas bawah diubah dari **0 menjadi 50**. Ini berarti ketika potensiometer berada pada posisi minimal (0V / ADC 0), LED tidak akan mati total, melainkan tetap menyala dengan tingkat PWM 50.
* Batas atas diubah dari **255 menjadi 200**. Ini berarti ketika potensiometer berada pada putaran maksimal (5V / ADC 1023), LED tidak akan menyala dengan kecerahan puncaknya, melainkan dibatasi hanya sampai tingkat PWM 200.
* Nilai ADC di antara 0 dan 1023 akan diskalakan secara linear ke dalam rentang PWM 50 hingga 200, sehingga memastikan LED hanya beroperasi pada tingkat "kecerahan sedang".
