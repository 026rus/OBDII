#include "ParseJson.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"


using namespace rapidjson;



ParseJson::ParseJson()
{
   /* FILE* fp = fopen("src/data/sample.json", "r"); // non-Windows use "r" windows use "rb"

    fseek(fp, 0, SEEK_END);
    size_t filesize = (size_t)ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char* buffer = (char*)malloc(filesize + 1);
    size_t readLength = fread(buffer, 1, filesize, fp);
    buffer[readLength] = '\0';
    fclose(fp);

    // In situ parsing the buffer into d, buffer will also be modified
    Document d;
    d.ParseInsitu(buffer);

    // Query/manipulate the DOM here...
    assert(d.HasMember("P0001"));
    assert(d["P0001"].IsString());
    printf("P0001 : %s\n", d["P0001"].GetString());

    free(buffer);

    // Note: At this point, d may have dangling pointers pointed to the deallocated buffer.*/
}

ParseJson::~ParseJson()
{

}

