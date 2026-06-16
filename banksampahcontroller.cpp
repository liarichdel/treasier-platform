#include "BankSampahController.h"

BankSampahController::BankSampahController()
    : headPengiriman(nullptr), totalPengiriman(0), counterResi(0) {}

BankSampahController::~BankSampahController() {
    PengirimanNode* curr = headPengiriman;
    while (curr) {
        PengirimanNode* tmp = curr;
        curr = curr->next;
        delete tmp->data;
        delete tmp;
    }
}

void BankSampahController::inisialisasi() {
    //Tree: Harga poin per kg tiap jenis sampah
    pohonHarga.tambahKategori(PLASTIK,    150.0, 0.5);
    pohonHarga.tambahKategori(KERTAS,      80.0, 1.0);
    pohonHarga.tambahKategori(LOGAM,      400.0, 0.2);
    pohonHarga.tambahKategori(KACA,        60.0, 1.0);
    pohonHarga.tambahKategori(ORGANIK,     20.0, 2.0);
    pohonHarga.tambahKategori(ELEKTRONIK, 750.0, 0.1);
    pohonHarga.tambahKategori(TEKSTIL,     50.0, 0.5);

    //Hash Table: Nasabah seed
    daftarNasabah("3578011001010001", "Soobin Santoso",     "Jl. Gubeng No.1",    "081234567890");
    daftarNasabah("3578011001010002", "Yeonjun Rahayu",      "Jl. Wonokromo No.5", "081234567891");
    daftarNasabah("3578011001010003", "Beomgyu Fauzi",      "Jl. Rungkut No.12",  "081234567892");
    daftarNasabah("3578011001010004", "Karina Lestari",     "Jl. Kenjeran No.3",  "081234567893");
    daftarNasabah("3578011001010005", "Renjun Pratama",    "Jl. Mulyosari No.7", "081234567894");
    daftarNasabah("3578011001010006", "Carmen Wati",        "Jl. Manyar No.22",   "081234567895");
    daftarNasabah("3578011001010007", "Taehyun Gunawan",   "Jl. Dukuh No.9",     "081234567896");
    daftarNasabah("3578011001010008", "Yuna Handayani",  "Jl. Bratang No.14",  "081234567897");
    daftarNasabah("3578011001010009", "Heeseung Nugroho",    "Jl. Nginden No.6",   "081234567898");
    daftarNasabah("3578011001010010", "Lisa Susanti",    "Jl. Semolowaru No.2","081234567899");

    //tambah beberapa transaksi seed untuk Soobin
    {
        TransaksiNode tx;
        tx.kodeResi   = "RES-SEED-0001";
        tx.jenis      = PLASTIK;
        tx.beratKg    = 3.5;
        tx.poinMutasi = pohonHarga.hitungPoin(PLASTIK, 3.5);
        tx.waktu      = QDateTime::currentDateTime().addDays(-5);
        dbNasabah.tambahTransaksi("3578011001010001", tx);
        dbNasabah.updatePoin("3578011001010001", tx.poinMutasi);
    }
    {
        TransaksiNode tx;
        tx.kodeResi   = "RES-SEED-0002";
        tx.jenis      = KERTAS;
        tx.beratKg    = 5.0;
        tx.poinMutasi = pohonHarga.hitungPoin(KERTAS, 5.0);
        tx.waktu      = QDateTime::currentDateTime().addDays(-3);
        dbNasabah.tambahTransaksi("3578011001010001", tx);
        dbNasabah.updatePoin("3578011001010001", tx.poinMutasi);
    }

    // Wilayah seed
    DataWilayah w1; w1.idPos="POS-GBG"; w1.namaKecamatan="Kec. Gubeng";
    w1.totalVolumeTon=13.2; w1.jumlahTransaksi=210; tambahWilayah(w1);
    DataWilayah w2; w2.idPos="POS-WNK"; w2.namaKecamatan="Kec. Wonokromo";
    w2.totalVolumeTon=9.8;  w2.jumlahTransaksi=165; tambahWilayah(w2);
    DataWilayah w3; w3.idPos="POS-RGT"; w3.namaKecamatan="Kec. Rungkut";
    w3.totalVolumeTon=7.4;  w3.jumlahTransaksi=130; tambahWilayah(w3);
    DataWilayah w4; w4.idPos="POS-KNJ"; w4.namaKecamatan="Kec. Kenjeran";
    w4.totalVolumeTon=11.1; w4.jumlahTransaksi=185; tambahWilayah(w4);
    DataWilayah w5; w5.idPos="POS-MLY"; w5.namaKecamatan="Kec. Mulyorejo";
    w5.totalVolumeTon=5.6;  w5.jumlahTransaksi=95;  tambahWilayah(w5);
    DataWilayah w6; w6.idPos="POS-SUK"; w6.namaKecamatan="Kec. Sukolilo";
    w6.totalVolumeTon=8.3;  w6.jumlahTransaksi=140; tambahWilayah(w6);
    DataWilayah w7; w7.idPos="POS-TBN"; w7.namaKecamatan="Kec. Tambaksari";
    w7.totalVolumeTon=10.5; w7.jumlahTransaksi=172; tambahWilayah(w7);
    DataWilayah w8; w8.idPos="POS-SWH"; w8.namaKecamatan="Kec. Sawahan";
    w8.totalVolumeTon=6.9;  w8.jumlahTransaksi=115; tambahWilayah(w8);
    sortirWilayah();

    // Graph seed (peta jaringan Surabaya)
    int gbg = tambahLokasi("Pos Gubeng",         -7.265, 112.748);
    int wnk = tambahLokasi("Pos Wonokromo",      -7.310, 112.728);
    int rgt = tambahLokasi("Pos Rungkut",        -7.320, 112.790);
    int knj = tambahLokasi("Pos Kenjeran",       -7.230, 112.780);
    int mly = tambahLokasi("Pos Mulyorejo",      -7.250, 112.790);
    int suk = tambahLokasi("Pos Sukolilo",       -7.295, 112.800);
    int tbn = tambahLokasi("Pos Tambaksari",     -7.255, 112.750);
    int hub = tambahLokasi("Hub Tengah",         -7.280, 112.760);
    int fab = tambahLokasi("Pabrik Daur Ulang",  -7.340, 112.810, true);

    tambahRuas(gbg, hub, 4.2);
    tambahRuas(gbg, tbn, 2.8);
    tambahRuas(wnk, hub, 3.5);
    tambahRuas(wnk, fab, 5.1);
    tambahRuas(rgt, suk, 3.0);
    tambahRuas(rgt, fab, 2.4);
    tambahRuas(knj, mly, 3.8);
    tambahRuas(knj, tbn, 4.5);
    tambahRuas(mly, suk, 5.2);
    tambahRuas(suk, fab, 3.6);
    tambahRuas(tbn, hub, 2.1);
    tambahRuas(hub, fab, 6.3);
    tambahRuas(hub, suk, 4.8);

    // antrean seed (beberapa warga yg sudah mengantri)
    wargaTiba("3578011001010002", KERTAS,  4.0);
    wargaTiba("3578011001010003", PLASTIK, 2.5);
    wargaTiba("3578011001010004", LOGAM,   1.0);
}

QString BankSampahController::generateResi() {
    counterResi++;
    return QString("RES-%1-%2")
        .arg(QDateTime::currentDateTime().toString("yyyyMMdd"))
        .arg(counterResi, 4, 10, QChar('0'));
}

void BankSampahController::simpanPengiriman(DataPengiriman* p) {
    PengirimanNode* node = new PengirimanNode();
    node->data = p;
    node->next = headPengiriman;
    headPengiriman = node;
    totalPengiriman++;
}

// QUEUE
QString BankSampahController::wargaTiba(const QString& noKTP,
                                        JenisSampah j, double berat) {
    DataSetoran s;
    s.noAntrian  = antrean.generateNoAntrian();
    s.noKTPWarga = noKTP;
    s.jenis      = j;
    s.beratKg    = berat;
    s.waktuTiba  = QDateTime::currentDateTime();
    antrean.enqueue(s);
    return s.noAntrian;
}

bool BankSampahController::panggilBerikutnya(DataSetoran& out) {
    return antrean.dequeue(out);
}

QString BankSampahController::prosesSetoran(const DataSetoran& s) {
    // Hitung poin via BST
    int poin = pohonHarga.hitungPoin(s.jenis, s.beratKg);
    if (poin == 0) return ""; //di bawah batas minimum

    // Buat record transaksi
    TransaksiNode tx;
    tx.kodeResi   = generateResi();
    tx.jenis      = s.jenis;
    tx.beratKg    = s.beratKg;
    tx.poinMutasi = poin;
    tx.waktu      = QDateTime::currentDateTime();

    // Simpan ke Linked List riwayat nasabah via Hash Table
    dbNasabah.tambahTransaksi(s.noKTPWarga, tx);
    dbNasabah.updatePoin(s.noKTPWarga, poin);

    // Buat balok sampah terus push ke stack gudang
    BalokSampah balok;
    balok.idBalok   = gudang.generateIdBalok();
    balok.jenis     = s.jenis;
    balok.beratKg   = s.beratKg;
    balok.waktuPres = QDateTime::currentDateTime();
    balok.noKTPAsal = s.noKTPWarga;
    gudang.push(balok);

    updateVolumeWilayah("POS-GBG", s.beratKg / 1000.0);

    return tx.kodeResi;
}

//  NASABAH
bool BankSampahController::daftarNasabah(const QString& ktp, const QString& nama,
                                         const QString& alamat, const QString& telp) {
    return dbNasabah.daftarNasabah(ktp, nama, alamat, telp);
}

NasabahNode* BankSampahController::loginNasabah(const QString& ktp) {
    NasabahNode* n = dbNasabah.cariNasabah(ktp);
    if (n) nasabahAktif = ktp;
    return n;
}

NasabahNode* BankSampahController::getNasabahPtr(const QString& ktp) const {
    return dbNasabah.cariNasabah(ktp);
}

//  WILAYAH / SORTING
void BankSampahController::tambahWilayah(const DataWilayah& w) {
    dataWilayah.tambah(w);
}

void BankSampahController::updateVolumeWilayah(const QString& idPos, double ton) {
    dataWilayah.update(idPos, ton);
}

void BankSampahController::sortirWilayah() {
    mesinSorting.urutkanVolume(dataWilayah);
    mesinSorting.tandaiKritis(dataWilayah, 10.0);
}

//  SEARCHING
TransaksiNode* BankSampahController::cariResi(const QString& ktp,
                                              const QString& resi) const {
    NasabahNode* n = dbNasabah.cariNasabah(ktp);
    if (!n) return nullptr;
    return mesinCari.cariResiLinear(n->headRiwayat, resi);
}

NasabahNode* BankSampahController::cariNasabahNama(const QString& nama) const {
    return mesinCari.cariNasabahByNama(dbNasabah, nama);
}

DataWilayah* BankSampahController::cariWilayah(const QString& idPos) {
    return mesinCari.cariWilayahLinear(dataWilayah, idPos);
}

//  GRAPH / PENGIRIMAN
int BankSampahController::tambahLokasi(const QString& n, double lat, double lon, bool p) {
    return peta.tambahLokasi(n, lat, lon, p);
}

void BankSampahController::tambahRuas(int a, int b, double km) {
    peta.tambahJalan(a, b, km);
}

DataPengiriman* BankSampahController::buatPengiriman(const QString& idPos, int nodeAsal) {
    int pabrik = peta.cariPabrikUtama();
    if (pabrik < 0 || gudang.isEmpty()) return nullptr;

    DataPengiriman* p = new DataPengiriman();
    p->kodeResi      = generateResi();
    p->idPosAsal     = idPos;
    p->nodeAsal      = nodeAsal;
    p->nodeTujuan    = pabrik;
    p->status        = MENUNGGU;
    p->waktuBerangkat = QDateTime::currentDateTime();

    // nganu balok dari Stack
    while (!gudang.isEmpty()) {
        BalokSampah balok;
        gudang.pop(balok);
        p->totalBeratKg += balok.beratKg;
        p->jumlahBalok++;
    }

    HasilDijkstra* hasil = peta.dijkstra(nodeAsal);
    p->jarakOptimalKm    = hasil[pabrik].jarak;
    p->jalurOptimal      = peta.rekonstruksiJalur(hasil, pabrik, p->panjangJalur);
    delete[] hasil;

    simpanPengiriman(p);
    return p;
}

bool BankSampahController::updateStatusPengiriman(const QString& resi, StatusPengiriman st) {
    PengirimanNode* curr = headPengiriman;
    while (curr) {
        if (curr->data && curr->data->kodeResi == resi) {
            curr->data->status = st;
            return true;
        }
        curr = curr->next;
    }
    return false;
}
