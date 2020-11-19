//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#ifndef __TextFile__
#define __TextFile__

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#include <QObject>
#include <QUrl>
#include <QIODevice>
#include <QTextStream>

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

//#define AUTO_UTF8_BOM

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class TextFile : public QObject
{
    Q_OBJECT

public:
    TextFile(QObject* parent = nullptr);
    ~TextFile();

    Q_INVOKABLE void close();
    Q_INVOKABLE bool open(const QUrl& fileUrl);
    Q_INVOKABLE QString read(int maxlen = -1);
    Q_INVOKABLE QString readAll();
    Q_INVOKABLE QString readLine(int maxLen = 0);

protected:
    QIODevice* m_Device;
    QTextStream* m_TextStream;
#ifdef AUTO_UTF8_BOM
    QIODevice* m_AutoUTF8Device;
#endif

    bool open(const QString& filePath);

};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#endif

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
