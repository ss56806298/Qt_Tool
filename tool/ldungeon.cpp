#include "ldungeon.h"

ldungeon::ldungeon(Ui *ui, QWidget *parent) {
    server_area = ui->area_box->currentText();

    setWindowTitle(server_area);

    main_server_ip = ui->main_server_ip;

    //获取现在有哪些渠道组
    l_manager = new QNetworkAccessManager(this);
    connect(l_manager, &QNetworkAccessManager::finished, this, ldungeon::setServerInfo);
    QUrl url = "http://" + main_server_ip + "/original_server/common/serverSearch?area=" + server_area;
    l_manager->get(QNetworkRequest(url));//发送请求

    int i = 0;
    layout->addWidget(server_num_box, i, 0, 1, 1);
    i++;
    layout->addWidget(ldungeon_label, i, 0, 1, 2, Qt::AlignCenter);
    i++;
    layout->addWidget(ldungeon_no_label, i, 0, 1, 1);
    layout->addWidget(ldungeon_no_line, i, 1, 1, 1);
    i++;
    layout->addWidget(ldungeon_frame_label, i, 0, 1, 1);
    layout->addWidget(ldungeon_frame_line, i, 1, 1, 1);
    i++;
    layout->addWidget(submit_cal_button, i, 0, 1, 1);
    i++;
    layout->addWidget(ldungeon_cal_history_label, i, 0, 1, 1);
    layout->addWidget(ldungeon_history_button, i, 1, 1, 1);
    i++;
    layout->addWidget(ldungeon_cal_history_browser, i, 0, 1, 2);

    widget->setLayout(layout);

    setCentralWidget(widget);

    connect(ldungeon_history_button, &QPushButton::clicked, this, &ldungeon::searchLdungeonCalHistory);
    connect(submit_cal_button, &QPushButton::clicked, this, &ldungeon::calLdungeon);
}

//查询结算历史
void ldungeon::searchLdungeonCalHistory() {
    QString server_num = server_num_box->currentText();

    QString server_ip = getAddressByServerNum(server_num);

    QUrl url = "http://" + server_ip + "/The_Wall/common/ldungeonHistory";

    l_manager = new QNetworkAccessManager(this);

    connect(l_manager, &QNetworkAccessManager::finished, this, &ldungeon::ldungeonHistoryShow);

    l_manager->get(QNetworkRequest(url));
}

//结算历史显示
void ldungeon::ldungeonHistoryShow(QNetworkReply *reply) {
    QJsonParseError error;

    if (reply->error() == QNetworkReply::NoError) {
        QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll(), &error);
        ldungeon_cal_history_browser->append("查询成功:");
        if (error.error == QJsonParseError::NoError) {
            if (jsonDocument.isObject()) {

                QVariantMap result = jsonDocument.toVariant().toMap();
                foreach(QVariant serverHistoryT, result["ldungeon_history"].toList()) {
                    QVariantMap serverHistory = serverHistoryT.toMap();

                    ldungeon_cal_history_browser->append("降临编号: " + serverHistory["area_id"].toString() + "\n结算时间: " + serverHistory["create_time"].toString());
                }
            }
        }
    } else {
        ldungeon_cal_history_browser->append("查询失败");
    }

    l_manager->clearConnectionCache();
    l_manager->clearAccessCache();

    reply->deleteLater();//最后要释放reply对象
}

//结算降临
void ldungeon::calLdungeon() {
    QString server_num = server_num_box->currentText();

    QString area_id = ldungeon_no_line->text();
    QString frame_id = ldungeon_frame_line->text();

    QString server_ip = getAddressByServerNum(server_num);

    QUrl url = "http://" + server_ip + "/The_Wall/common/ldungeonCal?area_id=" + area_id + "&frame_id=" + frame_id;

    l_manager = new QNetworkAccessManager(this);

    connect(l_manager, &QNetworkAccessManager::finished, this, &ldungeon::ldungeonCalResultShow);

    l_manager->get(QNetworkRequest(url));
}

//结算结果显示
void ldungeon::ldungeonCalResultShow(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QString result = QString(reply->readAll());
        ldungeon_cal_history_browser->append(result);
    } else {
        ldungeon_cal_history_browser->append("结算失败");
    }


    l_manager->clearConnectionCache();
    l_manager->clearAccessCache();

    reply->deleteLater();//最后要释放reply对象
}


//获取该渠道下的服务器信息
void ldungeon::setServerInfo(QNetworkReply *reply) {
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

    l_manager->clearConnectionCache();
    l_manager->clearAccessCache();

    reply->deleteLater();//最后要释放reply对象
}

//根据服务器的编号获取服务器的address
QString ldungeon::getAddressByServerNum(QString server_num) {
    return serverMap[server_num].server_address;
}
