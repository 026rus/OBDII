#include "ParseJson.h"
#include <QFile>
#include <QString>
#include <QDir>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>


ParseJson::ParseJson()
{

    LoadFile();

}

void ParseJson::LoadFile(){
    QFile file("data/pcodes.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        printf("Failed to read\n");
        return;
    }
    QByteArray parsedJson = file.readAll();
    /*while (!file.atEnd()) {
        parsedJson.append(file.readLine());

    }*/

   // printf(parsedJson.constData());
    SearchFile(parsedJson, "P0001");
}

void ParseJson::SearchFile(QByteArray parsedJson, QString code){

    QJsonDocument jsonDoc = QJsonDocument::fromJson(parsedJson);
    QJsonObject jsonObj = jsonDoc.object();

    if (jsonDoc.isObject()){

        QJsonObject::iterator itr = jsonObj.find(code);
        if (itr == jsonObj.end()){
            qDebug() << "Code Not Found";
        }else{
            qDebug() << "GOT HERE";

            QJsonValue jsonVal = jsonObj.value(code);


          //QJsonValue jsonVal = *itr.value()->toObject()->find(code);
            qDebug() << jsonVal.toString();

        }


    }
}

ParseJson::~ParseJson()
{

}

