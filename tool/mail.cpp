#include "mail.h"

mail::mail(ui *ui, QWidget *parent) {
    server_area = ui->area_box->currentText();

    setWindowTitle("mail");

    main_server_ip = ui->main_server_ip;

    //获取现在有哪些渠道组
    m_manager = new QNetworkAccessManager(this);
    connect(m_manager, &QNetworkAccessManager::finished, this, &mail::setServerInfo); //关联信号和槽

    QUrl url = "http://" + main_server_ip + "/original_server/common/serverSearch?area=" + server_area;

    m_manager->get(QNetworkRequest(url));//发送请求

    int i = 0;
    left_layout->addWidget(server_num_box, i, 0, 1, 1);
    i++;
    left_layout->addWidget(mail_title_label, i, 0, 1, 1);
    left_layout->addWidget(mail_title_line, i, 1, 1, 1);
    i++;
    left_layout->addWidget(mail_message_label, i, 0, 1, 1);
    left_layout->addWidget(mail_message_line, i, 1, 1, 1);
    i++;
    left_layout->addWidget(mail_sender_label, i, 0, 1, 1);
    left_layout->addWidget(mail_sender_line, i, 1, 1, 1);
    i++;
    left_layout->addWidget(expiry_date_label, i, 0, 1, 1);
    left_layout->addWidget(expiry_date_line, i, 1, 1, 1);
    i++;
    left_layout->addWidget(reward_id_label, i, 0, 1, 1);
    left_layout->addWidget(reward_num_label, i, 1, 1, 1);

    for (int j=0;j<4;j++) {
        QLineEdit *idEdit = new QLineEdit;
        QLineEdit *numEdit = new QLineEdit;
        rw item_i = {idEdit, numEdit};
        items_info.append(item_i);
    }

    QList<rw>::iterator k;
    for (k = items_info.begin();k!=items_info.end();k++) {
        i++;
        left_layout->addWidget(k->id, i, 0, 1, 1);
        left_layout->addWidget(k->num, i, 1, 1, 1);
    }
    i++;
    left_layout->addWidget(mail_send_ids_label, i, 0, 1, 2);
    i++;
    left_layout->addWidget(mail_send_ids_line, i, 0, 1, 2);
    i++;
    left_layout->addWidget(send_by_userid_button, i, 1, 1, 2);
    i++;
    left_layout->addWidget(limit_flag_box, i, 0, 1, 2);
    i++;
    left_layout->addWidget(send_to_all_button, i, 0, 1, 2);

    left_widget->setLayout(left_layout);

    layout->addWidget(left_widget);
    layout->addWidget(send_result_browser);

    widget->setLayout(layout);

    setCentralWidget(widget);

    connect(send_to_all_button, &QPushButton::clicked, this, &mail::sendToAll);
    connect(send_by_userid_button, &QPushButton::clicked, this, &mail::sendByUserid);
}

//发送给所有玩家
void mail::sendToAll() {
    QString server_num = server_num_box->currentText();

    QString server_ip = getAddressByServerNum(server_num);

    //将所有的按钮和输入框设为不可用
    server_num_box->setEnabled(false);
    mail_title_line->setEnabled(false);
    mail_message_line->setEnabled(false);
    mail_sender_line->setEnabled(false);
    expiry_date_line->setEnabled(false);
    QList<rw>::iterator k;
    for (k = items_info.begin();k!=items_info.end();k++) {
        k->id->setEnabled(false);
        k->num->setEnabled(false);
    }

    send_by_account_button->setEnabled(false);
    send_by_userid_button->setEnabled(false);
    limit_flag_box->setEnabled(false);
    send_to_all_button->setEnabled(false);

    //提取内容
    QString title = mail_title_line->text().trimmed();  //邮件标题
    QString message = mail_message_line->toPlainText().trimmed();   //邮件正文
    QString sender = mail_sender_line->text().trimmed();    //发件人
    QString expiry_date = expiry_date_line->text().trimmed(); //失效期
    QString limit_flag; //是否限制新玩家的领取
    if (limit_flag_box->checkState() == Qt::Checked) {
        limit_flag = "1";
    } else {
        limit_flag = "0";
    }
    QVariantList rewards;

    for (k = items_info.begin();k!=items_info.end();k++) {
        QString id = k->id->text().trimmed();
        QString num = k->num->text().trimmed();
        if (id != "" && num != "") {
            QVariantMap reward;
            reward.insert("id", id);
            reward.insert("num", num);

            rewards.append(reward);
        }
    }

    QJsonDocument jsonDocument = QJsonDocument::fromVariant(rewards);

    QString rewards_json = jsonDocument.toJson(QJsonDocument::Compact); //奖励内容

    //发送邮件
    QEventLoop loop;

    m_manager = new QNetworkAccessManager(this);

    connect(m_manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    QNetworkRequest request;

    QUrl url = "http://" + server_ip + "/The_Wall/common/mailSendAll";
    request.setUrl(url);

    QByteArray postData;
    postData.append("title=" + title);
    postData.append("&message=" + message);
    postData.append("&sender=" + sender);
    postData.append("&expiry_date=" + expiry_date);
    postData.append("&rewards=" + rewards_json);
    postData.append("&limit_flag=" + limit_flag);

    request.setRawHeader("Content-Type","application/x-www-form-urlencoded");

    QNetworkReply *reply;

    reply = m_manager->post(request, postData);

    loop.exec();

    if (reply->error() == QNetworkReply::NoError) {
        send_result_browser->append(reply->readAll());
    } else {
        send_result_browser->append("网络异常,发送失败");
    }

    m_manager->clearConnectionCache();
    m_manager->clearAccessCache();

    reply->deleteLater();//最后要释放reply对象

    //将所有的按钮和输入框设为可用
    server_num_box->setEnabled(true);
    mail_title_line->setEnabled(true);
    mail_message_line->setEnabled(true);
    mail_sender_line->setEnabled(true);
    expiry_date_line->setEnabled(true);
    for (k = items_info.begin();k!=items_info.end();k++) {
        k->id->setEnabled(true);
        k->num->setEnabled(true);
    }

    send_by_account_button->setEnabled(true);
    send_by_userid_button->setEnabled(true);
    limit_flag_box->setEnabled(true);
    send_to_all_button->setEnabled(true);
}

//发送给指定ID的玩家
void mail::sendByUserid() {
    QString server_num = server_num_box->currentText();

    QString server_ip = getAddressByServerNum(server_num);

    //将所有的按钮和输入框设为不可用
    server_num_box->setEnabled(false);
    mail_title_line->setEnabled(false);
    mail_message_line->setEnabled(false);
    mail_sender_line->setEnabled(false);
    expiry_date_line->setEnabled(false);
    QList<rw>::iterator k;
    for (k = items_info.begin();k!=items_info.end();k++) {
        k->id->setEnabled(false);
        k->num->setEnabled(false);
    }

    send_by_account_button->setEnabled(false);
    send_by_userid_button->setEnabled(false);
    limit_flag_box->setEnabled(false);
    send_to_all_button->setEnabled(false);

    //提取内容
    QString title = mail_title_line->text().trimmed();  //邮件标题
    QString message = mail_message_line->toPlainText().trimmed();   //邮件正文
    QString sender = mail_sender_line->text().trimmed();    //发件人
    QString expiry_date = expiry_date_line->text().trimmed(); //失效期
    QString user_ids = mail_send_ids_line->toPlainText().trimmed(); //收件的ids

    QVariantList rewards;

    for (k = items_info.begin();k!=items_info.end();k++) {
        QString id = k->id->text().trimmed();
        QString num = k->num->text().trimmed();
        if (id != "" && num != "") {
            QVariantMap reward;
            reward.insert("id", id);
            reward.insert("num", num);

            rewards.append(reward);
        }
    }

    QJsonDocument jsonDocument = QJsonDocument::fromVariant(rewards);

    QString rewards_json = jsonDocument.toJson(QJsonDocument::Compact); //奖励内容

    //发送邮件
    QEventLoop loop;

    m_manager = new QNetworkAccessManager(this);

    connect(m_manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    QNetworkRequest request;

    QUrl url = "http://" + server_ip + "/The_Wall/common/mailSendByUserids";
    request.setUrl(url);

    QByteArray postData;
    postData.append("title=" + title);
    postData.append("&message=" + message);
    postData.append("&sender=" + sender);
    postData.append("&expiry_date=" + expiry_date);
    postData.append("&rewards=" + rewards_json);
    postData.append("&user_ids=" + user_ids);

    request.setRawHeader("Content-Type","application/x-www-form-urlencoded");

    QNetworkReply *reply;

    reply = m_manager->post(request, postData);

    loop.exec();

    if (reply->error() == QNetworkReply::NoError) {
        send_result_browser->append(reply->readAll());
    } else {
        send_result_browser->append("网络异常,发送失败");
    }

    m_manager->clearConnectionCache();
    m_manager->clearAccessCache();

    reply->deleteLater();//最后要释放reply对象

    //将所有的按钮和输入框设为可用
    server_num_box->setEnabled(true);
    mail_title_line->setEnabled(true);
    mail_message_line->setEnabled(true);
    mail_sender_line->setEnabled(true);
    expiry_date_line->setEnabled(true);
    for (k = items_info.begin();k!=items_info.end();k++) {
        k->id->setEnabled(true);
        k->num->setEnabled(true);
    }

    send_by_account_button->setEnabled(true);
    send_by_userid_button->setEnabled(true);
    limit_flag_box->setEnabled(true);
    send_to_all_button->setEnabled(true);
}

void mail::setServerInfo(QNetworkReply *reply) {
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

    m_manager->clearConnectionCache();
    m_manager->clearAccessCache();

    reply->deleteLater();//最后要释放reply对象
}

//根据服务器的编号获取服务器的address
QString mail::getAddressByServerNum(QString server_num) {
    return serverMap[server_num].server_address;
}
