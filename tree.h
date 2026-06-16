#ifndef TREE_H
#define TREE_H
#include "DataStructures.h"

class PohonKlasifikasi {
private:
    HargaNode* root;

    HargaNode* insert(HargaNode* node, JenisSampah j, double poin, double minKg);
    HargaNode* search(HargaNode* node, JenisSampah j) const;
    void       destroy(HargaNode* node);
    void       inorder(HargaNode* node, QStringList& out) const;

public:
    PohonKlasifikasi();
    ~PohonKlasifikasi();

    void    tambahKategori(JenisSampah j, double poinPerKg, double minKg);
    int     hitungPoin(JenisSampah j, double beratKg) const;
    double  getPoinPerKg(JenisSampah j) const;
    double  getMinKg(JenisSampah j) const;
    bool    updateHarga(JenisSampah j, double hargaBaru);
    QStringList getInorderList() const;
    HargaNode*  getRoot() const { return root; }
};

#endif // TREE_H
