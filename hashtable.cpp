#include "HashTable.h"

HashTableNasabah::HashTableNasabah() : jumlahNasabah(0) {
    for (int i = 0; i < HASH_SIZE; i++) bucket[i] = nullptr;
}

HashTableNasabah::~HashTableNasabah() {
    for (int i = 0; i < HASH_SIZE; i++) {
        NasabahNode* curr = bucket[i];
        while (curr) {
            NasabahNode* tmp = curr;
            curr = curr->nextChain;
            //hapus linked list riwayat
            TransaksiNode* tx = tmp->headRiwayat;
            while (tx) {
                TransaksiNode* ttx = tx;
                tx = tx->next;
                delete ttx;
            }
            delete tmp;
        }
        bucket[i] = nullptr;
    }
}

//polynomial rolling hash pada string noKTP
int HashTableNasabah::hashFunction(const QString& noKTP) const {
    long long hash = 0;
    long long base = 31;
    long long mod  = HASH_SIZE;
    for (QChar c : noKTP) {
        hash = (hash * base + c.unicode()) % mod;
    }
    return static_cast<int>((hash + mod) % mod);
}

bool HashTableNasabah::daftarNasabah(const QString& noKTP, const QString& nama,
                                     const QString& alamat, const QString& noTelp) {
    if (cariNasabah(noKTP)) return false; // Sudah ada
    int idx = hashFunction(noKTP);
    NasabahNode* node = new NasabahNode();
    node->noKTP    = noKTP;
    node->nama     = nama;
    node->alamat   = alamat;
    node->noTelp   = noTelp;
    node->totalPoin = 0;
    //prepend ke bucket (chaining)
    node->nextChain = bucket[idx];
    bucket[idx]     = node;
    jumlahNasabah++;
    return true;
}

NasabahNode* HashTableNasabah::cariNasabah(const QString& noKTP) const {
    int idx = hashFunction(noKTP);
    NasabahNode* curr = bucket[idx];
    while (curr) {
        if (curr->noKTP == noKTP) return curr;
        curr = curr->nextChain;
    }
    return nullptr;
}

bool HashTableNasabah::tambahTransaksi(const QString& noKTP, const TransaksiNode& tx) {
    NasabahNode* n = cariNasabah(noKTP);
    if (!n) return false;
    TransaksiNode* node = new TransaksiNode();
    *node = tx;
    node->next = n->headRiwayat;
    n->headRiwayat = node;
    return true;
}

bool HashTableNasabah::updatePoin(const QString& noKTP, int delta) {
    NasabahNode* n = cariNasabah(noKTP);
    if (!n) return false;
    n->totalPoin += delta;
    if (n->totalPoin < 0) n->totalPoin = 0;
    return true;
}

bool HashTableNasabah::hapusNasabah(const QString& noKTP) {
    int idx = hashFunction(noKTP);
    NasabahNode* curr = bucket[idx];
    NasabahNode* prev = nullptr;
    while (curr) {
        if (curr->noKTP == noKTP) {
            if (prev) prev->nextChain = curr->nextChain;
            else       bucket[idx]    = curr->nextChain;
            TransaksiNode* tx = curr->headRiwayat;
            while (tx) { TransaksiNode* t = tx; tx = tx->next; delete t; }
            delete curr;
            jumlahNasabah--;
            return true;
        }
        prev = curr; curr = curr->nextChain;
    }
    return false;
}

NasabahNode* HashTableNasabah::getBucket(int i) const {
    if (i < 0 || i >= HASH_SIZE) return nullptr;
    return bucket[i];
}
