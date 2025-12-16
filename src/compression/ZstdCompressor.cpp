#include "ZstdCompressor.h"

ZstdCompressor::ZstdCompressor(QObject *parent) : QObject(parent) {}
QByteArray ZstdCompressor::compress(const QByteArray &data) { return data; }
QByteArray ZstdCompressor::decompress(const QByteArray &data) { return data; }
