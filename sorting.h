#ifndef SORTING_H
#define SORTING_H
#include "DataStructures.h"

class ArrayWilayah {
private:
    DataWilayah* data;
    int          ukuran;
    int          kapasitas;
    void         resize();

public:
    ArrayWilayah(int kapAwal = 16);
    ~ArrayWilayah();

    void         tambah(const DataWilayah& w);
    bool         update(const QString& idPos, double tambahTon);
    DataWilayah* get(int i);
    const DataWilayah* get(int i) const;
    int          getUkuran() const { return ukuran; }
    void         clear();
    DataWilayah* getData() { return data; }
};

//merge sort
class SortingWilayah {
private:
    void merge(DataWilayah* arr, int l, int m, int r);
    void mergeSort(DataWilayah* arr, int l, int r);

public:
    void urutkanVolume(ArrayWilayah& w);
    void tandaiKritis(ArrayWilayah& w, double threshold = 10.0);
};

#endif // SORTING_H
