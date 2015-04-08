#ifndef PARSEJSON_H
#define PARSEJSON_H
#include <QByteArray>
#include <QString>

class ParseJson
{
public:
    ParseJson();
    void LoadFile();
    void SearchFile(QByteArray parsedJson, QString code);

    ~ParseJson();
};

#endif // PARSEJSON_H
