#include "Tree.h"

PohonKlasifikasi::PohonKlasifikasi() : root(nullptr) {}
PohonKlasifikasi::~PohonKlasifikasi() { destroy(root); }

void PohonKlasifikasi::destroy(HargaNode* node) {
    if (!node) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
}

HargaNode* PohonKlasifikasi::insert(HargaNode* node, JenisSampah j,
                                    double poin, double minKg) {
    if (!node) {
        HargaNode* n = new HargaNode();
        n->jenis = j; n->poinPerKg = poin; n->batasMinKg = minKg;
        return n;
    }
    if (j < node->jenis)      node->left  = insert(node->left,  j, poin, minKg);
    else if (j > node->jenis) node->right = insert(node->right, j, poin, minKg);
    else { node->poinPerKg = poin; node->batasMinKg = minKg; }
    return node;
}

HargaNode* PohonKlasifikasi::search(HargaNode* node, JenisSampah j) const {
    if (!node || node->jenis == j) return node;
    if (j < node->jenis) return search(node->left,  j);
    else                 return search(node->right, j);
}

void PohonKlasifikasi::tambahKategori(JenisSampah j, double poinPerKg, double minKg) {
    root = insert(root, j, poinPerKg, minKg);
}

int PohonKlasifikasi::hitungPoin(JenisSampah j, double beratKg) const {
    HargaNode* n = search(root, j);
    if (!n || beratKg < n->batasMinKg) return 0;
    return static_cast<int>(beratKg * n->poinPerKg);
}

double PohonKlasifikasi::getPoinPerKg(JenisSampah j) const {
    HargaNode* n = search(root, j);
    return n ? n->poinPerKg : 0.0;
}

double PohonKlasifikasi::getMinKg(JenisSampah j) const {
    HargaNode* n = search(root, j);
    return n ? n->batasMinKg : 0.0;
}

bool PohonKlasifikasi::updateHarga(JenisSampah j, double hargaBaru) {
    HargaNode* n = search(root, j);
    if (!n) return false;
    n->poinPerKg = hargaBaru;
    return true;
}

void PohonKlasifikasi::inorder(HargaNode* node, QStringList& out) const {
    if (!node) return;
    inorder(node->left, out);
    out << QString("%1 | %2 poin/kg | min %3 kg")
               .arg(namaJenisSampah(node->jenis), -12)
               .arg(node->poinPerKg, 6, 'f', 0)
               .arg(node->batasMinKg, 4, 'f', 1);
    inorder(node->right, out);
}

QStringList PohonKlasifikasi::getInorderList() const {
    QStringList out;
    inorder(root, out);
    return out;
}
