#ifndef PARSEJSON_H
#define PARSEJSON_H
#include <QByteArray>
#include <QString>

class ParseJson
{
public:
    ParseJson();
    ParseJson(QString code);
    QString LoadFile(QString code);
    QString SearchFile(QByteArray parsedJson, QString code);
    QString getDesc(QString code);

    ~ParseJson();
};

#endif // PARSEJSON_H
