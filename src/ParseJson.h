#ifndef PARSEJSON_H
#define PARSEJSON_H
#include <QByteArray>

class ParseJson
{
public:
    ParseJson();
    ParseJson(QByteArray parsedJson, char* code);
    ~ParseJson();
};

#endif // PARSEJSON_H
