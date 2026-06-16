#ifndef GRAPH_H
#define GRAPH_H
#include "DataStructures.h"

class GrafJaringan {
private:
    LokasiNode lokasi[MAKS_NODE];
    EdgeNode*  adjList[MAKS_NODE];
    int        jumlahNode;

    struct HeapItem { double jarak; int nodeId; };
    struct MinHeap {
        HeapItem data[MAKS_NODE * MAKS_NODE];
        int      ukuran;
        MinHeap() : ukuran(0) {}
        void     push(double j, int id);
        HeapItem pop();
        bool     isEmpty() const { return ukuran == 0; }
    private:
        void heapifyUp(int i);
        void heapifyDown(int i);
    };

public:
    GrafJaringan();
    ~GrafJaringan();

    int  tambahLokasi(const QString& nama, double lat, double lon, bool isPabrik = false);
    void tambahJalan(int dari, int ke, double bobot);          // bidirectional
    void tambahJalanSatuArah(int dari, int ke, double bobot);  // directional

    HasilDijkstra* dijkstra(int sumber) const;   // caller harus delete[]
    int*  rekonstruksiJalur(const HasilDijkstra* hasil, int tujuan,
                           int& panjang) const; // caller harus delete[]
    int   cariPabrikUtama() const;
    int   getJumlahNode() const { return jumlahNode; }
    const LokasiNode& getLokasi(int id) const { return lokasi[id]; }
    EdgeNode* getAdjList(int id) const;
    void  clear();
};

#endif // GRAPH_H
