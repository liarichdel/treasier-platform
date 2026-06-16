#ifndef STACK_H
#define STACK_H
#include "DataStructures.h"

class StackGudang {
private:
    StackNode* top;
    int        ukuran;
    int        kapasitasMaks;
    int        counterBalok;

public:
    StackGudang(int kapasitas = 100);
    ~StackGudang();

    bool       push(const BalokSampah& balok);
    bool       pop(BalokSampah& hasil);
    bool       peek(BalokSampah& hasil) const;
    bool       isFull()  const { return ukuran >= kapasitasMaks; }
    bool       isEmpty() const { return ukuran == 0; }
    int        getUkuran() const { return ukuran; }
    int        getKapasitas() const { return kapasitasMaks; }
    StackNode* getTop() const { return top; }
    void       clear();
    QString    generateIdBalok();
};

#endif // STACK_H
