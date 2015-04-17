#include "ParseJson.h"
#include <QFile>
#include <QString>
#include <QDir>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>



ParseJson::ParseJson()
{

   // LoadFile("B0001");

}

ParseJson::ParseJson(QString code){
    LoadFile(code);
}


QString ParseJson::LoadFile(QString code){

    qDebug().noquote() << "Inside 1: "<< code;
    QChar letter = code.at(0);
    QString path;

    qDebug() << "Inside 1: "<< code.at(0);

    switch(letter.toLatin1()){
    case 'B':
        path = "data/Bcodes.json";
        break;
    case 'C':
        path = "data/ccodes.json";
           break;
    case 'U':
        path = "data/ucodes.json";
        break;
    case 'P':
        path = "data/pcodes.json";
        break;
    default:
        path = "";
        break;

    }

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        printf("Failed to read\n");
        return("Failed to read\n");
    }
    QByteArray parsedJson = file.readAll();

    return SearchFile(parsedJson, code);
}

QString ParseJson::SearchFile(QByteArray parsedJson, QString code){

    QJsonDocument jsonDoc = QJsonDocument::fromJson(parsedJson);
    QJsonObject jsonObj = jsonDoc.object();

    if (jsonDoc.isObject()){

        QJsonObject::iterator itr = jsonObj.find(code);
        if (itr == jsonObj.end()){
            qDebug() << "Code Not Found";
            return "Code Not Found";
        }else{

            QJsonValue jsonVal = jsonObj.value(code);


          //QJsonValue jsonVal = *itr.value()->toObject()->find(code);
            qDebug() << "Jason :) " <<jsonVal.toString();
            return jsonVal.toString();
        }


    }
}

QString ParseJson::getDesc(QString code){
    return LoadFile(code);

}

ParseJson::~ParseJson()
{

}

