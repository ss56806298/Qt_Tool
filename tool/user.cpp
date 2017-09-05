#include "user.h"

user::user(ui *ui, QWidget *parent)
//    : QMainWindow(parent)
{
    server_area = ui->area_box->currentText();
    QString main_server_ip = ui->main_server_ip;
    setWindowTitle(server_area);

    //获取现在有哪些渠道组
    n_manager = new QNetworkAccessManager(this);
    connect(n_manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(setServerInfo(QNetworkReply *))); //关联信号和槽

    QUrl url = "http://" + main_server_ip + "/original_server/common/serverSearch?area=" + server_area;

    n_manager->get(QNetworkRequest(url));//发送请求

    //布局
    layout->addWidget(server_num_box, 0, 0, 1, 1);

    layout->addWidget(search_nickname_label, 1, 0, 1, 1);
    layout->addWidget(search_nickname_line, 1, 1, 1, 1);
    layout->addWidget(search_nickname_button, 1, 2, 1, 1);

    layout->addWidget(user_ids_line, 2, 0, 4, 3);

    reward_id_line->setMaxLength(5);
    layout->addWidget(reward_id_line, 6, 0, 1, 1);
    layout->addWidget(reward_users_button, 6, 2, 1, 1);
    layout->addWidget(reward_frame_result_label, 7, 0, 1, 3);

    widget->setLayout(layout);

    setCentralWidget(widget);

    //信号
    connect(search_nickname_button, &QPushButton::clicked, this, &user::searchUserInfo);
    connect(reward_users_button, &QPushButton::clicked, this, &user::rewardUsersPortraitOrFrame);
}


//确定服务器的信息
void user::setServerInfo(QNetworkReply *reply) {
    //处理返回的JSON数据
    QJsonParseError error;

    QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll(), &error);
    if (error.error == QJsonParseError::NoError) {
        if (jsonDocument.isObject()) {
            QVariantMap result = jsonDocument.toVariant().toMap();

            foreach(QVariant serverListT, result["server_list"].toList()) {
                QVariantMap serverList = serverListT.toMap();
                sd sd1;
                sd1.server_num = serverList["server_num"].toString();
                sd1.server_name = serverList["server_name"].toString();
                sd1.server_address = serverList["server_address"].toString();
                serverMap[serverList["server_num"].toString()] = sd1;

                server_num_box->addItem(sd1.server_num);
            }

        }
    } else {
        qDebug()<<error.errorString();
    }

    reply->deleteLater();//最后要释放replay对象
}

//查询用户的信息
void user::searchUserInfo() {

    QString server_num_string = server_num_box->currentText();

    //获取游戏服务器的地址
    QString server_address = getAddressByServerNum(server_num_string);

    //获取需要查询的昵称
    QString nickname = search_nickname_line->text();

    n_manager = new QNetworkAccessManager(this);

    connect(n_manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(setUserInfo(QNetworkReply *))); //关联信号和槽

    QUrl url = "http://" + server_address + "/The_Wall/common/userSearch?nickname=" + nickname;

    n_manager->get(QNetworkRequest(url));//发送请求
}

//将查询到的用户信息显示出来
void user::setUserInfo(QNetworkReply *reply) {
    //查询的昵称

    QString nickname = search_nickname_line->text();

    QMainWindow *windowU = new QMainWindow;

    windowU->setWindowTitle(nickname);

    QWidget *widgetU = new QWidget();

    QGridLayout *layoutU = new QGridLayout();

    QLabel *user_id_label = new QLabel("玩家id");
    QLabel *gender_label = new QLabel("性别");
    QLabel *level_label = new QLabel("等级");
    QLabel *strengthen_label = new QLabel("总战力");
    QLabel *create_time_label = new QLabel("账号创建时间");

    layoutU->addWidget(user_id_label, 0, 0, 1, 1);
    layoutU->addWidget(gender_label, 0, 1, 1, 1);
    layoutU->addWidget(level_label, 0, 2, 1, 1);
    layoutU->addWidget(strengthen_label, 0, 3, 1, 1);
    layoutU->addWidget(create_time_label, 0, 4, 1, 1);
    //处理返回的JSON数据
    QJsonParseError error;

    QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll(), &error);
    if (error.error == QJsonParseError::NoError) {
        if (jsonDocument.isObject()) {
            QVariantMap result = jsonDocument.toVariant().toMap();
            int i = 1;
            foreach(QVariant playerListT, result["players_info"].toList()) {
                QVariantMap playerList = playerListT.toMap();

                QLabel *user_id_content_label = new QLabel(playerList["user_id"].toString());
                QLabel *gender_content_label = new QLabel(playerList["gender"].toString());
                QLabel *level_content_label = new QLabel(playerList["level"].toString());
                QLabel *strengthen_content_label = new QLabel(playerList["strengthen_prediction_sum"].toString());
                QLabel *create_time_content_label = new QLabel(playerList["create_time"].toString());

                layoutU->addWidget(user_id_content_label, i , 0, 1, 1);
                layoutU->addWidget(gender_content_label, i , 1, 1, 1);
                layoutU->addWidget(level_content_label, i , 2, 1, 1);
                layoutU->addWidget(strengthen_content_label, i , 3, 1, 1);
                layoutU->addWidget(create_time_content_label, i , 4, 1, 1);

                i++;
            }

        }
    } else {
        qDebug()<<error.errorString();
    }

    reply->deleteLater();//最后要释放replay对象

    widgetU->setLayout(layoutU);

    windowU->setCentralWidget(widgetU);

    windowU->show();
}



//根据服务器的编号获取服务器的address
QString user::getAddressByServerNum(QString server_num) {
    return serverMap[server_num].server_address;
}

//发放头像或头像框奖励
void user::rewardUsersPortraitOrFrame() {
    QString server_num_string = server_num_box->currentText();

    //获取游戏服务器的地址
    QString server_address = getAddressByServerNum(server_num_string);

    //需要查询的ids
    QString user_ids = user_ids_line->toPlainText();

    //需要发放的ID
    QString reward_id = reward_id_line->text();

    //拼接地址
    QUrl url = "http://" + server_address + "/The_Wall/common/rewardUsers";

    QByteArray postData;

    postData.append("user_ids=" + user_ids);
    postData.append("&reward_id=" + reward_id);

    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("Content-Type","application/x-www-form-urlencoded");

    n_manager = new QNetworkAccessManager(this);
    connect(n_manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(rewardPFResult(QNetworkReply *)));
    n_manager->post(request, postData);

}

//奖励的结果
void user::rewardPFResult(QNetworkReply *reply) {
    QString result = reply->readAll();
    reward_frame_result_label->setText(result);
}
