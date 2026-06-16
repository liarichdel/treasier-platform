#include "Graph.h"

// MinHeap

void GrafJaringan::MinHeap::heapifyUp(int i) {
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (data[parent].jarak > data[i].jarak) {
            HeapItem tmp = data[parent];
            data[parent] = data[i];
            data[i]      = tmp;
            i = parent;
        } else break;
    }
}

void GrafJaringan::MinHeap::heapifyDown(int i) {
    while (true) {
        int smallest = i;
        int l = 2*i+1, r = 2*i+2;
        if (l < ukuran && data[l].jarak < data[smallest].jarak) smallest = l;
        if (r < ukuran && data[r].jarak < data[smallest].jarak) smallest = r;
        if (smallest == i) break;
        HeapItem tmp    = data[i];
        data[i]         = data[smallest];
        data[smallest]  = tmp;
        i = smallest;
    }
}

void GrafJaringan::MinHeap::push(double j, int id) {
    data[ukuran] = {j, id};
    heapifyUp(ukuran++);
}

GrafJaringan::HeapItem GrafJaringan::MinHeap::pop() {
    HeapItem top    = data[0];
    data[0]         = data[--ukuran];
    heapifyDown(0);
    return top;
}

// GrafJaringan

GrafJaringan::GrafJaringan() : jumlahNode(0) {
    for (int i = 0; i < MAKS_NODE; i++) adjList[i] = nullptr;
}

GrafJaringan::~GrafJaringan() { clear(); }

void GrafJaringan::clear() {
    for (int i = 0; i < MAKS_NODE; i++) {
        EdgeNode* curr = adjList[i];
        while (curr) {
            EdgeNode* tmp = curr;
            curr = curr->next;
            delete tmp;
        }
        adjList[i] = nullptr;
    }
    jumlahNode = 0;
}

int GrafJaringan::tambahLokasi(const QString& nama, double lat, double lon, bool isPabrik) {
    if (jumlahNode >= MAKS_NODE) return -1;
    int id = jumlahNode++;
    lokasi[id].id           = id;
    lokasi[id].namaLokasi   = nama;
    lokasi[id].latitude     = lat;
    lokasi[id].longitude    = lon;
    lokasi[id].isPabrikUtama = isPabrik;
    return id;
}

void GrafJaringan::tambahJalanSatuArah(int dari, int ke, double bobot) {
    if (dari < 0 || ke < 0 || dari >= jumlahNode || ke >= jumlahNode) return;
    EdgeNode* e = new EdgeNode();
    e->idTetangga = ke;
    e->bobot      = bobot;
    e->next       = adjList[dari];
    adjList[dari] = e;
}

void GrafJaringan::tambahJalan(int dari, int ke, double bobot) {
    tambahJalanSatuArah(dari, ke, bobot);
    tambahJalanSatuArah(ke, dari, bobot);
}

HasilDijkstra* GrafJaringan::dijkstra(int sumber) const {
    HasilDijkstra* hasil = new HasilDijkstra[jumlahNode];
    bool* visited        = new bool[jumlahNode]();
    hasil[sumber].jarak  = 0.0;

    MinHeap heap;
    heap.push(0.0, sumber);

    while (!heap.isEmpty()) {
        HeapItem curr = heap.pop();
        int u = curr.nodeId;
        if (visited[u]) continue;
        visited[u] = true;

        EdgeNode* edge = adjList[u];
        while (edge) {
            int v    = edge->idTetangga;
            double w = edge->bobot;
            if (!visited[v] && hasil[u].jarak + w < hasil[v].jarak) {
                hasil[v].jarak       = hasil[u].jarak + w;
                hasil[v].predecessor = u;
                heap.push(hasil[v].jarak, v);
            }
            edge = edge->next;
        }
    }
    delete[] visited;
    return hasil;
}

int* GrafJaringan::rekonstruksiJalur(const HasilDijkstra* hasil,
                                     int tujuan, int& panjang) const {
    // hitung panjang jalur
    int tmp = tujuan;
    panjang = 0;
    while (tmp != -1) { panjang++; tmp = hasil[tmp].predecessor; }

    if (panjang == 0) return nullptr;
    int* jalur = new int[panjang];
    tmp = tujuan;
    for (int i = panjang - 1; i >= 0; i--) {
        jalur[i] = tmp;
        tmp = hasil[tmp].predecessor;
    }
    return jalur;
}

int GrafJaringan::cariPabrikUtama() const {
    for (int i = 0; i < jumlahNode; i++)
        if (lokasi[i].isPabrikUtama) return i;
    return -1;
}

EdgeNode* GrafJaringan::getAdjList(int id) const {
    if (id < 0 || id >= jumlahNode) return nullptr;
    return adjList[id];
}
