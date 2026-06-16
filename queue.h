#ifndef QUEUE_H
#define QUEUE_H
#include "DataStructures.h"

class Queue {
private:
    AntreanNode* front;
    AntreanNode* rear;
    int          ukuran;
    int          counter;

public:
    Queue();
    ~Queue();

    void        enqueue(const DataSetoran& setoran);
    bool        dequeue(DataSetoran& hasil);
    bool        peek(DataSetoran& hasil) const;
    bool        isEmpty() const { return ukuran == 0; }
    int         getUkuran() const { return ukuran; }
    AntreanNode* getHead() const { return front; }
    void        clear();
    QString     generateNoAntrian();
};

#endif // QUEUE_H
