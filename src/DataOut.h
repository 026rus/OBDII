#ifndef DATAOUT_H
#define DATAOUT_H

#include <QPair>
#include <QByteArray>
#include <QJsonObject>
#include <QThread>
#include <QObject>
#include <QVector>


class DataOut : public QThread
{
    Q_OBJECT

public:
    DataOut(QVector<QPair<QString,QVector<double> > > data);
    void run() Q_DECL_OVERRIDE;
    ~DataOut();

private:
    QByteArray ToByteArray(QJsonObject input);
    void Send(QByteArray toSend);
    void Collector();

    QVector<QPair<QString,QVector<double> > > input;

};

#endif // DATAOUT_H
