#ifndef XORWORKER_H
#define XORWORKER_H

#include <QObject>

class XorWorker : public QObject
{
    Q_OBJECT
public:
    explicit XorWorker(QObject *parent = nullptr);

signals:
};

#endif // XORWORKER_H
