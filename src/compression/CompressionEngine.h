#ifndef COMPRESSIONENGINE_H
#define COMPRESSIONENGINE_H

#include <QObject>

class CompressionEngine : public QObject
{
    Q_OBJECT
public:
    explicit CompressionEngine(QObject *parent = nullptr);
};

#endif
