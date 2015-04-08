#ifndef PARSEJSON_H
#define PARSEJSON_H
#include <QByteArray>
#include <QString>

class ParseJson
{
public:
    ParseJson();
    ParseJson(QString code);
    void LoadFile(QString code);
    void SearchFile(QByteArray parsedJson, QString code);

    ~ParseJson();
};

#endif // PARSEJSON_H
