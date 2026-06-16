#include "Queue.h"

Queue::Queue() : front(nullptr), rear(nullptr), ukuran(0), counter(0) {}

Queue::~Queue() { clear(); }

void Queue::clear() {
    AntreanNode* curr = front;
    while (curr) {
        AntreanNode* tmp = curr;
        curr = curr->next;
        delete tmp;
    }
    front = rear = nullptr;
    ukuran = 0;
}

QString Queue::generateNoAntrian() {
    counter++;
    return QString("A-%1").arg(counter, 4, 10, QChar('0'));
}

void Queue::enqueue(const DataSetoran& setoran) {
    AntreanNode* node = new AntreanNode();
    node->data = setoran;
    if (!rear) { front = rear = node; }
    else        { rear->next = node; rear = node; }
    ukuran++;
}

bool Queue::dequeue(DataSetoran& hasil) {
    if (!front) return false;
    hasil = front->data;
    AntreanNode* tmp = front;
    front = front->next;
    if (!front) rear = nullptr;
    delete tmp;
    ukuran--;
    return true;
}

bool Queue::peek(DataSetoran& hasil) const {
    if (!front) return false;
    hasil = front->data;
    return true;
}
