#include "Sorting.h"

// ArrayWilayah
ArrayWilayah::ArrayWilayah(int kapAwal)
    : ukuran(0), kapasitas(kapAwal) {
    data = new DataWilayah[kapasitas];
}

ArrayWilayah::~ArrayWilayah() { delete[] data; }

void ArrayWilayah::resize() {
    int newKap = kapasitas * 2;
    DataWilayah* newData = new DataWilayah[newKap];
    for (int i = 0; i < ukuran; i++) newData[i] = data[i];
    delete[] data;
    data = newData;
    kapasitas = newKap;
}

void ArrayWilayah::tambah(const DataWilayah& w) {
    if (ukuran >= kapasitas) resize();
    data[ukuran++] = w;
}

bool ArrayWilayah::update(const QString& idPos, double tambahTon) {
    for (int i = 0; i < ukuran; i++) {
        if (data[i].idPos == idPos) {
            data[i].totalVolumeTon += tambahTon;
            data[i].jumlahTransaksi++;
            return true;
        }
    }
    return false;
}

DataWilayah* ArrayWilayah::get(int i) {
    if (i < 0 || i >= ukuran) return nullptr;
    return &data[i];
}

const DataWilayah* ArrayWilayah::get(int i) const {
    if (i < 0 || i >= ukuran) return nullptr;
    return &data[i];
}

void ArrayWilayah::clear() { ukuran = 0; }

// SortingWilayah (Merge Sort)
void SortingWilayah::merge(DataWilayah* arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    DataWilayah* L = new DataWilayah[n1];
    DataWilayah* R = new DataWilayah[n2];

    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    // descending (volume terbesar di depan)
    while (i < n1 && j < n2) {
        if (L[i].totalVolumeTon >= R[j].totalVolumeTon) arr[k++] = L[i++];
        else                                             arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    delete[] L;
    delete[] R;
}

void SortingWilayah::mergeSort(DataWilayah* arr, int l, int r) {
    if (l >= r) return;
    int m = l + (r - l) / 2;
    mergeSort(arr, l, m);
    mergeSort(arr, m + 1, r);
    merge(arr, l, m, r);
}

void SortingWilayah::urutkanVolume(ArrayWilayah& w) {
    if (w.getUkuran() < 2) return;
    mergeSort(w.getData(), 0, w.getUkuran() - 1);
}

void SortingWilayah::tandaiKritis(ArrayWilayah& w, double threshold) {
    for (int i = 0; i < w.getUkuran(); i++) {
        DataWilayah* d = w.get(i);
        if (d) d->statusKritis = (d->totalVolumeTon >= threshold);
    }
}
