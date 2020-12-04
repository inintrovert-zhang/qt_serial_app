#include "sqlui.h"


SQLUI::SQLUI(QWidget *parent) :
    QWidget(parent)
{
    SetDBName("Hanvor_ble_UID");
    opendatabase();
}

SQLUI::~SQLUI()
{
    _db.close();
}

void SQLUI::SetDBName(QString table)
{
    _tableName = table + ".db";
}

/*
    打开数据库.
*/
bool SQLUI::opendatabase()
{
    if(QSqlDatabase::contains("qt_sql_default_connection"))
      _db = QSqlDatabase::database("qt_sql_default_connection");
    else
      _db = QSqlDatabase::addDatabase("QSQLITE");

    _db.setDatabaseName(_tableName);
    return _db.open();
}

bool SQLUI::CreatTable(QString &TableName,QStringList &value)
{
    QSqlQuery query(QSqlDatabase::database());
    QString sql = QString("create table ") + TableName + QString("(");
    int i;
    for (i=0; i < value.size(); i++)
    {
        if(i == 0)
        {
            sql = sql + value.value(i)+" varchar PRIMARY KEY";
            if (i != value.size() - 1)
            {
                sql+=QString(",");
            }
            else
            {
                sql = sql + QString(")");
            }
        }else
        {
            sql = sql + value.value(i);
            if (i != value.size() - 1)
            {
                sql+=QString(",");
            }
            else
            {
                sql = sql + QString(")");
            }
        }
    }
    if (query.exec(sql))
    {
        return true;
    }
    else
    {
 //       firstuse.show();
        return false;
    }
}

/*
    插入函数.
    构造SQL插入语句.
*/
bool SQLUI::insert(QString &table, QStringList &names, QStringList &values)
{
    if (names.size() != values.size())
    {
        return false;
    }

    QSqlQuery query(QSqlDatabase::database());
    QString sql = QString("insert into ") + table + QString("(");

    int i;

    for (i=0; i < names.size(); i++)
    {
        sql = sql + names.value(i);
        if (i != names.size() - 1)
        {
            sql+=QString(",");
        }
        else
        {
            sql = sql + QString(")");
        }
    }

    sql = sql + QString(" values(");

    for (i = 0; i < values.size(); i++)
    {
        sql = sql + QString("'") + values.value(i) + QString("'");
        if (i != values.size()-1)
        {
            sql = sql + QString(",");
        }
    }

    sql = sql + QString(")");
    qDebug() << sql;
    if (query.exec(sql))
    {
        return true;
    }
    else
    {
 //       firstuse.show();
        return false;
    }
}

/*
    修改函数.
    构造SQL修改语句.
*/
bool SQLUI::Updata(QString &table, QStringList &names, QStringList &values, QString &expression)
{
    if (names.size() != values.size())
    {
        return false;
    }

    //UPDATE 表名称 SET 列名称 = 新值 WHERE 列名称 = 某值
    QSqlQuery query(QSqlDatabase::database());
    QString sql = QString("update ")+table+QString(" set ");
    for (int i = 0; i < names.size(); i++)
    {
        sql = sql + names.value(i);
        sql = sql + QString(" = '");
        sql = sql + values.value(i);
        sql = sql + QString("'");
        if (i != names.size()-1)
        {
            sql = sql + QString(" ,");
        }
    }
    sql = sql + QString(" where ") + expression;
     qDebug() << sql;
    if (query.exec(sql))
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*
    删除函数.
    构造SQL删除语句.
*/
bool SQLUI::del(QString &table, QString &expression)
{
    //DELETE FROM 表名称 WHERE 列名称 = 值
    QSqlQuery query(QSqlDatabase::database());
    QString sql = QString("delete from ") + table + QString(" where ") + expression;

    if (query.exec(sql))
    {
        return true;
    }
    else
    {
        return false;
    }
}

int SQLUI::getToatalRecNum(QString &table)
{
    QSqlQuery query(QSqlDatabase::database());
    QString sql = QString("select * from ") + table;
    qDebug() << sql;
    query.exec(sql);
    QSqlQueryModel *queryModel = new QSqlQueryModel();
    queryModel->setQuery(query);
    while(queryModel->canFetchMore())
    {
        queryModel->fetchMore();
    }
    qDebug() <<"queryModel->rowCount()" << queryModel->rowCount();
    return queryModel->rowCount();
}

//Select * From #temptable Where no>=10 And no < 20
int SQLUI::getThisRecNum(QString &table, QString &time)
{
    QSqlQuery query(QSqlDatabase::database());
    QString sql = QString("select * from "+table+" where time > '"+time+"'");;
    qDebug() << sql;
    query.exec(sql);
    QSqlQueryModel *queryModel = new QSqlQueryModel();
    queryModel->setQuery(query);
    while(queryModel->canFetchMore())
    {
        queryModel->fetchMore();
    }
    return queryModel->rowCount();
}

int SQLUI::getThisComRecNum(QString &table, QString &time, QString &com)
{
    QSqlQuery query(QSqlDatabase::database());
    QString sql = QString("select * from "+table+" where time > '"+time+"' and ErrCode like '%"+com+"%'");
    qDebug() << sql;
    query.exec(sql);
    QSqlQueryModel *queryModel = new QSqlQueryModel();
    queryModel->setQuery(query);
    while(queryModel->canFetchMore())
    {
        queryModel->fetchMore();
    }
    return queryModel->rowCount();
}



void SQLUI::QueryAllDataFromOne(QString column,QString &value,QString &value1)
{
/*    QString sql;
    QSqlQuery query(QSqlDatabase::database());
    if(value1 == NULL)
    {
        sql = QString("select ProductName,BoxNumber,QRCode,Status,time from QRCODE where "\
                              +column+" = '"+value+"'");
    }else
    {
        sql = QString("select ProductName,BoxNumber,QRCode,Status,time from QRCODE where "\
                              +column+" > '"+value+"' and "+column+" < '"+value1+"'");
    }

    //清空上次表格残留内容
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->clearContents();

    //开始查询
    bool queryResult = query.exec(sql);
    //设置5列
    ui->tableWidget->setColumnCount(5);

    if(queryResult)
    {
        int row = 0;
        while (query.next())
        {
            ui->tableWidget->insertRow(row);//添加新行
            ui->tableWidget->setItem( row, 0, new QTableWidgetItem( query.value(0).toString()));
            ui->tableWidget->setItem( row, 1, new QTableWidgetItem( query.value(1).toString()));
            ui->tableWidget->setItem( row, 2, new QTableWidgetItem( query.value(2).toString()));
            ui->tableWidget->setItem( row, 3, new QTableWidgetItem( query.value(3).toString()));
            ui->tableWidget->setItem( row, 4, new QTableWidgetItem( query.value(4).toString()));
            row++;
        }
        row = 0;
        QStringList Headers;
        //自动适应宽度不太好看，通过设置标题宽度间接设置最小宽度
        Headers<<tr("------产品名称------")<<tr("---------箱号---------")\
              <<tr("---------------------二维码---------------------")\
             <<tr("---状态---")<<tr("-----------时间-----------");
        ui->tableWidget->setHorizontalHeaderLabels(Headers);

        //设置表头背景色
        ui->tableWidget->horizontalHeader()\
                ->setStyleSheet("QHeaderView::section{background:skyblue;}");

        //设置每次选择为选定一行
        ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

        //设置充满表宽度
        ui->tableWidget->horizontalHeader()->setStretchLastSection(true);

        ui->tableWidget->resizeColumnsToContents();

        //设置表头字体加粗
        QFont font = ui->tableWidget->horizontalHeader()->font();
        font.setBold(true);
        ui->tableWidget->horizontalHeader()->setFont(font);
    }
    else
    {
        firstuse.show();
    }*/
}

void SQLUI::GetValues(QString &table, QStringList &values)
{
    QSqlQuery query(QSqlDatabase::database());
    QString sql = QString("select * from ") + table;
    if(query.exec(sql))
    {
        while (query.next())
        {
            values << query.value(0).toString();
        }
    }else
    {
       // firstuse.show();
    }
}

void SQLUI::GetValues2(QString &table, QStringList &values, QStringList &values2, QStringList &values3)
{
    QSqlQuery query(QSqlDatabase::database());
    QString sql = QString("select * from ") + table;
    qDebug() << sql;
    if(query.exec(sql))
    {
        while (query.next())
        {
            values << query.value(0).toString();
            values2 << query.value(1).toString();
            values3 << query.value(2).toString();
        }
    }else
    {
       // firstuse.show();
        qDebug() << "sql no dat";
    }
}

bool SQLUI::CheckRepeat(QString &TableName,QString &column,QString &value)
{
    QSqlQuery query(QSqlDatabase::database());
    QString sql = QString("select "+column+" from "+TableName+" where "+column+" = '"+value+"'");
    qDebug() << sql;
    if(query.exec(sql))
    {
        if(query.next())
        {
            return true;
        }
    }
    return false;
}

bool SQLUI::CheckRepeat2(QString &TableName, QString &column, QString &value, QString &column2, QString &value2)
{
    QSqlQuery query(QSqlDatabase::database());
    QString sql = QString("select "+column+" from "+TableName+" where "+column+" = '"+value+"' and "+column2+" = '"+value2+"'");
    qDebug() << sql;
    if(query.exec(sql))
    {
        if(query.next())
        {
            return true;
        }
    }
    return false;
}

bool SQLUI::CheckPWD(QString PWD)
{
    QString pwd;
    QSqlQuery query(QSqlDatabase::database());
    if(query.exec("select PASSWD from USER where USER='Admin'"))
    {
        while(query.next())
        {
            pwd = query.value(0).toString();
            qDebug() << pwd;
        }
        if(pwd == PWD)
        {
            qDebug() << "OK";
            return true;
        }else
        {
            qDebug() << "Fail";
            return false;
        }
    }else
    {
       // firstuse.show();
        return false;
    }
}

void SQLUI::InitDB()
{
    QStringList QRKEY,ADKEY,ADVALUE,ERRKEY;
    QString QRTable = "QRCODE";
    QString AdminTable = "USER";
    QString QRTableErr = "QERRCODE";

    QRKEY<<"UID_16";
    ERRKEY<<"time"<<"UID_16"<<"ErrCode";
    ADKEY<<"USER"<<"PASSWD";
    ADVALUE<<"Admin"<<"123456";

    QString username = "Admin";
    bool repeat = CheckRepeat(AdminTable,AdminTable,username);
    if(!repeat)
    {
        SetDBName("Hanvor_ble_UID");
        opendatabase();

        CreatTable(QRTable,QRKEY);
        CreatTable(QRTableErr,ERRKEY);
        CreatTable(AdminTable,ADKEY);

        insert(AdminTable,ADKEY,ADVALUE);
        qDebug() << "InitSQL";
    }else
        qDebug() << "DB Already Initialed...";
}

void SQLUI::on_OKPushButton_clicked()
{
 /*   QString column,value,StartTime,EndTime,null;
    null = "";
    if(ui->QRcodeButton->isChecked())
    {
        column = "QRCode";
        value = ui->QRText->text();
        QueryAllDataFromOne(column,value,null);
    }else if(ui->BoxNumButton->isChecked())
    {
        column = "BoxNumber";
        value = ui->Boxtext->text();
        QueryAllDataFromOne(column,value,null);
    }else if(ui->TimeButton->isChecked())
    {
        column = "time";

        QDate Start = ui->StartTime->date();
        QDate End = ui->EndTime->date();
        StartTime = Start.toString("yyyyMMdd");
        EndTime = End.toString("yyyyMMdd");

        StartTime = StartTime+"000000";
        EndTime = EndTime+"000000";

        if(EndTime >= StartTime)
        {
            QueryAllDataFromOne(column,StartTime,EndTime);
        }
    }*/
}
