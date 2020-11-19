//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#include "TextFile.h"
#include "AutoUTF8BOMDevice.h"
#include <QFile>

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

TextFile::TextFile(QObject* parent) :
    QObject(parent),
    m_Device(nullptr),
    m_TextStream(nullptr)
#ifdef AUTO_UTF8_BOM
  , m_AutoUTF8Device(nullptr)
#endif
{
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

TextFile::~TextFile()
{
    if (m_Device || m_TextStream)
    {
        close();
    }
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

void TextFile::close()
{
    if (m_TextStream)
    {
        delete m_TextStream;
        m_TextStream = nullptr;
    }

#ifdef AUTO_UTF8_BOM
    if (m_AutoUTF8Device)
    {
        m_AutoUTF8Device->close();
        delete m_AutoUTF8Device;
    }
#endif

    if (m_Device)
    {
        m_Device->close();
        delete m_Device;
    }
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

bool TextFile::open(const QUrl& fileUrl)
{
    QString filePath = fileUrl.toLocalFile();
    if (fileUrl.scheme() == "qrc")
    {
        filePath = ":" + fileUrl.path();
    }

    return open(filePath);
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

bool TextFile::open(const QString& filePath)
{
    if (m_Device || m_TextStream)
    {
        close();
    }

    QFile* file = new QFile();
    file->setFileName(filePath);
    if (!file->open(QFile::ReadOnly))
    {
        delete file;
        return false;
    }

#ifndef AUTO_UTF8_BOM
    m_Device = file;
    m_TextStream = new QTextStream(file);
#endif

#ifdef AUTO_UTF8_BOM
    m_Device = file;
    m_AutoUTF8Device = new AutoUTF8BOMDevice(file);
    m_AutoUTF8Device->open(QIODevice::ReadOnly);
    m_TextStream = new QTextStream(m_AutoUTF8Device);
#endif

    return true;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

QString TextFile::read(int maxlen)
{
    if (!m_TextStream)
    {
        return QString();
    }

    return m_TextStream->read(maxlen);
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

QString TextFile::readAll()
{
    if (!m_TextStream)
    {
        return QString();
    }

    return m_TextStream->readAll();
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

QString TextFile::readLine(int maxlen)
{
    if (!m_TextStream)
    {
        return QString();
    }

    return m_TextStream->readLine(maxlen);
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
