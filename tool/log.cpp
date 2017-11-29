#include "ui.h"
#include "log.h"

#include <QValidator>
#include <QEventLoop>
#include <QTableWidget>
#include <QHeaderView>
#include <QDebug>

Log::Log(Ui *ui) :
    ui(ui)
{
    //获取现在有哪些渠道组
    l_manager = new QNetworkAccessManager(this);
    connect(l_manager, &QNetworkAccessManager::finished, this, &Log::setServerInfo); //关联信号和槽

    QUrl url = "http://" + ui->main_server_ip + "/original_server/common/serverSearch?area=" + ui->area_box->currentText();

    l_manager->get(QNetworkRequest(url));//发送请求

    QDate currentDate = QDate::currentDate();
    //按钮区域-左
    int i = 0;
    left_layout->addWidget(server_num_box, i, 0, 1, 1);
    i++;
    left_layout->addWidget(user_id_label, i, 0, 1, 1);
    user_id_line->setValidator(num_validator);
    left_layout->addWidget(user_id_line, i, 1, 1, 1);
    i++;
    left_layout->addWidget(start_date_label, i, 0, 1, 1);
    left_layout->addWidget(start_date_edit, i, 1, 1, 1);
    start_date_edit->setDate(currentDate); //设置时间
    start_date_edit->setMinimumDate(QDate(2017, 10, 1));
    start_date_edit->setMaximumDate(currentDate); //设置最大时间
    i++;
    left_layout->addWidget(end_date_label, i, 0, 1, 1);
    left_layout->addWidget(end_date_edit, i, 1, 1, 1);
    end_date_edit->setDate(currentDate); //设置时间
    end_date_edit->setMinimumDate(QDate(2017, 10, 1));
    end_date_edit->setMaximumDate(currentDate); //设置最大时间

    button_layout->addWidget(login_log_button);
    button_layout->addWidget(stone_log_button);
    button_layout->addWidget(pve_log_button);
    button_layout->addWidget(pvp_log_button);
    button_layout->addWidget(season_log_button);

//    layout->addWidget();

    left_widget->setLayout(left_layout);
    button_widget->setLayout(button_layout);

    //主区域
    layout->addWidget(left_widget);
    layout->addWidget(button_widget);
    layout->addWidget(result_browser);

    widget->setLayout(layout);

    setCentralWidget(widget);

    //时间防傻设定
    connect(start_date_edit, &QDateEdit::dateChanged, this, &Log::timeChanged);
    connect(end_date_edit, &QDateEdit::dateChanged, this, &Log::timeChanged);

    //按钮
    connect(login_log_button, &QPushButton::clicked, this, &Log::loginLogInquire);
    connect(stone_log_button, &QPushButton::clicked, this, &Log::stoneLogInquire);
    connect(pve_log_button, &QPushButton::clicked, this, &Log::pveLogInquire);
    connect(pvp_log_button, &QPushButton::clicked, this, &Log::pvpLogInquire);
    connect(season_log_button, &QPushButton::clicked, this, &Log::seasonLogInquire);
}

void Log::loginLogInquire()
{
    //获取user_id
    QString userId = user_id_line->text().trimmed();

    if (userId == "") {
        result_browser->append("<div style=\"color:#FF0000\">玩家id不能为空</div>");
        return;
    }

    //首先去下载LOG
    QEventLoop loop;
    l_manager = new QNetworkAccessManager(this);

    QDate start_date = start_date_edit->date();
    QDate end_date = end_date_edit->date();
    QString start_date_string = QString::number(start_date.year(), 10) + "-" + QString::number(start_date.month(), 10) + "-" + QString::number(start_date.day(), 10);
    QString end_date_string = QString::number(end_date.year(), 10) + "-" + QString::number(end_date.month(), 10) + "-" + QString::number(end_date.day(), 10);

    QNetworkRequest request;
    QNetworkReply *reply;
    QUrl url = "http://" + getAddressByServerNum(server_num_box->currentText()) + "/The_Wall/common/logFiles?user_id=" + userId + "&type=1&start_date=" + start_date_string + "&end_date=" + end_date_string;
//        QUrl url = "http://localhost/The_Wall/common/logFiles?user_id=" + userId + "&type=1&start_date=" + start_date_string + "&end_date=" + end_date_string;

    request.setUrl(url);
    connect(l_manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    reply = l_manager->get(QNetworkRequest(url));

    result_browser->append("正在检测日志中..");

    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        result_browser->append("日志检测失败,请联系管理员..");
        return;
    }

    //解析JSON
    QJsonParseError js_err;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll(), &js_err);
    if (js_err.error != QJsonParseError::NoError) {
        result_browser->append("日志数据检测失败,请联系管理员..");
        return;
    }
    QVariantMap js_result = jsonDocument.toVariant().toMap();
    QList <QString> log_file_list;
    int log_num = 0;
    foreach (QVariant log_file, js_result["log_files"].toList()) {
        QString log_file_s = log_file.toString();
        log_file_list.append(log_file_s);
        log_num++;
    }

    if (log_num == 0) {
        result_browser->append("无该玩家的相关日志");
        return;
    }
    result_browser->append("检测到该玩家的相关日志" + QString::number(log_num, 10) + "篇,准备下载");

    l_manager->clearConnectionCache();
    l_manager->clearAccessCache();

    reply->deleteLater();//最后要释放reply对象

    //生成新的窗口记录查询的日志
    QMainWindow *window = new QMainWindow;

    QTableWidget *w_widget = new QTableWidget;
    w_widget->setColumnCount(2);
    w_widget->setHorizontalHeaderLabels(QStringList()<<"日期"<<"IP");

    w_widget->verticalHeader()->setVisible(false);

    window->setCentralWidget(w_widget);

    window->setWindowTitle(userId);

    int row = 0;
    //开始循环解析日志
    foreach (QString log_file, log_file_list) {
        QNetworkRequest request;
        QNetworkReply *reply;

        request.setUrl(QUrl("http://" + getAddressByServerNum(server_num_box->currentText()) + "/The_Wall/common/logDownload?log_path=" + log_file));

        connect(l_manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
        reply = l_manager->get(request);

        loop.exec();

        QString log_string = reply->readAll();

        QString log_time = "";
        QString log_cut = "";
        QString key = "";
        QString value = "";
        int i;
        int time_flag = 0;  //是否是时间字段的FLAG
        int log_size = log_string.size();

        //设置w_widget列的长度
        for (i = 0;i < log_size; i++) {

            QString v = log_string.at(i);

            if (v == "[") {
                w_widget->insertRow(row);
                time_flag = 1;
            } else if (v == "]") {
                time_flag = 0;
                w_widget->setItem(row, 0, new QTableWidgetItem(log_time));
            } else if (v == "=") {
                key = log_cut;
                log_cut = "";
            } else if (v == "," || v == "\xd" || v == "\n") {
                value = log_cut;
                log_cut = "";
                if (key == "ip") {
                    w_widget->setItem(row, 1, new QTableWidgetItem(value));
                }
                if (v == "\n") {
                    log_time = "";
                    row++;
                }
            } else if (v == "\xa") {
                log_time = "";
                row++;
            } else {
                if (time_flag) {
                    log_time += v;
                } else {
                    log_cut += v;
                }
            }
        }

        l_manager->clearConnectionCache();
        l_manager->clearAccessCache();

        reply->deleteLater();
    }

    window->show();
}

//符石消费LOG下载解析
void Log::stoneLogInquire()
{
    //获取user_id
    QString userId = user_id_line->text().trimmed();

    if (userId == "") {
        result_browser->append("<div style=\"color:#FF0000\">玩家id不能为空</div>");
        return;
    }

    //首先去下载LOG
    QEventLoop loop;
    l_manager = new QNetworkAccessManager(this);

    QDate start_date = start_date_edit->date();
    QDate end_date = end_date_edit->date();
    QString start_date_string = QString::number(start_date.year(), 10) + "-" + QString::number(start_date.month(), 10) + "-" + QString::number(start_date.day(), 10);
    QString end_date_string = QString::number(end_date.year(), 10) + "-" + QString::number(end_date.month(), 10) + "-" + QString::number(end_date.day(), 10);

    QNetworkRequest request;
    QNetworkReply *reply;
//    QUrl url = "http://" + getAddressByServerNum(server_num_box->currentText()) + "/The_Wall/common/logFiles?user_id=" + userId + "&type=1&start_date=" + start_date_string + "&end_date=" + end_date_string;
        QUrl url = "http://" + getAddressByServerNum(server_num_box->currentText()) + "/The_Wall/common/logFiles?user_id=" + userId + "&type=2&start_date=" + start_date_string + "&end_date=" + end_date_string;
    request.setUrl(url);
    connect(l_manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    reply = l_manager->get(QNetworkRequest(url));

    result_browser->append("正在检测日志中..");

    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        result_browser->append("日志检测失败,请联系管理员..");
        return;
    }

    //解析JSON
    QJsonParseError js_err;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll(), &js_err);
    if (js_err.error != QJsonParseError::NoError) {
        result_browser->append("日志数据检测失败,请联系管理员..");
        return;
    }
    QVariantMap js_result = jsonDocument.toVariant().toMap();
    QList <QString> log_file_list;
    int log_num = 0;
    foreach (QVariant log_file, js_result["log_files"].toList()) {
        QString log_file_s = log_file.toString();
        log_file_list.append(log_file_s);
        log_num++;
    }

    if (log_num == 0) {
        result_browser->append("无该玩家的相关日志");
        return;
    }
    result_browser->append("检测到该玩家的相关日志" + QString::number(log_num, 10) + "篇,准备下载");

    l_manager->clearConnectionCache();
    l_manager->clearAccessCache();

    reply->deleteLater();//最后要释放reply对象

    //生成新的窗口记录查询的日志
    QMainWindow *window = new QMainWindow;

    QTableWidget *w_widget = new QTableWidget;
    w_widget->setColumnCount(5);
    w_widget->setHorizontalHeaderLabels(QStringList()<<"日期"<<"数量"<<"状态"<<"类型"<<"剩余符石");

    w_widget->verticalHeader()->setVisible(false);

    window->setCentralWidget(w_widget);

    window->setWindowTitle(userId);

    int row = 0;
    //开始循环解析日志
    foreach (QString log_file, log_file_list) {
        QNetworkRequest request;
        QNetworkReply *reply;

        request.setUrl(QUrl("http://" + getAddressByServerNum(server_num_box->currentText()) + "/The_Wall/common/logDownload?log_path=" + log_file));

        connect(l_manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
        reply = l_manager->get(request);

        loop.exec();

        QString log_string = reply->readAll();

        QString log_time = "";
        QString log_cut = "";
        QString key = "";
        QString value = "";
        int i;
        int time_flag = 0;  //是否是时间字段的FLAG
        int log_size = log_string.size();

        //设置w_widget列的长度
        for (i = 0;i < log_size; i++) {

            QString v = log_string.at(i);

            if (v == "[") {
                w_widget->insertRow(row);
                time_flag = 1;
            } else if (v == "]") {
                time_flag = 0;
                w_widget->setItem(row, 0, new QTableWidgetItem(log_time));
            } else if (v == "=") {
                key = log_cut;
                log_cut = "";
            } else if (v == "," || v == "\xd" || v == "\n") {
                value = log_cut;
                log_cut = "";

                if (key == "num") {
                    w_widget->setItem(row, 1, new QTableWidgetItem(value));
                } else if (key == "flag") {
                    QString flag = "消费";;
                    if (value == "0") {
                        flag = "入账";
                    }
                    w_widget->setItem(row, 2, new QTableWidgetItem(flag));
                } else if (key == "type") {
                    QString stone_type;
                    switch (value.toInt()) {
                        case 1:
                            stone_type = "抽卡";
                            break;
                        case 2:
                            stone_type = "补签";
                            break;
                        case 3:
                            stone_type = "商店购物";
                            break;
                        case 4:
                            stone_type = "竞技场挑战CD";
                            break;
                        case 5:
                            stone_type = "竞技场挑战次数";
                            break;
                        case 6:
                            stone_type = "赛季挑战CD";
                            break;
                        case 7:
                            stone_type = "赛季挑战次数";
                            break;
                        case 8:
                            stone_type = "重置精英副本";
                            break;
                        case 9:
                            stone_type = "体力购买";
                            break;
                        case 10:
                            stone_type = "技能点恢复";
                            break;
                        case 11:
                            stone_type = "刷新商店";
                            break;
                        case 12:
                            stone_type = "商城购物";
                            break;
                        case 13:
                            stone_type = "刷新兑换所";
                            break;
                        case 14:
                            stone_type = "成长基金";
                            break;
                        case 15:
                            stone_type = "改名";
                            break;
                        default:
                            stone_type = "\\";
                            break;
                    }

                    w_widget->setItem(row, 3, new QTableWidgetItem(stone_type));

                } else if (key == "remain") {
                    w_widget->setItem(row, 4, new QTableWidgetItem(value));
                }

                if (v == "\n") {
                    log_time = "";
                    row++;
                }
            } else if (v == "\xa") {
                log_time = "";
                row++;
            } else {
                if (time_flag) {
                    log_time += v;
                } else {
                    log_cut += v;
                }
            }
        }

        l_manager->clearConnectionCache();
        l_manager->clearAccessCache();

        reply->deleteLater();
    }

    window->show();
}

//PVE LOG下载解析
void Log::pveLogInquire()
{
    //获取user_id
    QString userId = user_id_line->text().trimmed();

    if (userId == "") {
        result_browser->append("<div style=\"color:#FF0000\">玩家id不能为空</div>");
        return;
    }

    //首先去下载LOG
    QEventLoop loop;
    l_manager = new QNetworkAccessManager(this);

    QDate start_date = start_date_edit->date();
    QDate end_date = end_date_edit->date();
    QString start_date_string = QString::number(start_date.year(), 10) + "-" + QString::number(start_date.month(), 10) + "-" + QString::number(start_date.day(), 10);
    QString end_date_string = QString::number(end_date.year(), 10) + "-" + QString::number(end_date.month(), 10) + "-" + QString::number(end_date.day(), 10);

    QNetworkRequest request;
    QNetworkReply *reply;
//    QUrl url = "http://" + getAddressByServerNum(server_num_box->currentText()) + "/The_Wall/common/logFiles?user_id=" + userId + "&type=1&start_date=" + start_date_string + "&end_date=" + end_date_string;
        QUrl url = "http://" + getAddressByServerNum(server_num_box->currentText()) + "/The_Wall/common/logFiles?user_id=" + userId + "&type=3&start_date=" + start_date_string + "&end_date=" + end_date_string;
    request.setUrl(url);
    connect(l_manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    reply = l_manager->get(QNetworkRequest(url));

    result_browser->append("正在检测日志中..");

    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        result_browser->append("日志检测失败,请联系管理员..");
        return;
    }

    //解析JSON
    QJsonParseError js_err;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll(), &js_err);
    if (js_err.error != QJsonParseError::NoError) {
        result_browser->append("日志数据检测失败,请联系管理员..");
        return;
    }
    QVariantMap js_result = jsonDocument.toVariant().toMap();
    QList <QString> log_file_list;
    int log_num = 0;
    foreach (QVariant log_file, js_result["log_files"].toList()) {
        QString log_file_s = log_file.toString();
        log_file_list.append(log_file_s);
        log_num++;
    }

    if (log_num == 0) {
        result_browser->append("无该玩家的相关日志");
        return;
    }
    result_browser->append("检测到该玩家的相关日志" + QString::number(log_num, 10) + "篇,准备下载");

    l_manager->clearConnectionCache();
    l_manager->clearAccessCache();

    reply->deleteLater();//最后要释放reply对象

    //生成新的窗口记录查询的日志
    QMainWindow *window = new QMainWindow;

    QTableWidget *w_widget = new QTableWidget;
    w_widget->setColumnCount(3);
    w_widget->setHorizontalHeaderLabels(QStringList()<<"日期"<<"副本ID"<<"行为");

    w_widget->verticalHeader()->setVisible(false);

    window->setCentralWidget(w_widget);

    window->setWindowTitle(userId);

    int row = 0;
    //开始循环解析日志
    foreach (QString log_file, log_file_list) {
        QNetworkRequest request;
        QNetworkReply *reply;

        request.setUrl(QUrl("http://" + getAddressByServerNum(server_num_box->currentText()) + "/The_Wall/common/logDownload?log_path=" + log_file));

        connect(l_manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
        reply = l_manager->get(request);

        loop.exec();

        QString log_string = reply->readAll();

        QString log_time = "";
        QString log_cut = "";
        QString key = "";
        QString value = "";
        int i;
        int time_flag = 0;  //是否是时间字段的FLAG
        int log_size = log_string.size();

        //设置w_widget列的长度
        for (i = 0;i < log_size; i++) {

            QString v = log_string.at(i);
            if (v == "[") {
                w_widget->insertRow(row);
                time_flag = 1;
            } else if (v == "]") {
                time_flag = 0;
                w_widget->setItem(row, 0, new QTableWidgetItem(log_time));
            } else if (v == "=") {
                key = log_cut;
                log_cut = "";
            } else if (v == "," || v == "\xd" || v == "\n") {
                value = log_cut;
                log_cut = "";
                if (key == "did") {
                    w_widget->setItem(row, 1, new QTableWidgetItem(value));
                } else if (key == "type") {
                    QString dungeon_type;
                    switch (value.toInt()) {
                        case 1:
                            dungeon_type = "开始";
                            break;
                        case 2:
                            dungeon_type = "完成";
                            break;
                        case 3:
                            dungeon_type = "扫荡";
                            break;
                        case 4:
                            dungeon_type = "复活";
                            break;
                        case 5:
                            dungeon_type = "放弃";
                            break;
                        default:
                            dungeon_type = "\\";
                            break;
                    }
                    w_widget->setItem(row, 2, new QTableWidgetItem(dungeon_type));
                }

                if (v == "\n") {
                    log_time = "";
                    row++;
                }
            } else if (v == "\xa") {
                log_time = "";
                row++;
            } else {
                if (time_flag) {
                    log_time += v;
                } else {
                    log_cut += v;
                }
            }
        }

        l_manager->clearConnectionCache();
        l_manager->clearAccessCache();

        reply->deleteLater();
    }

    window->show();
}

//pvp的日志下载解析
void Log::pvpLogInquire()
{
    //获取user_id
    QString userId = user_id_line->text().trimmed();

    if (userId == "") {
        result_browser->append("<div style=\"color:#FF0000\">玩家id不能为空</div>");
        return;
    }

    //首先去下载LOG
    QEventLoop loop;
    l_manager = new QNetworkAccessManager(this);

    QDate start_date = start_date_edit->date();
    QDate end_date = end_date_edit->date();
    QString start_date_string = QString::number(start_date.year(), 10) + "-" + QString::number(start_date.month(), 10) + "-" + QString::number(start_date.day(), 10);
    QString end_date_string = QString::number(end_date.year(), 10) + "-" + QString::number(end_date.month(), 10) + "-" + QString::number(end_date.day(), 10);

    QNetworkRequest request;
    QNetworkReply *reply;
//    QUrl url = "http://" + getAddressByServerNum(server_num_box->currentText()) + "/The_Wall/common/logFiles?user_id=" + userId + "&type=1&start_date=" + start_date_string + "&end_date=" + end_date_string;
        QUrl url = "http://" + getAddressByServerNum(server_num_box->currentText()) + "/The_Wall/common/pvpLog?user_id=" + userId + "&type=1&start_date=" + start_date_string + "&end_date=" + end_date_string;
    request.setUrl(url);
    connect(l_manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    reply = l_manager->get(QNetworkRequest(url));

    result_browser->append("正在检测日志中..");

    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        result_browser->append("日志检测失败,请联系管理员..");
        return;
    }

    //解析JSON
    QJsonParseError js_err;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll(), &js_err);
    if (js_err.error != QJsonParseError::NoError) {
        result_browser->append("日志数据检测失败,请联系管理员..");
        return;
    }

    //生成新的窗口记录查询的日志
    QMainWindow *window = new QMainWindow;

    QTableWidget *w_widget = new QTableWidget;
    w_widget->setColumnCount(6);
    w_widget->setHorizontalHeaderLabels(QStringList()<<"日期"<<"战斗状态"<<"对手"<<"结果"<<"排名"<<"分数变动");

    w_widget->verticalHeader()->setVisible(false);

    window->setCentralWidget(w_widget);

    window->setWindowTitle(userId);

    int row = 0;

    QVariantMap js_result = jsonDocument.toVariant().toMap();
    foreach (QVariant log_list, js_result["logs"].toList()) {
        QVariantMap log = log_list.toMap();
        w_widget->insertRow(row);

        w_widget->setItem(row, 0, new QTableWidgetItem(log["create_time"].toString()));
        //战斗状态
        QString attack_flag;
        if(log["attack_flag"].toInt() == 1) {
            attack_flag = "进攻";
        } else {
            attack_flag = "防守";
        }
        w_widget->setItem(row, 1, new QTableWidgetItem(attack_flag));
        w_widget->setItem(row, 2, new QTableWidgetItem(log["enemy_nickname"].toString()));
        //战斗结果
        QString pvp_result;
        switch (log["result"].toInt()) {
            case 1:
                pvp_result = "胜";
                break;
            case 0:
                pvp_result = "负";
                break;
            default:
                pvp_result = "平";
                break;
        }
        w_widget->setItem(row, 3, new QTableWidgetItem(pvp_result));
        QString rank;
        switch (log["rank"].toInt()) {
            case 0:
                rank = "暂无排名";
                break;
            default:
                rank = log["rank"].toString();
                break;
        }
        w_widget->setItem(row, 4, new QTableWidgetItem(rank));
        w_widget->setItem(row, 5, new QTableWidgetItem(log["score"].toString()));

        row++;
    }

    if (row == 0) {
        result_browser->append("无该玩家的相关日志");
        return;
    }

    l_manager->clearConnectionCache();
    l_manager->clearAccessCache();

    reply->deleteLater();//最后要释放reply对象

    window->show();
}

//赛季的日志下载解析
void Log::seasonLogInquire()
{
    //获取user_id
    QString userId = user_id_line->text().trimmed();

    if (userId == "") {
        result_browser->append("<div style=\"color:#FF0000\">玩家id不能为空</div>");
        return;
    }

    //首先去下载LOG
    QEventLoop loop;
    l_manager = new QNetworkAccessManager(this);

    QDate start_date = start_date_edit->date();
    QDate end_date = end_date_edit->date();
    QString start_date_string = QString::number(start_date.year(), 10) + "-" + QString::number(start_date.month(), 10) + "-" + QString::number(start_date.day(), 10);
    QString end_date_string = QString::number(end_date.year(), 10) + "-" + QString::number(end_date.month(), 10) + "-" + QString::number(end_date.day(), 10);

    QNetworkRequest request;
    QNetworkReply *reply;
//    QUrl url = "http://" + getAddressByServerNum(server_num_box->currentText()) + "/The_Wall/common/logFiles?user_id=" + userId + "&type=1&start_date=" + start_date_string + "&end_date=" + end_date_string;
        QUrl url = "http://" + getAddressByServerNum(server_num_box->currentText()) + "/The_Wall/common/pvpLog?user_id=" + userId + "&type=2&start_date=" + start_date_string + "&end_date=" + end_date_string;
    request.setUrl(url);
    connect(l_manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    reply = l_manager->get(QNetworkRequest(url));

    result_browser->append("正在检测日志中..");

    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        result_browser->append("日志检测失败,请联系管理员..");
        return;
    }

    //解析JSON
    QJsonParseError js_err;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll(), &js_err);
    if (js_err.error != QJsonParseError::NoError) {
        result_browser->append("日志数据检测失败,请联系管理员..");
        return;
    }

    //生成新的窗口记录查询的日志
    QMainWindow *window = new QMainWindow;

    QTableWidget *w_widget = new QTableWidget;
    w_widget->setColumnCount(6);
    w_widget->setHorizontalHeaderLabels(QStringList()<<"日期"<<"赛季编号"<<"战斗状态"<<"对手"<<"结果"<<"排名"<<"分数变动");

    w_widget->verticalHeader()->setVisible(false);

    window->setCentralWidget(w_widget);

    window->setWindowTitle(userId);

    int row = 0;

    QVariantMap js_result = jsonDocument.toVariant().toMap();
    foreach (QVariant log_list, js_result["logs"].toList()) {
        QVariantMap log = log_list.toMap();
        w_widget->insertRow(row);

        w_widget->setItem(row, 0, new QTableWidgetItem(log["create_time"].toString()));
        w_widget->setItem(row, 1, new QTableWidgetItem(log["season_no"].toString()));
        //战斗状态
        QString attack_flag;
        if(log["attack_flag"].toInt() == 1) {
            attack_flag = "进攻";
        } else {
            attack_flag = "防守";
        }
        w_widget->setItem(row, 2, new QTableWidgetItem(attack_flag));
        w_widget->setItem(row, 3, new QTableWidgetItem(log["enemy_nickname"].toString()));
        //战斗结果
        QString pvp_result;
        switch (log["result"].toInt()) {
            case 1:
                pvp_result = "胜";
                break;
            case 0:
                pvp_result = "负";
                break;
            default:
                pvp_result = "平";
                break;
        }
        w_widget->setItem(row, 4, new QTableWidgetItem(pvp_result));
        QString rank;
        switch (log["rank"].toInt()) {
            case 0:
                rank = "暂无排名";
                break;
            default:
                rank = log["rank"].toString();
                break;
        }
        w_widget->setItem(row, 5, new QTableWidgetItem(rank));
        w_widget->setItem(row, 6, new QTableWidgetItem(log["score"].toString()));

        row++;
    }

    if (row == 0) {
        result_browser->append("无该玩家的相关日志");
        return;
    }

    l_manager->clearConnectionCache();
    l_manager->clearAccessCache();

    reply->deleteLater();//最后要释放reply对象

    window->show();
}

//void Log::updateLog()
//{
//    //获取user_id
//    QString userId = user_id_line->text().trimmed();

//    if (userId == "") {
//        result_browser->append("<div style=\"color:#FF0000\">玩家id不能为空</div>");
//        return;
//    }

//    l_manager = new QNetworkAccessManager(this);

//    QNetworkRequest request;
//    QUrl url = "http://" + getAddressByServerNum(server_num_box->currentText()) + "/The_Wall/common/logDownload?user_id=" + userId + "&type=";

//    request.setUrl(url);

//}

//void Log::dealWithRawLogKV(QString key, QString value) {
//    if (key == "ip") {
//        result_browser->append("登录ip: " + value);
//    }
//}

void Log::timeChanged(const QDate &date) {
    if (start_date_edit->date() > end_date_edit->date()) {
        start_date_edit->setDate(end_date_edit->date());
    }
}

void Log::setServerInfo(QNetworkReply *reply) {
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
QString Log::getAddressByServerNum(QString server_num) {
    return serverMap[server_num].server_address;
}
