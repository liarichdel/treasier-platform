#ifndef HASHTABLE_H
#define HASHTABLE_H
#include "DataStructures.h"

class HashTableNasabah {
private:
    NasabahNode* bucket[HASH_SIZE];
    int          jumlahNasabah;

    int hashFunction(const QString& noKTP) const;

public:
    HashTableNasabah();
    ~HashTableNasabah();

    bool          daftarNasabah(const QString& noKTP, const QString& nama,
                       const QString& alamat, const QString& noTelp);
    NasabahNode*  cariNasabah(const QString& noKTP) const;
    bool          tambahTransaksi(const QString& noKTP, const TransaksiNode& tx);
    bool          updatePoin(const QString& noKTP, int delta);
    bool          hapusNasabah(const QString& noKTP);
    int           getJumlah() const { return jumlahNasabah; }
    NasabahNode*  getBucket(int i) const;
    int           getBucketCount() const { return HASH_SIZE; }
    int           getHashIndex(const QString& noKTP) const { return hashFunction(noKTP); }
};

#endif // HASHTABLE_H
