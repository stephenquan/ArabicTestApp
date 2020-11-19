//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#include "AutoUTF8BOMDevice.h"
#include <QDebug>
#include <QTextCodec>

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

const QByteArray AutoUTF8BOMDevice::UTF8_BOM("\xEF\xBB\xBF", 3);
const QByteArray AutoUTF8BOMDevice::UTF16BE_BOM("\xFE\xFF", 2);
const QByteArray AutoUTF8BOMDevice::UTF16LE_BOM("\xFF\xFE", 2);

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

AutoUTF8BOMDevice::AutoUTF8BOMDevice(QIODevice* internal) :
    m_Internal(internal),
    m_Offset(0)
{
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

bool AutoUTF8BOMDevice::open(OpenMode mode)
{
    if (!m_Internal)
    {
        return false;
    }

    if (!QIODevice::open(mode))
    {
        return false;
    }

    m_Buffer = m_Internal->read(1024);

    /*
    bool hasBOM = false;
    if (m_Buffer.left(UTF8_BOM.length()) == UTF8_BOM)
    {
        hasBOM = true;
    }
    if (m_Buffer.left(UTF16BE_BOM.length()) == UTF16BE_BOM)
    {
        hasBOM = true;
    }
    if (m_Buffer.left(UTF16LE_BOM.length()) == UTF16LE_BOM)
    {
        hasBOM = true;
    }
    */

    bool hasBOM = QTextCodec::codecForUtfText(m_Buffer, nullptr);

    // Test to see if this is an implicit UTF16BE .. 0.1%
    // Test to see if this is an implicit UTF16LE .. 0.1%
    // Test to see if this is an implicit UTF8 ... 80%
    // Test to see if this is a region specific local file .. 15%

    if (!hasBOM)
    {
        // Assume UTF8 if BOM was missing
        m_Buffer.insert(0, UTF8_BOM);
        m_Offset = UTF8_BOM.length();
    }

    return true;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

void AutoUTF8BOMDevice::close()
{
    QIODevice::close();
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

qint64 AutoUTF8BOMDevice::pos() const
{
    if (!m_Internal)
    {
        return 0;
    }

    return m_Internal->pos() - m_Buffer.length() + m_Offset;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

qint64 AutoUTF8BOMDevice::size() const
{
    if (!m_Internal)
    {
        return 0;
    }

    return m_Internal->size() + m_Offset;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

bool AutoUTF8BOMDevice::seek(qint64 pos)
{
    if (!m_Internal)
    {
        return 0;
    }

    m_Buffer.clear();
    return m_Internal->seek(pos - m_Offset);
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

bool AutoUTF8BOMDevice::atEnd() const
{
    if (!m_Internal)
    {
        return true;
    }

    if (!m_Buffer.isEmpty())
    {
        return false;
    }

    return m_Internal->atEnd();
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

bool AutoUTF8BOMDevice::reset()
{
    return seek(0);
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

qint64 AutoUTF8BOMDevice::bytesAvailable() const
{
    if (!m_Internal)
    {
        return 0;
    }

    return m_Buffer.length() + m_Internal->bytesAvailable();
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

qint64 AutoUTF8BOMDevice::bytesToWrite() const
{
    return 0;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

qint64 AutoUTF8BOMDevice::readData(char *data, qint64 maxlen)
{
    qDebug() << Q_FUNC_INFO << "maxlen: " << maxlen;
    Q_UNUSED(data)
    Q_UNUSED(maxlen)

    if (!m_Internal)
    {
        return 0;
    }

    if (maxlen <= 0)
    {
        return 0;
    }

    int total = 0;
    if (m_Buffer.length() > 0)
    {
        int avail = qMin<int>(static_cast<int>(maxlen), m_Buffer.length());
        memcpy(data, m_Buffer.data(), static_cast<size_t>(avail));
        data += avail;
        m_Buffer.remove(0, avail);
        total += avail;
        maxlen -= avail;
    }

    if (maxlen > 0)
    {
        total += m_Internal->read(data, maxlen);
    }

    return total;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

qint64 AutoUTF8BOMDevice::writeData(const char *data, qint64 len)
{
    Q_UNUSED(data)
    Q_UNUSED(len)
    return 0;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
