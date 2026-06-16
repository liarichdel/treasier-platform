# treasIER v1.0.0

**treasIER** (*Trash Made Easier & Valuable as a Treasure*) adalah platform manajemen bank sampah cerdas berbasis desktop yang dirancang menggunakan bahasa **C++ murni** dan framework **Qt Widgets**. 

Aplikasi ini dikembangkan secara mandiri menggunakan **8 struktur data fundamental tanpa library kontainer STL (Standard Template Library)** demi optimasi alokasi memori dan kecepatan pemrosesan data yang ketat.

---

## Project Overview

**treasIER** hadir sebagai solusi digital untuk mengatasi inefisiensi logistik dan birokrasi manual pada ekosistem bank sampah konvensional. Proyek ini menjembatani kompleksitas teori *Computer Science* dengan kebutuhan nyata di lapangan melalui antarmuka GUI yang interaktif. 

Sistem ini mentransformasikan data sampah menjadi komoditas berharga dengan mengotomatisasi seluruh siklus operasional dalam satu dashboard terintegrasi:
* **Alur Operasional Warga:** Mengatur tertib fisik antrean warga di pos pelayanan hingga otomatisasi pencatatan nota transaksi digital yang instan.
* **Tata Kelola Inventaris:** Memantau kapasitas penampungan gudang penampungan balok sampah secara berjenjang.
* **Analisis Data & Logistik Wilayah:** Menyajikan statistik performa tonase pengumpulan tiap sektor wilayah secara dinamis serta mengalkulasi rute armada pengiriman logistik paling optimal ke pabrik daur ulang.

Dengan arsitektur *Non-STL Engine*, aplikasi ini memastikan manajemen pointer dan memori berjalan pada efisiensi tertinggi, menjadikannya model sistem perangkat lunak yang tangguh dan andal untuk komputasi berkelanjutan.

---

## Penjelasan Fitur & Runtime Algoritma

Setiap modul antarmuka pada GUI **treasIER** dirancang terhubung langsung dengan mesin algoritma spesifik di latar belakang yang aktif secara instan berdasarkan interaksi pengguna:

### 1. Modul Manajemen Antrean Warga Aktif
* **Fungsi Fitur di GUI:** Menampilkan daftar urutan warga yang sedang mengantre di lokasi fisik pos setoran untuk mencegah penumpukan pelayanan.
* **Eksekusi Algoritma (Queue FIFO):** Saat operator menginput data warga baru, sistem mengeksekusi fungsi `enqueue()` untuk menyisipkan node ke dalam antrean. Ketika operator mengklik tombol *"Proses Antrean Next"* pada antarmuka, fungsi `dequeue()` langsung memanggil warga terdepan secara adil berdasarkan prinsip First-In, First-Out (FIFO).

### 2. Form Pencatatan Transaksi Sampah
* **Fungsi Fitur di GUI:** Menyediakan kolom input jenis sampah dan berat setoran (kg) setelah warga dipanggil dari antrean.
* **Eksekusi Algoritma (BST Tree):** Tepat saat operator memasukkan angka berat dan memilih kategori sampah, fungsi rekursif `hitungPoin()` pada **Binary Search Tree** langsung aktif mengecek batas minimum kilogram dan mengonversi berat sampah menjadi poin saldo secara presisi di latar belakang.

### 3. Fitur Otentikasi & Lokap Profil Nasabah
* **Fungsi Fitur di GUI:** Menampilkan data profil, nama, dan akumulasi saldo milik nasabah secara otomatis setelah nomor identitas dimasukkan.
* **Eksekusi Algoritma (Hash Table):** Ketika operator mengetikkan nomor KTP warga, sistem menjalankan fungsi `cariNasabah()` menggunakan **Polynomial Rolling Hash Table**. String KTP diubah menjadi indeks array dalam waktu kilat $O(1)$. Jika terjadi *collision* (kesamaan indeks), sistem secara otomatis melakukan penelusuran *Chaining* lewat pointer Linked List internal.

### 4. Fitur Riwayat Transaksi Global
* **Fungsi Fitur di GUI:** Menyajikan tabel rekam jejak nota dan riwayat setoran yang pernah dilakukan oleh nasabah bersangkutan.
* **Eksekusi Algoritma (Linked List Prepend):** Di detik operator menekan tombol *"Selesai Transaksi"*, fungsi `tambahTransaksi()` langsung dipicu untuk membuat node nota baru dan melakukan operasi *prepend* (penyisipan di posisi paling depan `headRiwayat`). Hal ini memastikan tabel riwayat pada GUI otomatis tersusun secara kronologis dari yang paling terbaru.

### 5. Modul Kontainer Gudang (Simulasi Inventaris)
* **Fungsi Fitur di GUI:** Menampilkan visualisasi tumpukan komoditas sampah yang telah dipres menjadi balok padat untuk memantau sisa kapasitas ruang penampungan fisik.
* **Eksekusi Algoritma (Stack LIFO):** * **Saat Simpan (Push):** Ketika admin memasukkan data balok sampah baru ke gudang, fungsi `push()` dieksekusi membuat tumpukan visual pada komponen GUI naik tepat di posisi `top`.
  * **Saat Bongkar (Pop):** Ketika armada truk logistik datang dan admin mengklik tombol *"Kirim Barang"*, fungsi `pop()` langsung dijalankan untuk membongkar dan menghapus balok teratas dari memori sesuai prinsip Last-In, First-Out (LIFO).

### 6. Panel Analisis Wilayah Kritis
* **Fungsi Fitur di GUI:** Menyajikan data statistik performa pengumpulan limbah dari berbagai sektor wilayah operasional pos.
* **Eksekusi Algoritma (Merge Sort):** Ketika admin mengklik tombol *"Urutkan Volume"* pada tabel GUI, algoritma **Merge Sort** manual langsung bekerja membagi dan menyusun ulang baris tabel secara *descending* berdasarkan tonase volume terbesar dengan stabilitas performa tinggi $O(n \log n)$.

### 7. Komponen Pencarian Data (Searching Engine)
* **Fungsi Fitur di GUI:** Menyediakan kolom pencarian cepat untuk melacak berkas transaksi atau mencari data sektor wilayah tertentu.
* **Eksekusi Algoritma (Linear & Binary Search):** * **Linear Search:** Aktif berjalan melalui fungsi `cariResiLinear()` saat admin mengetik nomor resi acak untuk menelusuri rantai memori dari awal hingga akhir.
  * **Binary Search:** Aktif berjalan melalui fungsi `cariWilayahBinary()` berkecepatan $O(\log n)$ saat admin mencari ID Pos Wilayah tertentu karena data pada tabel sudah terurut.

### 8. Modul Navigasi & Peta Rute Distribusi
* **Fungsi Fitur di GUI:** Menampilkan kanvas peta interaktif berisi titik-titik lokasi pos penampungan (*Vertex*) dan jalur penghubung antar-jalan (*Edge*).
* **Eksekusi Algoritma (Weighted Graph & Dijkstra):** Ketika operator memilih lokasi pos asal, menentukan pabrik daur ulang tujuan, lalu menekan tombol *"Kalkulasi Rute Tercepat"*, **Algoritma Dijkstra berbasis MinHeap manual** langsung bekerja intensif menghitung jalur dengan akumulasi hambatan jarak terkecil. Hasil koordinat jalurnya langsung ditangkap dan digambar secara *real-time* oleh objek `QPainter` menjadi garis hijau menyala di atas peta visual layar.

---

## Teknologi yang Digunakan

* **Bahasa Pemrograman:** C++ 
* **Framework GUI:** Qt Widgets 
* **IDE Terkait:** Qt Creator
* **Sistem Build:** QMake

---

