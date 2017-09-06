#include "season.h"

season::season(ui *ui, QWidget *parent){
    server_area = ui->area_box->currentText();

    setWindowTitle(server_area);

    main_server_ip = ui->main_server_ip;

    //获取现在有哪些渠道组
    s_manager = new QNetworkAccessManager(this);
    connect(s_manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(setServerInfo(QNetworkReply *))); //关联信号和槽

    QUrl url = "http://" + main_server_ip + "/original_server/common/serverSearch?area=" + server_area;

    s_manager->get(QNetworkRequest(url));//发送请求

    int i = 0;
    layout->addWidget(server_num_box, i, 0, 1, 1);
    i++;
    layout->addWidget(season_label, i, 0, 1, 2, Qt::AlignCenter);
    i++;
    layout->addWidget(season_no_label, i, 0, 1, 1);
    layout->addWidget(season_no_line, i, 1, 1, 1);
    i++;
    layout->addWidget(submit_cal_button, i, 0, 1, 1);
    i++;
    layout->addWidget(season_cal_history_label, i, 0, 1, 1);
    layout->addWidget(season_history_button, i, 1, 1, 1);
    i++;
    layout->addWidget(season_cal_history_browser, i, 0, 1, 2);

    widget->setLayout(layout);

    setCentralWidget(widget);

    connect(season_history_button, &QPushButton::clicked, this, &season::searchSeasonCalHistory);
    connect(submit_cal_button, &QPushButton::clicked, this, &season::calSeason);
}

//查询结算历史
void season::searchSeasonCalHistory() {
    QString server_num = server_num_box->currentText();

    QString server_ip = getAddressByServerNum(server_num);

    QUrl url = "http://" + server_ip + "/The_Wall/common/seasonHistory";

    s_manager = new QNetworkAccessManager(this);

    connect(s_manager, &QNetworkAccessManager::finished, this, &season::seasonHistoryShow);

    s_manager->get(QNetworkRequest(url));
}

//结算历史显示
void season::seasonHistoryShow(QNetworkReply *reply) {
    QJsonParseError error;

    if (reply->error() == QNetworkReply::NoError) {
        QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll(), &error);

        if (error.error == QJsonParseError::NoError) {
            if (jsonDocument.isObject()) {
                 season_cal_history_browser->append("查询成功:");

                QVariantMap result = jsonDocument.toVariant().toMap();
                foreach(QVariant serverHistoryT, result["season_history"].toList()) {
                    QVariantMap serverHistory = serverHistoryT.toMap();

                    season_cal_history_browser->append("赛季编号: " + serverHistory["season_no"].toString() + "\n结算时间: " + serverHistory["create_time"].toString());
                }
            }
        }
    } else {
        season_cal_history_browser->append("查询失败");
    }

    s_manager->clearConnectionCache();
    s_manager->clearAccessCache();

    reply->deleteLater();//最后要释放reply对象
}

//结算赛季
void season::calSeason() {
    QString server_num = server_num_box->currentText();

    QString season_no = season_no_line->text();

    QString server_ip = getAddressByServerNum(server_num);

    QUrl url = "http://" + server_ip + "/The_Wall/common/seasonCal?season_no=" + season_no;

    s_manager = new QNetworkAccessManager(this);

    connect(s_manager, &QNetworkAccessManager::finished, this, &season::seasonCalResultShow);

    s_manager->get(QNetworkRequest(url));
}

//结算结果显示
void season::seasonCalResultShow(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QString result = QString(reply->readAll());
        qDebug() << result;
        if (result == "success") {
            season_cal_history_browser->append("结算成功");
        } else {
            season_cal_history_browser->append(result);
        }
    } else {
        season_cal_history_browser->append("结算失败");
    }


    s_manager->clearConnectionCache();
    s_manager->clearAccessCache();

    reply->deleteLater();//最后要释放reply对象
}

void season::setServerInfo(QNetworkReply *reply) {
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

    s_manager->clearConnectionCache();
    s_manager->clearAccessCache();

    reply->deleteLater();//最后要释放reply对象
}

//根据服务器的编号获取服务器的address
QString season::getAddressByServerNum(QString server_num) {
    return serverMap[server_num].server_address;
}
