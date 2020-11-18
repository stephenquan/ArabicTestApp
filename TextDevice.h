#ifndef __TextDevice__
#define __TextDevice__

#include <QIODevice>

class TextDevice : public QIODevice
{
public:
    TextDevice(QIODevice* internal);

    bool open(OpenMode mode) Q_DECL_OVERRIDE;
    void close() Q_DECL_OVERRIDE;

    qint64 pos() const Q_DECL_OVERRIDE;
    qint64 size() const Q_DECL_OVERRIDE;
    bool seek(qint64 pos) Q_DECL_OVERRIDE;
    bool atEnd() const Q_DECL_OVERRIDE;
    bool reset() Q_DECL_OVERRIDE;

    qint64 bytesAvailable() const Q_DECL_OVERRIDE;
    qint64 bytesToWrite() const Q_DECL_OVERRIDE;

    qint64 readData(char *data, qint64 maxlen) Q_DECL_OVERRIDE;
    qint64 writeData(const char *data, qint64 len) Q_DECL_OVERRIDE;

protected:
    QIODevice* m_Internal;

};

#endif
