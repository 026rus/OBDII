#include "jsonparse.h"

class JSONParseData : public QSharedData
{
public:

};

JSONParse::JSONParse() : data(new JSONParseData)
{

}

JSONParse::JSONParse(const JSONParse &rhs) : data(rhs.data)
{

}

JSONParse &JSONParse::operator=(const JSONParse &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

JSONParse::~JSONParse()
{

}

