#include "Searching.h"

TransaksiNode* SearchingEngine::cariResiLinear(TransaksiNode* head,
                                               const QString& kodeResi) const {
    TransaksiNode* curr = head;
    while (curr) {
        if (curr->kodeResi == kodeResi) return curr;
        curr = curr->next;
    }
    return nullptr;
}

int SearchingEngine::cariWilayahBinary(DataWilayah* arr, int n,
                                       const QString& idPos) const {
    int lo = 0, hi = n - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if      (arr[mid].idPos == idPos) return mid;
        else if (arr[mid].idPos <  idPos) lo = mid + 1;
        else                              hi = mid - 1;
    }
    return -1;
}

NasabahNode* SearchingEngine::cariNasabahByNama(const HashTableNasabah& ht,
                                                const QString& nama) const {
    QString namaCari = nama.toLower();
    for (int i = 0; i < ht.getBucketCount(); i++) {
        NasabahNode* curr = ht.getBucket(i);
        while (curr) {
            if (curr->nama.toLower().contains(namaCari)) return curr;
            curr = curr->nextChain;
        }
    }
    return nullptr;
}

DataWilayah* SearchingEngine::cariWilayahLinear(ArrayWilayah& w,
                                                const QString& idPos) const {
    for (int i = 0; i < w.getUkuran(); i++) {
        DataWilayah* d = w.get(i);
        if (d && d->idPos == idPos) return d;
    }
    return nullptr;
}
