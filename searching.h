#ifndef SEARCHING_H
#define SEARCHING_H
#include "DataStructures.h"
#include "HashTable.h"
#include "Sorting.h"

class SearchingEngine {
public:
    TransaksiNode* cariResiLinear(TransaksiNode* head, const QString& kodeResi) const;

    int cariWilayahBinary(DataWilayah* arr, int n, const QString& idPos) const;

    NasabahNode* cariNasabahByNama(const HashTableNasabah& ht, const QString& nama) const;

    DataWilayah* cariWilayahLinear(ArrayWilayah& w, const QString& idPos) const;
};

#endif // SEARCHING_H
