#ifndef BANKSAMPAHCONTROLLER_H
#define BANKSAMPAHCONTROLLER_H

#include "Queue.h"
#include "Tree.h"
#include "HashTable.h"
#include "Stack.h"
#include "Sorting.h"
#include "Searching.h"
#include "Graph.h"

class BankSampahController {
private:
    Queue              antrean;
    PohonKlasifikasi   pohonHarga;
    HashTableNasabah   dbNasabah;
    StackGudang        gudang;
    ArrayWilayah       dataWilayah;
    SortingWilayah     mesinSorting;
    SearchingEngine    mesinCari;
    GrafJaringan       peta;

    PengirimanNode*    headPengiriman;
    int                totalPengiriman;
    int                counterResi;
    QString            nasabahAktif;   // nomor KTP yang sedang login

    QString generateResi();
    void    simpanPengiriman(DataPengiriman* p);

public:
    BankSampahController();
    ~BankSampahController();

    void inisialisasi();

    // Queue
    QString  wargaTiba(const QString& noKTP, JenisSampah j, double berat);
    bool     panggilBerikutnya(DataSetoran& out);
    Queue&   getAntrean()  { return antrean; }

    // Proses Setoran (queue→tree→hashTable→linkedList→stack)
    QString  prosesSetoran(const DataSetoran& s);

    // Nasabah
    bool         daftarNasabah(const QString& ktp, const QString& nama,
                       const QString& alamat, const QString& telp);
    NasabahNode* loginNasabah(const QString& ktp);
    void         logout()  { nasabahAktif.clear(); }
    QString      getNasabahAktif() const { return nasabahAktif; }
    NasabahNode* getNasabahPtr(const QString& ktp) const;
    HashTableNasabah& getDB() { return dbNasabah; }

    // Tree
    PohonKlasifikasi& getPohon() { return pohonHarga; }

    // Stack / Gudang
    StackGudang& getGudang() { return gudang; }

    // Wilayah / Sorting
    void tambahWilayah(const DataWilayah& w);
    void updateVolumeWilayah(const QString& idPos, double tambahTon);
    void sortirWilayah();
    ArrayWilayah& getWilayah() { return dataWilayah; }

    // Searching
    TransaksiNode* cariResi(const QString& ktp, const QString& resi) const;
    NasabahNode*   cariNasabahNama(const QString& nama) const;
    DataWilayah*   cariWilayah(const QString& idPos);

    // Pengiriman / Graph
    int  tambahLokasi(const QString& n, double lat, double lon, bool pabrik=false);
    void tambahRuas(int a, int b, double km);
    DataPengiriman* buatPengiriman(const QString& idPos, int nodeAsal);
    bool            updateStatusPengiriman(const QString& resi, StatusPengiriman st);
    PengirimanNode* getHeadPengiriman() const { return headPengiriman; }
    GrafJaringan&   getPeta() { return peta; }

    // Statistik
    int getTotalNasabah()    const { return dbNasabah.getJumlah(); }
    int getTotalPengiriman() const { return totalPengiriman; }
    int getAntreanSize()     const { return antrean.getUkuran(); }
    int getGudangSize()      const { return gudang.getUkuran(); }
};

#endif // BANKSAMPAHCONTROLLER_H
