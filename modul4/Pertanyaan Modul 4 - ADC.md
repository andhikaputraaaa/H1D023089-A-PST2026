# Jawaban Pertanyaan Praktikum Modul 4: ADC dan PWM (Percobaan 4A)

## 1. Fungsi perintah analogRead() pada rangkaian
Fungsi analogRead() digunakan untuk membaca nilai tegangan analog yang masuk ke pin analog Arduino (pada program ini pin A0) dari potensiometer. Nilai yang dibaca akan dikonversi oleh ADC (Analog to Digital Converter) menjadi data digital dengan rentang 0 sampai 1023. 

Pada rangkaian ini:
* **0** berarti tegangan mendekati 0V
* **1023** berarti tegangan mendekati 5V (atau tegangan referensi Arduino)

Nilai hasil pembacaan ini kemudian digunakan sebagai input untuk menentukan sudut servo.

## 2. Alasan Penggunaan Fungsi map()
Fungsi map() diperlukan untuk mengubah rentang nilai ADC dari potensiometer menjadi rentang sudut servo yang sesuai.

* ADC menghasilkan nilai 0–1023
* Servo bergerak pada sudut 0–180 derajat

Jika nilai ADC langsung digunakan untuk servo, maka hasilnya tidak sesuai karena servo tidak menerima input dalam skala 0–1023. Oleh karena itu, map() digunakan untuk mengkonversi nilai ADC menjadi sudut servo yang tepat agar servo bergerak proporsional sesuai putaran potensiometer.

## 3. Modifikasi Program (Rentang Servo 30° hingga 150°)

Untuk membatasi pergerakan sudut motor servo agar hanya berputar antara 30° hingga 150° tanpa mengubah input potensiometer, kita hanya perlu memodifikasi parameter output pada fungsi map().

**Potongan Kode yang Dimodifikasi:**

```cpp
// ===================== KONVERSI DATA =====================
  // Ubah nilai ADC menjadi sudut servo (30–150 derajat)
  pos = map(val, 
             0,      // Nilai minimum ADC (Tetap)
             1023,   // Nilai maksimum ADC (Tetap)
             30,     // Sudut minimum servo (Dimodifikasi dari 0)
             150);   // Sudut maksimum servo (Dimodifikasi dari 180)
```
**Penjelasan Modifikasi Program**
Fungsi map() bekerja dengan sintaks map(value, fromLow, fromHigh, toLow, toHigh). Pada modifikasi ini, parameter rentang input (fromLow dan fromHigh) dipertahankan pada 0 dan 1023 karena potensiometer tetap beroperasi penuh pada rentang tegangan standarnya (hingga 5V).

Perubahan dilakukan pada parameter rentang output (toLow dan toHigh), yang diubah menjadi 30 dan 150. Hasil dari modifikasi ini adalah:

* Saat ADC membaca nilai minimum 0, fungsi akan menghasilkan sudut 30°.
* Saat ADC membaca nilai maksimum 1023, fungsi akan menghasilkan sudut 150°.
* Nilai di antaranya akan diskalakan secara linear di antara 30° hingga 150°.
