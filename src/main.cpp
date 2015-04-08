#include "CarCoreApp.h"
#include "PortReaderWriter.h"
#include "ParseJson.h"
#include <cstdio>

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <iostream>


using namespace serial;
void parse(char code[]){
/*****************************************************
 //RapidJSON working search of char array
    const char json[] = " { \"P0001\" : \"TEST 1\", \"P0002\" : \"TEST 2\", \"P0003\" : \"TEST 3\" } ";
    rapidjson::Document document;  // Default template parameter uses UTF8 and MemoryPoolAllocator.

    #if 0
        // "normal" parsing, decode strings to new buffers. Can use other input stream via ParseStream().
        if (document.Parse(json).HasParseError())
            return;
    #else
        // In-situ parsing, decode strings directly in the source string. Source must be string.
        char buffer[sizeof(json)];
        memcpy(buffer, json, sizeof(json));
        if (document.ParseInsitu(buffer).HasParseError())
            return;
    #endif

        printf("\nParsing to document succeeded.\n");

        ////////////////////////////////////////////////////////////////////////////
        // 2. Access values in document.

        printf("\nAccess values in document:\n");
        printf("%s = %s\n",code, document[code].GetString());

*********************************************/

}



int main(int argc, char **argv) {

    //CarCoreApp *app = new CarCoreApp(argc, &argv);
    qDebug() << "Application has launched.";
    //app->run();
    ParseJson *parser = new ParseJson();
    //parse("P0002");
    //return app->exec();
    return 1;
}
