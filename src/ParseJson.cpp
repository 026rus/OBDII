#include "ParseJson.h"
#include <QFile>
#include <QString>
#include <QDir>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

//using namespace QJsonObject;



ParseJson::ParseJson()
{


    QFile file("data/sample.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        printf("Failed to read\n");
        return;
    }
    QByteArray parsedJson = file.readAll();
    /*while (!file.atEnd()) {
        parsedJson.append(file.readLine());

    }*/

    printf(parsedJson.constData());
    ParseJson(parsedJson, "hello");
}

ParseJson::ParseJson(QByteArray parsedJson, char *code){


    QJsonDocument jsonDoc = QJsonDocument::fromJson(parsedJson);
    QJsonObject jsonObj = jsonDoc.object();
    if (jsonDoc.isObject()){

        //QJsonObject foundCode =
        QJsonValue jsonVal = jsonObj.value(code);

        qDebug() << jsonVal.toString();

    }

}

ParseJson::~ParseJson()
{

}

