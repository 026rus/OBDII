#ifndef JSONPARSE_H
#define JSONPARSE_H

#include <QSharedDataPointer>

class JSONParseData;

class JSONParse
{
public:
    JSONParse();
    JSONParse(const JSONParse &);
    JSONParse &operator=(const JSONParse &);
    ~JSONParse();

private:
    QSharedDataPointer<JSONParseData> data;
};

#endif // JSONPARSE_H
