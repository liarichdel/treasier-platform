#include "Stack.h"

StackGudang::StackGudang(int kap)
    : top(nullptr), ukuran(0), kapasitasMaks(kap), counterBalok(0) {}

StackGudang::~StackGudang() { clear(); }

void StackGudang::clear() {
    while (top) {
        StackNode* tmp = top;
        top = top->next;
        delete tmp;
    }
    ukuran = 0;
}

QString StackGudang::generateIdBalok() {
    counterBalok++;
    return QString("BLK-%1").arg(counterBalok, 4, 10, QChar('0'));
}

bool StackGudang::push(const BalokSampah& balok) {
    if (isFull()) return false;
    StackNode* node = new StackNode();
    node->data = balok;
    node->next = top;
    top = node;
    ukuran++;
    return true;
}

bool StackGudang::pop(BalokSampah& hasil) {
    if (!top) return false;
    hasil    = top->data;
    StackNode* tmp = top;
    top      = top->next;
    delete tmp;
    ukuran--;
    return true;
}

bool StackGudang::peek(BalokSampah& hasil) const {
    if (!top) return false;
    hasil = top->data;
    return true;
}
