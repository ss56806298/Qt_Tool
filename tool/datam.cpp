#include "datam.h"
#include <QEventLoop>
#include <QTableWidget>
#include <QHeaderView>

datam::datam(Ui *ui, QWidget *parent) {
    server_area = ui->area_box->currentText();

    setWindowTitle("data_manager");

    main_server_ip = ui->main_server_ip;

    //获取现在有哪些渠道组
    d_manager = new QNetworkAccessManager(this);
    connect(d_manager, &QNetworkAccessManager::finished, this, &datam::setServerInfo); //关联信号和槽

    QUrl url = "http://" + main_server_ip + "/original_server/common/serverSearch?area=" + server_area;

    d_manager->get(QNetworkRequest(url));//发送请求

    //人物信息
    int i = 0;
//    game_master_layout->addLayout(user_id_label, i, 0, 1, 1);
//    game_master_layout->addLayout(user_id_line, i, 1, 1, 1);
    game_master_layout->addWidget(game_master_label, i, 0, 1, 2);
    i++;
    game_master_layout->addWidget(game_master_level_label, i, 0, 1, 1);
    game_master_layout->addWidget(game_master_level_line, i, 1, 1, 1);
    game_master_level_line->setValidator(num_validator);
    i++;
    game_master_layout->addWidget(game_master_nickname_label, i, 0, 1, 1);
    game_master_layout->addWidget(game_master_nickname_line, i, 1, 1, 1);
    i++;
    game_master_layout->addWidget(game_master_exp_label, i, 0, 1, 1);
    game_master_layout->addWidget(game_master_exp_line, i, 1, 1, 1);
    game_master_exp_line->setValidator(num_validator);
    i++;
    game_master_layout->addWidget(game_master_stone_label, i, 0, 1, 1);
    game_master_layout->addWidget(game_master_stone_line, i, 1, 1, 1);
    game_master_stone_line->setValidator(num_validator);
    i++;
    game_master_layout->addWidget(game_master_coin_label, i, 0, 1, 1);
    game_master_layout->addWidget(game_master_coin_line, i, 1, 1, 1);
    game_master_coin_line->setValidator(num_validator);
    i++;
    game_master_layout->addWidget(game_master_stt_label, i, 0, 1, 1);
    game_master_layout->addWidget(game_master_stt_line, i, 1, 1, 1);
    game_master_stt_line->setValidator(num_validator);
    i++;
    game_master_layout->addWidget(game_master_lpt_label, i, 0, 1, 1);
    game_master_layout->addWidget(game_master_lpt_line, i, 1, 1, 1);
    game_master_lpt_line->setValidator(num_validator);
    i++;
    game_master_layout->addWidget(game_master_gdp_label, i, 0, 1, 1);
    game_master_layout->addWidget(game_master_gdp_line, i, 1, 1, 1);
    game_master_gdp_line->setValidator(num_validator);

    game_master_level_line->setEnabled(false);

    //人物属性信息
    i=0;
    game_master_status_layout->addWidget(game_master_hp_label, i, 0, 1, 1);
    game_master_status_layout->addWidget(game_master_hp_line, i, 1, 1, 1);
    i++;
    game_master_status_layout->addWidget(game_master_attack_label, i, 0, 1, 1);
    game_master_status_layout->addWidget(game_master_attack_line, i, 1, 1, 1);
    i++;
    game_master_status_layout->addWidget(game_master_defense_label, i, 0, 1, 1);
    game_master_status_layout->addWidget(game_master_defense_line, i, 1, 1, 1);
    i++;
    game_master_status_layout->addWidget(game_master_stamina_label, i, 0, 1, 1);
    game_master_status_layout->addWidget(game_master_stamina_line, i, 1, 1, 1);
    game_master_stamina_line->setValidator(num_validator);
    i++;
    game_master_status_layout->addWidget(game_master_max_stamina_label, i, 0, 1, 1);
    game_master_status_layout->addWidget(game_master_max_stamina_line, i, 1, 1, 1);
    i++;
    game_master_status_layout->addWidget(game_master_sps_label, i, 0, 1, 1);
    game_master_status_layout->addWidget(game_master_sps_line, i, 1, 1, 1);
    i++;
    game_master_status_layout->addWidget(game_master_button, i, 0, 1, 2);

    game_master_hp_line->setEnabled(false);
    game_master_attack_line->setEnabled(false);
    game_master_defense_line->setEnabled(false);
    game_master_max_stamina_line->setEnabled(false);
    game_master_sps_line->setEnabled(false);

    //vip信息
    i=0;
    vip_layout->addWidget(vip_label, i, 0, 1, 2, Qt::AlignTop);
    i++;
    vip_layout->addWidget(vip_level_label, i, 0, 1, 1, Qt::AlignTop);
    vip_layout->addWidget(vip_level_line, i, 1, 1, 1, Qt::AlignTop);
    i++;
    vip_layout->addWidget(vip_exp_label, i, 0, 1, 1, Qt::AlignTop);
    vip_layout->addWidget(vip_exp_line, i, 1, 1, 1, Qt::AlignTop);
    i++;
    vip_layout->addWidget(vip_info_button, i, 0, 1, 2, Qt::AlignTop);

    vip_level_line->setEnabled(false);

    //惩罚
    i = 0;
    punish_layout->addWidget(login_banned_label , i, 0, 1, 1);
    punish_layout->addWidget(login_banned_stat_label , i, 1, 1, 1);
    i++;
    punish_layout->addWidget(silent_label, i, 0, 1, 1);
    punish_layout->addWidget(silent_stat_label, i, 1, 1, 1);

    //下部
    monster_button->setMinimumWidth(150);
    weapon_button->setMinimumWidth(150);
    guard_button->setMinimumWidth(150);

    down_layout->addWidget(monster_button);
    down_layout->addWidget(weapon_button);
    down_layout->addWidget(guard_button);
    down_layout->addStretch();

    //顶部
    top_layout->addWidget(server_num_box);
    top_layout->addWidget(user_id_label);
    top_layout->addWidget(user_id_line);
    user_id_line->setValidator(num_validator);
    user_id_line->setMaximumWidth(200);
    top_layout->addWidget(search_button);
    top_layout->addStretch();

    //上部
    up_layout->addWidget(game_master_widget);
    up_layout->addWidget(game_master_status_widget);
    up_layout->addWidget(vip_widget);
    up_layout->addWidget(punish_widget);
    up_layout->addWidget(result_browser);
    game_master_widget->setLayout(game_master_layout);
    game_master_status_widget->setLayout(game_master_status_layout);
    vip_widget->setLayout(vip_layout);
    punish_widget->setLayout(punish_layout);

    //全局
    layout->addWidget(top_widget);
    layout->addWidget(up_widget);
    layout->addWidget(down_widget);

    top_widget->setLayout(top_layout);
    up_widget->setLayout(up_layout);
    down_widget->setLayout(down_layout);

    widget->setLayout(layout);

    setCentralWidget(widget);

    connect(search_button, &QPushButton::clicked, this, &datam::searchUser);

    connect(user_id_line, &QLineEdit::textChanged, this, &datam::userIdChanged);

    connect(game_master_button, &QPushButton::clicked, this, &datam::modifyGameMaster);
    connect(vip_info_button, &QPushButton::clicked, this, &datam::modifyVipInfo);
    connect(monster_button, &QPushButton::clicked, this, &datam::showMonsterInfo);
    connect(weapon_button, &QPushButton::clicked, this, &datam::showWeaponInfo);
    connect(guard_button, &QPushButton::clicked, this, &datam::showGuardInfo);
}

//改变了USER_ID的值
void datam::userIdChanged() {
    edit_flag = 0;
}

//查询用户
void datam::searchUser() {
    //获取目标SERVER_IP
    QString server_num = server_num_box->currentText();
    QString server_ip = getAddressByServerNum(server_num);

    //获取ID
    QString user_id = user_id_line->text().trimmed();
    if (user_id == "") {
        result_browser->append("id不能为空");
        return;
    }

    //创建连接
    d_manager = new QNetworkAccessManager;
    QEventLoop loop;

    QUrl url = "http://" + server_ip + "/The_Wall/common/dataModifySearch?user_id=" + user_id;
    connect(d_manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    QNetworkReply *reply;
    reply = d_manager->get(QNetworkRequest(url));
    loop.exec();

    //结果表示
    if (reply->error() != QNetworkReply::NoError) {
        result_browser->append("查询失败");
        return;
    }

    QJsonParseError json_error;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll(), &json_error);
    if (json_error.error != QJsonParseError::NoError) {
        result_browser->append("数据解析失败");
        return;
    }

    QVariantMap result = jsonDocument.toVariant().toMap();
    QVariantMap game_master = result["game_master"].toMap();
    QVariantMap vip_info = result["vip_info"].toMap();
    int login_banned_flag = result["login_banned_flag"].toInt();
    int chat_banned_flag = result["chat_banned_flag"].toInt();

    setGameMaster(game_master);
    setVipInfo(vip_info);
    setPunishInfo(login_banned_flag, chat_banned_flag);

    edit_flag = true;   //设为可编辑状态

    result_browser->append("查询成功");

    d_manager->clearConnectionCache();
    d_manager->clearAccessCache();

    reply->deleteLater();//最后要释放reply对象
}

//更改了game_master
void datam::modifyGameMaster() {
    if (edit_flag == 0) {
        result_browser->append("请先查询用户");
        return;
    }

    QString user_id = user_id_line->text();
    QString nickname = game_master_nickname_line->text();
    QString exp = game_master_exp_line->text();
    QString stone = game_master_stone_line->text();
    QString coin = game_master_coin_line->text();
    QString skip_ticket = game_master_stt_line->text();
    QString ladder_points = game_master_lpt_line->text();
    QString girudo_coin = game_master_gdp_line->text();
    QString stamina = game_master_stamina_line->text();

    //获取目标SERVER_IP
    QString server_num = server_num_box->currentText();
    QString server_ip = getAddressByServerNum(server_num);

    //建立连接
    d_manager = new QNetworkAccessManager(this);

    QByteArray postData;
    postData.append("user_id=" + user_id);
    postData.append("&nickname=" + nickname);
    postData.append("&exp=" + exp);
    postData.append("&stone=" + stone);
    postData.append("&coin=" + coin);
    postData.append("&skip_ticket=" + skip_ticket);
    postData.append("&ladder_points=" + ladder_points);
    postData.append("&girudo_coin=" + girudo_coin);
    postData.append("&stamina=" + stamina);

    QUrl url = "http://" + server_ip + "/The_Wall/common/modifyGameMaster";

    QEventLoop loop;
    QNetworkReply *reply;
    QNetworkRequest request;

    connect(d_manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);

    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    reply = d_manager->post(request, postData);

    loop.exec();

    //结果表示
    if (reply->error() != QNetworkReply::NoError) {
        result_browser->append("更改失败");
        return;
    }

    QJsonParseError json_error;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll(), &json_error);
    if (json_error.error != QJsonParseError::NoError) {
        result_browser->append("数据解析失败");
        return;
    }

    QVariantMap result = jsonDocument.toVariant().toMap();
    QVariantMap game_master = result["game_master"].toMap();

    setGameMaster(game_master);

    result_browser->append("更改成功");

    d_manager->clearConnectionCache();
    d_manager->clearAccessCache();

    reply->deleteLater();//最后要释放reply对象
}

//更改了vip_info
void datam::modifyVipInfo() {
    if (edit_flag == 0) {
        result_browser->append("请先查询用户");
        return;
    }

    QString user_id = user_id_line->text();
    QString exp = vip_exp_line->text();

    //获取目标SERVER_IP
    QString server_num = server_num_box->currentText();
    QString server_ip = getAddressByServerNum(server_num);

    //建立连接
    d_manager = new QNetworkAccessManager(this);

    QByteArray postData;
    postData.append("user_id=" + user_id);
    postData.append("&exp=" + exp);

    QUrl url = "http://" + server_ip + "/The_Wall/common/modifyVipInfo";

    QEventLoop loop;
    QNetworkReply *reply;
    QNetworkRequest request;

    connect(d_manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);

    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    reply = d_manager->post(request, postData);

    loop.exec();

    //结果表示
    if (reply->error() != QNetworkReply::NoError) {
        result_browser->append("更改失败");
        return;
    }

    QJsonParseError json_error;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll(), &json_error);
    if (json_error.error != QJsonParseError::NoError) {
        result_browser->append("数据解析失败");
        return;
    }

    QVariantMap result = jsonDocument.toVariant().toMap();
    QVariantMap vip_info = result["vip_info"].toMap();

    setVipInfo(vip_info);

    result_browser->append("更改成功");

    d_manager->clearConnectionCache();
    d_manager->clearAccessCache();

    reply->deleteLater();//最后要释放reply对象
}

void datam::setGameMaster(QVariantMap game_master) {
    game_master_level_line->setText(game_master["level"].toString());
    game_master_nickname_line->setText(game_master["nickname"].toString());
    game_master_exp_line->setText(game_master["exp"].toString());
    game_master_stone_line->setText(game_master["stone_free"].toString());
    game_master_coin_line->setText(game_master["coin"].toString());
    game_master_stt_line->setText(game_master["skip_ticket"].toString());
    game_master_lpt_line->setText(game_master["ladder_points"].toString());
    game_master_gdp_line->setText(game_master["girudo_coin"].toString());

    game_master_hp_line->setText(game_master["hp"].toString());
    game_master_attack_line->setText(game_master["attack"].toString());
    game_master_defense_line->setText(game_master["defense"].toString());
    game_master_stamina_line->setText(game_master["stamina"].toString());
    game_master_max_stamina_line->setText(game_master["max_stamina"].toString());
    game_master_sps_line->setText(game_master["strengthen_prediction"].toString());

}

void datam::setVipInfo(QVariantMap vip_info) {
    vip_exp_line->setText(vip_info["exp"].toString());
    vip_level_line->setText(vip_info["level"].toString());
}

void datam::setPunishInfo(int login_banned_flag, int chat_banned_flag)
{
    QString login_banned_message;
    if (login_banned_flag == 0) {
        login_banned_message = "正常";
//        login_banned_stat_label->set = ;
    } else {
        login_banned_message = "限制登录";
    }

    QString chat_banned_message;
    if (chat_banned_flag == 0) {
        chat_banned_message == "正常";
    } else {
        login_banned_message == "禁言中";
    }
}

//显示英雄数据
void datam::showMonsterInfo() {
    QString server_num = server_num_box->currentText();
    QString server_ip = getAddressByServerNum(server_num);
    QString user_id = user_id_line->text();

    if (user_id == "") {
        result_browser->append("id不能为空");
        return;
    }

    d_manager = new QNetworkAccessManager(this);

    QUrl url = "http://" + server_ip + "/The_Wall/common/showMonster?user_id=" + user_id;
//    QUrl url = "http://localhost/The_Wall/common/showMonster?user_id=" + user_id;

    QEventLoop loop;
    QNetworkReply *reply;
    QNetworkRequest request;

    connect(d_manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);

    request.setUrl(url);

    reply = d_manager->get(request);

    monster_button->setEnabled(false);

    loop.exec();

    monster_button->setEnabled(true);

    //结果表示
    if (reply->error() != QNetworkReply::NoError) {
        result_browser->append("<div style=\"color:#FF0000\">英雄查询失败</div>");
        return;
    }

    QJsonParseError js_err;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll(), &js_err);
    if (js_err.error != QJsonParseError::NoError) {
        result_browser->append("<div style=\"color:#FF0000\">数据解析失败</div>");
        return;
    }

    QVariantMap result = jsonDocument.toVariant().toMap();

    //生成新的窗口记录查询的日志
    QMainWindow *window = new QMainWindow;
    QTableWidget *m_widget = new QTableWidget;
    m_widget->setColumnCount(12);
    m_widget->setHorizontalHeaderLabels(QStringList()<<"英雄ID"<<"等级"<<"HP"<<"攻击"<<"防御"<<"暴击"<<"闪避"<<"法强"<<"主动技等级"<<"连锁技等级"<<"品阶"<<"战斗力");

    m_widget->verticalHeader()->setVisible(false);

    window->setCentralWidget(m_widget);

    int row = 0;
    foreach (QVariant monster_info, result["monster_info"].toList()) {
        QVariantMap monster = monster_info.toMap();
        m_widget->insertRow(row);

        m_widget->setItem(row, 0, new QTableWidgetItem(monster["user_monster"].toString()));
        m_widget->setItem(row, 1, new QTableWidgetItem(monster["level"].toString()));
        m_widget->setItem(row, 2, new QTableWidgetItem(monster["hp"].toString()));
        m_widget->setItem(row, 3, new QTableWidgetItem(monster["attack"].toString()));
        m_widget->setItem(row, 4, new QTableWidgetItem(monster["defense"].toString()));
        m_widget->setItem(row, 5, new QTableWidgetItem(monster["critical"].toString()));
        m_widget->setItem(row, 6, new QTableWidgetItem(monster["missing"].toString()));
        m_widget->setItem(row, 7, new QTableWidgetItem(monster["ap"].toString()));
        m_widget->setItem(row, 8, new QTableWidgetItem(monster["active_skill_level"].toString()));
        m_widget->setItem(row, 9, new QTableWidgetItem(monster["chain_skill_level"].toString()));
        m_widget->setItem(row, 10, new QTableWidgetItem(monster["grade_level"].toString()));
        m_widget->setItem(row, 11, new QTableWidgetItem(monster["strengthen_prediction"].toString()));

        row++;
    }

    if (row == 0) {
        result_browser->append("<div style=\"color:#FF0000\">无该玩家的英雄数据</div>");
        return;
    }

    d_manager->clearConnectionCache();
    d_manager->clearAccessCache();

    reply->deleteLater();

    window->show();
}

//显示武器数据
void datam::showWeaponInfo() {
    QString server_num = server_num_box->currentText();
    QString server_ip = getAddressByServerNum(server_num);
    QString user_id = user_id_line->text();

    if (user_id == "") {
        result_browser->append("id不能为空");
        return;
    }

    d_manager = new QNetworkAccessManager(this);

    QUrl url = "http://" + server_ip + "/The_Wall/common/showWeapon?user_id=" + user_id;
//    QUrl url = "http://localhost/The_Wall/common/showWeapon?user_id=" + user_id;

    QEventLoop loop;
    QNetworkReply *reply;
    QNetworkRequest request;

    connect(d_manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);

    request.setUrl(url);

    reply = d_manager->get(request);

    weapon_button->setEnabled(false);

    loop.exec();

    weapon_button->setEnabled(true);

    //结果表示
    if (reply->error() != QNetworkReply::NoError) {
        result_browser->append("<div style=\"color:#FF0000\">武器查询失败</div>");
        return;
    }

    QJsonParseError js_err;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll(), &js_err);
    if (js_err.error != QJsonParseError::NoError) {
        result_browser->append("<div style=\"color:#FF0000\">数据解析失败</div>");
        return;
    }

    QVariantMap result = jsonDocument.toVariant().toMap();

    //生成新的窗口记录查询的日志
    QMainWindow *window = new QMainWindow;
    QTableWidget *m_widget = new QTableWidget;
    m_widget->setColumnCount(7);
    m_widget->setHorizontalHeaderLabels(QStringList()<<"武器ID"<<"等级"<<"攻击"<<"防御"<<"主动技等级"<<"连锁技等级"<<"战斗力");

    m_widget->verticalHeader()->setVisible(false);

    window->setCentralWidget(m_widget);

    int row = 0;
    foreach (QVariant weapon_info, result["weapon_info"].toList()) {
        QVariantMap weapon = weapon_info.toMap();
        m_widget->insertRow(row);

        m_widget->setItem(row, 0, new QTableWidgetItem(weapon["user_weapon"].toString()));
        m_widget->setItem(row, 1, new QTableWidgetItem(weapon["level"].toString()));
        m_widget->setItem(row, 2, new QTableWidgetItem(weapon["attack"].toString()));
        m_widget->setItem(row, 3, new QTableWidgetItem(weapon["defense"].toString()));
        m_widget->setItem(row, 4, new QTableWidgetItem(weapon["active_skill_level"].toString()));
        m_widget->setItem(row, 5, new QTableWidgetItem(weapon["chain_skill_level"].toString()));
        m_widget->setItem(row, 6, new QTableWidgetItem(weapon["strengthen_prediction"].toString()));

        row++;
    }

    if (row == 0) {
        result_browser->append("<div style=\"color:#FF0000\">无该玩家的武器数据</div>");
        return;
    }

    d_manager->clearConnectionCache();
    d_manager->clearAccessCache();

    reply->deleteLater();

    window->show();
}

//显示装甲数据
void datam::showGuardInfo() {
    QString server_num = server_num_box->currentText();
    QString server_ip = getAddressByServerNum(server_num);
    QString user_id = user_id_line->text();

    if (user_id == "") {
        result_browser->append("id不能为空");
        return;
    }

    d_manager = new QNetworkAccessManager(this);

    QUrl url = "http://" + server_ip + "/The_Wall/common/showGuard?user_id=" + user_id;
//    QUrl url = "http://localhost/The_Wall/common/showGuard?user_id=" + user_id;

    QEventLoop loop;
    QNetworkReply *reply;
    QNetworkRequest request;

    connect(d_manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);

    request.setUrl(url);

    reply = d_manager->get(request);

    guard_button->setEnabled(false);

    loop.exec();

    guard_button->setEnabled(true);

    //结果表示
    if (reply->error() != QNetworkReply::NoError) {
        result_browser->append("<div style=\"color:#FF0000\">装甲查询失败</div>");
        return;
    }

    QJsonParseError js_err;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll(), &js_err);
    if (js_err.error != QJsonParseError::NoError) {
        result_browser->append("<div style=\"color:#FF0000\">数据解析失败</div>");
        return;
    }

    QVariantMap result = jsonDocument.toVariant().toMap();

    //生成新的窗口记录查询的日志
    QMainWindow *window = new QMainWindow;
    QTableWidget *m_widget = new QTableWidget;
    m_widget->setColumnCount(7);
    m_widget->setHorizontalHeaderLabels(QStringList()<<"装甲ID"<<"等级"<<"攻击"<<"防御"<<"被动技等级"<<"战斗力");

    m_widget->verticalHeader()->setVisible(false);

    window->setCentralWidget(m_widget);

    int row = 0;
    foreach (QVariant weapon_info, result["weapon_info"].toList()) {
        QVariantMap weapon = weapon_info.toMap();
        m_widget->insertRow(row);

        m_widget->setItem(row, 0, new QTableWidgetItem(weapon["user_weapon"].toString()));
        m_widget->setItem(row, 1, new QTableWidgetItem(weapon["level"].toString()));
        m_widget->setItem(row, 2, new QTableWidgetItem(weapon["attack"].toString()));
        m_widget->setItem(row, 3, new QTableWidgetItem(weapon["defense"].toString()));
        m_widget->setItem(row, 4, new QTableWidgetItem(weapon["passive_skill_level"].toString()));
        m_widget->setItem(row, 5, new QTableWidgetItem(weapon["strengthen_prediction"].toString()));

        row++;
    }

    if (row == 0) {
        result_browser->append("<div style=\"color:#FF0000\">无该玩家的武器数据</div>");
        return;
    }

    d_manager->clearConnectionCache();
    d_manager->clearAccessCache();

    reply->deleteLater();

    window->show();
}

void datam::setServerInfo(QNetworkReply *reply) {
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

    d_manager->clearConnectionCache();
    d_manager->clearAccessCache();

    reply->deleteLater();//最后要释放reply对象
}

//根据服务器的编号获取服务器的address
QString datam::getAddressByServerNum(QString server_num) {
    return serverMap[server_num].server_address;
}
