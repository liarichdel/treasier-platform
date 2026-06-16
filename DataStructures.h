#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include <QString>
#include <QDateTime>

//  KONSTANTA
static const int HASH_SIZE   = 101;
static const int MAKS_NODE   = 64;
static const double INF_DIST = 1e18;

//  ENUM
enum JenisSampah {
    PLASTIK = 0, KERTAS, LOGAM, KACA,
    ORGANIK, ELEKTRONIK, TEKSTIL, JENIS_COUNT
};

enum StatusPengiriman {
    MENUNGGU = 0, DALAM_PERJALANAN, TIBA_DI_PABRIK, SELESAI
};

//  HELPER
inline QString namaJenisSampah(JenisSampah j) {
    switch(j) {
    case PLASTIK:    return "Plastik";
    case KERTAS:     return "Kertas";
    case LOGAM:      return "Logam";
    case KACA:       return "Kaca";
    case ORGANIK:    return "Organik";
    case ELEKTRONIK: return "Elektronik";
    case TEKSTIL:    return "Tekstil";
    default:         return "Unknown";
    }
}

inline QString namaStatus(StatusPengiriman s) {
    switch(s) {
    case MENUNGGU:         return "Menunggu";
    case DALAM_PERJALANAN: return "Dalam Perjalanan";
    case TIBA_DI_PABRIK:   return "Tiba di Pabrik";
    case SELESAI:          return "Selesai";
    default:               return "Unknown";
    }
}

//  NODE: TRANSAKSI (Linked List riwayat)
struct TransaksiNode {
    QString       kodeResi;
    JenisSampah   jenis;
    double        beratKg;
    int           poinMutasi;
    QDateTime     waktu;
    TransaksiNode* next;

    TransaksiNode()
        : jenis(PLASTIK), beratKg(0.0), poinMutasi(0), next(nullptr) {}
};

//  NODE: NASABAH (elemen Hash Table bucket)
struct NasabahNode {
    QString        noKTP;
    QString        nama;
    QString        alamat;
    QString        noTelp;
    int            totalPoin;
    TransaksiNode* headRiwayat;   // linked list transaksi
    NasabahNode*   nextChain;     // chaining collision

    NasabahNode()
        : totalPoin(0), headRiwayat(nullptr), nextChain(nullptr) {}
};

//  NODE: DATA SETORAN (payload Queue)
struct DataSetoran {
    QString     noAntrian;
    QString     noKTPWarga;
    JenisSampah jenis;
    double      beratKg;
    QDateTime   waktuTiba;

    DataSetoran() : jenis(PLASTIK), beratKg(0.0) {}
};

struct AntreanNode {
    DataSetoran  data;
    AntreanNode* next;
    AntreanNode() : next(nullptr) {}
};

//  NODE: BST HARGA
struct HargaNode {
    JenisSampah jenis;
    double      poinPerKg;
    double      batasMinKg;
    HargaNode*  left;
    HargaNode*  right;

    HargaNode() : jenis(PLASTIK), poinPerKg(0), batasMinKg(0),
        left(nullptr), right(nullptr) {}
};

//  NODE: BALOK SAMPAH (payload Stack)
struct BalokSampah {
    QString     idBalok;
    JenisSampah jenis;
    double      beratKg;
    QDateTime   waktuPres;
    QString     noKTPAsal;

    BalokSampah() : jenis(PLASTIK), beratKg(0.0) {}
};

struct StackNode {
    BalokSampah data;
    StackNode*  next;
    StackNode() : next(nullptr) {}
};

//  DATA WILAYAH (untuk Sorting)
struct DataWilayah {
    QString idPos;
    QString namaKecamatan;
    double  totalVolumeTon;
    int     jumlahTransaksi;
    bool    statusKritis;

    DataWilayah()
        : totalVolumeTon(0.0), jumlahTransaksi(0), statusKritis(false) {}
};

//  GRAPH
struct EdgeNode {
    int       idTetangga;
    double    bobot;
    EdgeNode* next;
    EdgeNode() : idTetangga(-1), bobot(0.0), next(nullptr) {}
};

struct LokasiNode {
    int     id;
    QString namaLokasi;
    double  latitude;
    double  longitude;
    bool    isPabrikUtama;
    LokasiNode() : id(-1), latitude(0), longitude(0), isPabrikUtama(false) {}
};

struct HasilDijkstra {
    double jarak;
    int    predecessor;
    HasilDijkstra() : jarak(INF_DIST), predecessor(-1) {}
};

//  DATA PENGIRIMAN
struct DataPengiriman {
    QString          kodeResi;
    QString          idPosAsal;
    int              nodeAsal;
    int              nodeTujuan;
    double           totalBeratKg;
    int              jumlahBalok;
    StatusPengiriman status;
    QDateTime        waktuBerangkat;
    double           jarakOptimalKm;
    int*             jalurOptimal;
    int              panjangJalur;

    DataPengiriman()
        : nodeAsal(-1), nodeTujuan(-1), totalBeratKg(0.0),
        jumlahBalok(0), status(MENUNGGU), jarakOptimalKm(0.0),
        jalurOptimal(nullptr), panjangJalur(0) {}

    ~DataPengiriman() { delete[] jalurOptimal; }
};

struct PengirimanNode {
    DataPengiriman* data;
    PengirimanNode* next;
    PengirimanNode() : data(nullptr), next(nullptr) {}
};

#endif // DATASTRUCTURES_H
