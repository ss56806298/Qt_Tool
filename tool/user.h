#ifndef USER_H
#define USER_H

#include <QWidget>
#include <QMainWindow>

#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <QJsonDocument>
#include <QJsonParseError>
#include <QVariantMap>

#include "ui.h"

typedef struct serverDetail{
    QString server_num;
    QString server_name;
    QString server_address;
}sd;

class user : public QMainWindow
{
    public:
        QString server_area;   //渠道组
        QMap <QString, sd> serverMap;

    private:
        QNetworkAccessManager *n_manager;

        QString getAddressByServerNum(QString server_num);  //获取游戏的url,根据server的num

    Q_OBJECT
    public:
        user(ui *ui, QWidget *parent = Q_NULLPTR);

        //布局
        QGridLayout *layout = new QGridLayout(this);

        //button
        QPushButton *search_nickname_button = new QPushButton("查询");

        //下拉BOX
        QComboBox *server_num_box = new QComboBox(this);

        //LABEL
        QLabel *search_nickname_label = new QLabel("昵称查询");

        //输入框
        QLineEdit *search_nickname_line = new QLineEdit();

        //窗口
        QWidget *widget = new QWidget;

    private Q_SLOTS:
        void setServerInfo(QNetworkReply *reply);    //更新SERVER信息
        void searchUserInfo();      //按照昵称查询用户的信息
        void setUserInfo(QNetworkReply *reply);    //将查询到的玩家的昵称信息展示
};

#endif // USER_H
