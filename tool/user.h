#ifndef USER_H
#define USER_H

#include <QWidget>
#include <QMainWindow>

#include <QTextEdit>

#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <QJsonDocument>
#include <QJsonParseError>
#include <QVariantMap>

#include "ui.h"

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
        user(Ui *ui, QWidget *parent = Q_NULLPTR);

        //布局
        QGridLayout *layout = new QGridLayout(this);

        //button
        QPushButton *search_nickname_button = new QPushButton("查询");
        QPushButton *reward_users_button = new QPushButton("发送奖励");

        //下拉BOX
        QComboBox *server_num_box = new QComboBox(this);

        //LABEL
        QLabel *search_nickname_label = new QLabel("昵称查询");
        QLabel *reward_frame_result_label = new QLabel();

        //输入框
        QLineEdit *search_nickname_line = new QLineEdit();
        QLineEdit *reward_id_line = new QLineEdit();

        //窗口
        QWidget *widget = new QWidget;

        //文本框
        QTextEdit *user_ids_line = new QTextEdit;

    private Q_SLOTS:
        void setServerInfo(QNetworkReply *reply);    //更新SERVER信息
        void searchUserInfo();      //按照昵称查询用户的信息
        void setUserInfo(QNetworkReply *reply);    //将查询到的玩家的昵称信息展示
        void rewardUsersPortraitOrFrame();         //奖励玩家头像和头像框
        void rewardPFResult(QNetworkReply *reply);  //奖励的结果
};

#endif // USER_H
