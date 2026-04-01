# Percobaan 1A: Percabangan (if / if-else)

## Jawaban Pertanyaan Praktikum

### 1. Pada kondisi apa program masuk ke blok `if`?

Program masuk ke blok `if` ketika nilai variabel:

```cpp
timeDelay <= 100
```

Artinya, ketika delay sudah mencapai **100 ms atau kurang**, maka program menganggap LED sudah berkedip sangat cepat dan akan menjalankan proses **jeda 3 detik** lalu **reset delay kembali ke 1000 ms**.

---

### 2. Pada kondisi apa program masuk ke blok `else`?

Program masuk ke blok `else` ketika nilai if tidak terpenuhi, yaitu saat:

```cpp
  timeDelay > 100;
```

Pada kondisi ini, delay masih lebih besar dari 100 ms sehingga program akan terus mempercepat kedipan LED secara bertahap dengan cara:

```cpp
  timeDelay -= 100;
```

---

### 3. Apa fungsi dari perintah `delay(timeDelay)`?

Perintah `delay(timeDelay)` berfungsi untuk **memberi jeda waktu (dalam milidetik)** sebelum program melanjutkan ke instruksi berikutnya.

Pada program ini, `delay(timeDelay)` menentukan **seberapa lama LED menyala dan mati**, sehingga semakin kecil nilai `timeDelay`, maka kedipan LED akan semakin cepat.

---

### 4. Modifikasi Program: cepat → sedang → mati (tanpa reset langsung)

Sebelumnya program memiliki alur:
**mati → lambat → cepat → reset (mati)**

Diubah menjadi:
**mati → lambat → cepat → sedang → mati**

Artinya LED tidak langsung reset ke awal, tetapi setelah cepat akan melambat kembali hingga akhirnya berhenti (mati).

#### Kode Program Modifikasi

```cpp
const int ledPin = 12;        // Menentukan pin LED pada pin digital 12
int timeDelay = 1000;         // Variabel delay awal (1000 ms = 1 detik)
bool slowingDown = false;     // Penanda apakah LED sedang dalam fase melambat kembali

void setup() {
  pinMode(ledPin, OUTPUT);    // Mengatur pin LED sebagai output
}

void loop() {
  digitalWrite(ledPin, HIGH); // Menyalakan LED
  delay(timeDelay);           // Menunggu sesuai nilai timeDelay

  digitalWrite(ledPin, LOW);  // Mematikan LED
  delay(timeDelay);           // Menunggu sesuai nilai timeDelay

  // Jika belum memasuki fase melambat
  if (slowingDown == false) {

    // Jika delay sudah mencapai cepat (100 ms)
    if (timeDelay <= 100) {
      slowingDown = true;     // Ubah mode menjadi melambat kembali
    } 
    else {
      timeDelay -= 100;       // Mempercepat kedipan (delay diperkecil bertahap)
    }
  }

  // Jika sudah memasuki fase melambat
  else {

    // Jika delay sudah mencapai 1000 ms (sedang/lambat kembali)
    if (timeDelay >= 1000) {
      digitalWrite(ledPin, LOW); // Pastikan LED mati
      while (true);             // Menghentikan program agar LED tetap mati
    }
    else {
      timeDelay += 100;         // Memperlambat kedipan (delay ditambah bertahap)
    }
  }
}
```

#### Penjelasan Cara Kerja Program Modifikasi

1. Program mulai dari `timeDelay = 1000 ms` sehingga LED berkedip lambat.
2. Setiap siklus kedip, nilai `timeDelay` dikurangi 100 sehingga kedipan semakin cepat.
3. Saat `timeDelay` mencapai 100 ms, variabel `slowingDown` diubah menjadi `true`.
4. Ketika `slowingDown = true`, program mulai menaikkan `timeDelay` sehingga LED kembali melambat.
5. Jika `timeDelay` mencapai 1000 ms, LED dimatikan dan program dihentikan dengan `while(true)` sehingga LED tetap mati.
