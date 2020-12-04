#ifndef SQLUI_H
#define SQLUI_H

#include <QDebug>
#include <QWidget>
#include <QString>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QStringList>
#include <QSqlDatabase>


class SQLUI : public QWidget
{
    Q_OBJECT

public:
    explicit SQLUI(QWidget *parent = 0);
    ~SQLUI();

private:
    void SetDBName(QString table);
    bool opendatabase();

public slots:
    bool CreatTable(QString &TableName,QStringList &value);
    bool insert(QString &table, QStringList &names,QStringList &values);
    bool Updata(QString &table, QStringList &names,QStringList &values, QString &expression);
    bool del(QString &table, QString &expression);
    void GetValues(QString &table, QStringList &values);
    void InitDB();
    bool CheckPWD(QString PWD);
    bool CheckRepeat(QString &TableName,QString &column,QString &value);
    void QueryAllDataFromOne(QString column,QString &value,QString &value1);
private slots:
    void on_OKPushButton_clicked();

private:
    QSqlTableModel *model;
    QSqlDatabase _db;
    QString _tableName;
};

#endif // SQLUI_H
