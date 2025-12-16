#ifndef ZSTDCOMPRESSOR_H
#define ZSTDCOMPRESSOR_H

#include <QObject>
#include <QByteArray>

class ZstdCompressor : public QObject
{
    Q_OBJECT
public:
    explicit ZstdCompressor(QObject *parent = nullptr);
    QByteArray compress(const QByteArray &data);
    QByteArray decompress(const QByteArray &data);
};

#endif
