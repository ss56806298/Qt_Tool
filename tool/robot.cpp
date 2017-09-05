#include "robot.h"

robot::robot(ui *ui, QWidget *parent) {
    server_area = ui->area_box->currentText();

    setWindowTitle(server_area);

    main_server_ip = ui->main_server_ip;

    //获取现在有哪些渠道组
    r_manager = new QNetworkAccessManager(this);
    connect(r_manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(setServerInfo(QNetworkReply *))); //关联信号和槽

    QUrl url = "http://" + main_server_ip + "/original_server/common/serverSearch?area=" + server_area;

    r_manager->get(QNetworkRequest(url));//发送请求

    //昵称的默认值
    first_name << "无助的" << "圣光的" << "腐化的" << "强大的"<<"神圣的"<<"惋惜的"<<"危险的"<<"优雅的"<<"无言的"<<"蔼然的";
    middle_name << "强横"<< "鲁莽"<< "帅气"<< "残暴"<< "暴虐"<< "残忍"<< "仁慈"<< "霸气"<< "淡定"<< "寂寞";
    last_name<<"领主"<<"恶魔"<<"宗师"<<"军曹"<<"士兵"<<"天使"<<"女王"<<"虫子"<<"兽人"<<"勇士";
    portrait<<"10001"<<"10002"<<"10003"<<"10004"<<"10005"<<"10006"<<"10007"<<"10008";

    //布局
    //人物信息
    int i = 0;
    menu_layout->addWidget(server_num_box, i, 0, 1, 1);
    menu_layout->addWidget(create_robot_num_label, i, 1, 1, 1);
    menu_layout->addWidget(create_robot_num_line, i, 2, 1, 1);
    i++;
    menu_layout->addWidget(game_master_label, i, 0, 1, 4, Qt::AlignHCenter);
    i++;
    menu_layout->addWidget(game_master_exp_label, i, 0, 1, 1);
    menu_layout->addWidget(game_master_exp_min_line, i, 1, 1, 1);
    menu_layout->addWidget(game_master_exp_max_line, i, 2, 1, 1);
    i++;
    menu_layout->addWidget(game_master_nickname_label, i, 0, 1, 1);
    first_name_box->addItems(first_name);
    menu_layout->addWidget(first_name_box, i, 1, 1, 1);
    middle_name_box->addItems(middle_name);
    menu_layout->addWidget(middle_name_box, i, 2, 1, 1);
    last_name_box->addItems(last_name);
    menu_layout->addWidget(last_name_box, i, 3, 1, 1);
    i++;
    menu_layout->addWidget(game_master_gender_label, i, 0, 1, 1);
    menu_layout->addWidget(game_master_gender_male_line, i, 1, 1, 1);
    menu_layout->addWidget(game_master_gender_female_line, i, 2, 1, 1);
    i++;
    menu_layout->addWidget(game_master_portrait_label, i, 0, 1, 1);
    portrait_box->addItems(portrait);
    menu_layout->addWidget(portrait_box, i, 1, 1, 1);

    //英雄信息
    i++;
    menu_layout->addWidget(user_monster_label, i, 0 , 1, 4, Qt::AlignHCenter);
    i++;
    menu_layout->addWidget(user_monster_add_label, i, 0, 1, 1);
    menu_layout->addWidget(user_monster_id_line, i, 1, 1, 1);
    menu_layout->addWidget(user_monster_weight_line, i, 2, 1, 1);
    menu_layout->addWidget(user_monster_add_button, i, 3, 1, 1);
    i++;
    menu_layout->addWidget(user_monster_num_label, i , 0, 1, 1);
    menu_layout->addWidget(user_monster_num_min_line, i, 1, 1, 1);
    menu_layout->addWidget(user_monster_num_max_line, i, 2, 1, 1);

    connect(user_monster_add_button, &QPushButton::clicked, this, &robot::addMonster);

    //武器信息
    i++;
    menu_layout->addWidget(user_weapon_label, i, 0, 1, 4, Qt::AlignHCenter);
    i++;
    menu_layout->addWidget(user_weapon_add_label, i, 0, 1, 1);
    menu_layout->addWidget(user_weapon_id_line, i, 1, 1, 1);
    menu_layout->addWidget(user_weapon_weight_line, i, 2, 1, 1);
    menu_layout->addWidget(user_weapon_add_button, i, 3, 1, 1);

    connect(user_weapon_add_button, &QPushButton::clicked, this, &robot::addWeapon);

    //头盔信息
    i++;
    menu_layout->addWidget(user_helmet_label, i, 0, 1, 4, Qt::AlignCenter);
    i++;
    menu_layout->addWidget(user_helmet_add_label, i, 0, 1, 1);
    menu_layout->addWidget(user_helmet_id_line, i, 1, 1, 1);
    menu_layout->addWidget(user_helmet_weight_line, i, 2, 1, 1);
    menu_layout->addWidget(user_helmet_add_button, i, 3, 1, 1);

    connect(user_helmet_add_button, &QPushButton::clicked, this, &robot::addHelmet);

    //装甲信息
    i++;
    menu_layout->addWidget(user_armor_label, i, 0, 1, 4, Qt::AlignCenter);
    i++;
    menu_layout->addWidget(user_armor_add_label, i, 0, 1, 1);
    menu_layout->addWidget(user_armor_id_line, i, 1, 1, 1);
    menu_layout->addWidget(user_armor_weight_line, i, 2, 1, 1);
    menu_layout->addWidget(user_armor_add_button, i, 3, 1, 1);

    connect(user_armor_add_button, &QPushButton::clicked, this, &robot::addArmor);

    //pvp信息
    i++;
    menu_layout->addWidget(user_pvp_label, i, 0, 1, 4, Qt::AlignCenter);
    i++;
    menu_layout->addWidget(user_pvp_score_label, i, 0, 1, 1);
    menu_layout->addWidget(user_pvp_score_min_line, i, 1, 1, 1);
    menu_layout->addWidget(user_pvp_score_max_line, i, 2, 1, 1);

    //season信息
    i++;
    menu_layout->addWidget(user_season_label, i, 0, 1, 1);
    menu_layout->addWidget(season_checkbox, i, 1, 1, 1);
    menu_layout->addWidget(user_season_num_label, i, 2, 1, 1);
    menu_layout->addWidget(user_season_num_line, i, 3, 1, 1);

    //英雄布局
    monsterWidget->setColumnCount(2);
    monsterWidget->setRowCount(30);
    monsterWidget->setHorizontalHeaderLabels(QStringList()<<"英雄ID"<<"出现权重");
    monsterWidget->verticalHeader()->setVisible(false);         //关闭左边的垂直表头

    //武器布局
    weaponWidget->setColumnCount(2);
    weaponWidget->setRowCount(30);
    weaponWidget->setHorizontalHeaderLabels(QStringList()<<"武器ID"<<"出现权重");
    weaponWidget->verticalHeader()->setVisible(false);         //关闭左边的垂直表头

    //头盔布局
    helmetWidget->setColumnCount(2);
    helmetWidget->setRowCount(30);
    helmetWidget->setHorizontalHeaderLabels(QStringList()<<"头盔ID"<<"出现权重");
    helmetWidget->verticalHeader()->setVisible(false);         //关闭左边的垂直表头

    //装甲布局
    armorWidget->setColumnCount(2);
    armorWidget->setRowCount(30);
    armorWidget->setHorizontalHeaderLabels(QStringList()<<"装甲ID"<<"出现权重");
    armorWidget->verticalHeader()->setVisible(false);         //关闭左边的垂直表头

    //生成机器人的布局
    create_layout->addWidget(create_robot_button);
    create_layout->addWidget(create_log_browser);
    createWidget->setLayout(create_layout);

    menuWidget->setLayout(menu_layout);

    up_layout->addWidget(menuWidget);
    up_layout->addWidget(createWidget);
    down_layout->addWidget(monsterWidget);
    down_layout->addWidget(weaponWidget);
    down_layout->addWidget(helmetWidget);
    down_layout->addWidget(armorWidget);

    upWidget->setLayout(up_layout);
    downWidget->setLayout(down_layout);

    layout->addWidget(upWidget);
    layout->addWidget(downWidget);

    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    //信号
    connect(create_robot_button, &QPushButton::clicked, this, &robot::beginCreate);
}

//增加生成的英雄的种类
void robot::addMonster() {
    QString monster_id = user_monster_id_line->text().trimmed();
    int weight = user_monster_weight_line->text().toInt();

    monsters[monster_id] = weight;

    //刷新列表
    refreshMonsters();
}

//刷新英雄列表
void robot::refreshMonsters() {
    monsterWidget->clearContents();

    QMap <QString, int>::iterator i;
    int j=0;
    for (i=monsters.begin();i!=monsters.end();i++) {
        monsterWidget->setItem(j, 0, new QTableWidgetItem(i.key()));
        monsterWidget->setItem(j, 1, new QTableWidgetItem(QString::number(i.value(), 10)));
        j++;
    }
}

//增加生成的武器的种类
void robot::addWeapon() {
    QString weapon_id = user_weapon_id_line->text().trimmed();
    int weight = user_weapon_weight_line->text().toInt();

    weapons[weapon_id] = weight;

    //刷新列表
    refreshWeapons();
}

//刷新武器列表
void robot::refreshWeapons() {
    weaponWidget->clearContents();

    QMap <QString, int>::iterator i;
    int j=0;
    for (i=weapons.begin();i!=weapons.end();i++) {
        weaponWidget->setItem(j, 0, new QTableWidgetItem(i.key()));
        weaponWidget->setItem(j, 1, new QTableWidgetItem(QString::number(i.value(), 10)));
        j++;
    }
}

//增加生成的头盔的种类
void robot::addHelmet() {
    QString helmet_id = user_helmet_id_line->text().trimmed();
    int weight = user_helmet_weight_line->text().toInt();

    helmets[helmet_id] = weight;

    //刷新列表
    refreshHelmets();
}

//刷新头盔列表
void robot::refreshHelmets() {
    helmetWidget->clearContents();

    QMap <QString, int>::iterator i;
    int j=0;
    for (i=helmets.begin();i!=helmets.end();i++) {
        helmetWidget->setItem(j, 0, new QTableWidgetItem(i.key()));
        helmetWidget->setItem(j, 1, new QTableWidgetItem(QString::number(i.value(), 10)));
        j++;
    }
}

//增加生成的装甲的种类
void robot::addArmor() {
    QString armor_id = user_armor_id_line->text().trimmed();
    int weight = user_armor_weight_line->text().toInt();

    armors[armor_id] = weight;

    //刷新列表
    refreshArmors();
}

//刷新装甲列表
void robot::refreshArmors() {
    armorWidget->clearContents();

    QMap <QString, int>::iterator i;
    int j=0;
    for (i=armors.begin();i!=armors.end();i++) {
        armorWidget->setItem(j, 0, new QTableWidgetItem(i.key()));
        armorWidget->setItem(j, 1, new QTableWidgetItem(QString::number(i.value(), 10)));
        j++;
    }
}

//开始生成
void robot::beginCreate() {
    QString num = create_robot_num_line->text();
    create_num = num.toInt();

    create_robot_num_line->setEnabled(false);
    game_master_exp_min_line->setEnabled(false);
    game_master_exp_max_line->setEnabled(false);
    game_master_gender_male_line->setEnabled(false);
    game_master_gender_female_line->setEnabled(false);
    user_monster_id_line->setEnabled(false);
    user_monster_weight_line->setEnabled(false);
    user_monster_num_min_line->setEnabled(false);
    user_monster_num_max_line->setEnabled(false);
    user_weapon_id_line->setEnabled(false);
    user_weapon_weight_line->setEnabled(false);
    user_helmet_id_line->setEnabled(false);
    user_helmet_weight_line->setEnabled(false);
    user_armor_id_line->setEnabled(false);
    user_armor_weight_line->setEnabled(false);
    user_pvp_score_min_line->setEnabled(false);
    user_pvp_score_max_line->setEnabled(false);
    season_checkbox->setEnabled(false);
    user_season_num_line->setEnabled(false);

    user_monster_add_button->setEnabled(false);
    user_weapon_add_button->setEnabled(false);
    user_helmet_add_button->setEnabled(false);
    user_armor_add_button->setEnabled(false);

    continueCreate();
}

//继续生成
void robot::continueCreate() {
    QString first = makeRand(first_name);
    QString middle = makeRand(middle_name);
    QString last = makeRand(last_name);
    QString name = first + middle + last;

    QString weapon = makeRand(weapons);
    QString helmet = makeRand(helmets);
    QString armor = makeRand(armors);
    QString portrait_id = makeRand(portrait);

    int expMin = game_master_exp_min_line->text().toInt();
    int expMax = game_master_exp_max_line->text().toInt();

    QString exp = QString::number(expMin+rand()%(expMax-expMin+1), 10);

    int maleWeight = game_master_gender_male_line->text().toInt();
    int femaleWeight = game_master_gender_female_line->text().toInt();
    QMap <QString, int> genderMap;
    genderMap["1"] = maleWeight;
    genderMap["2"] = femaleWeight;
    QString gender = makeRand(genderMap);

    int scoreMax = user_pvp_score_max_line->text().toInt();
    int scoreMin = user_pvp_score_min_line->text().toInt();
    int pvpScore = scoreMin + rand() % (scoreMax - scoreMin + 1);

    int monsterMin = user_monster_num_min_line->text().toInt();
    int monsterMax = user_monster_num_max_line->text().toInt();
    int monsterNum = monsterMin + rand() % (monsterMax - monsterMin + 1);
    QStringList monsterList;
    do {
        QString monsterId = makeRand(monsters);
        if (!monsterList.contains(monsterId)) {
            monsterList << monsterId;
        }
    } while (monsterList.size() < monsterNum);

    QJsonDocument jsonDocument = QJsonDocument::fromVariant(monsterList);

    QString monster_json = jsonDocument.toJson(QJsonDocument::Compact);

    r_manager = new QNetworkAccessManager(this);

    QString server_num_string = server_num_box->currentText();

    //获取游戏服务器的地址
    QString server_address = getAddressByServerNum(server_num_string);

    connect(r_manager, &QNetworkAccessManager::finished, this, &robot::resultAppear);

    QByteArray postData;

    QUrl url;
    //PVP还是赛季？
    if (season_checkbox->checkState() == Qt::Checked) {
        QString season_no = user_season_num_line->text();

        postData.append("nickname=" + name);
        postData.append("&weapon=" + weapon);
        postData.append("&helmet=" + helmet);
        postData.append("&armor=" + armor);
        postData.append("&portrait_id=" + portrait_id);
        postData.append("&exp=" + exp);
        postData.append("&gender=" + gender);
        postData.append("&season_score=" + QString::number(pvpScore, 10));
        postData.append("&monsters=" + monster_json);
        postData.append("&season_no=" + season_no);

        url = "http://" + server_address + "/The_Wall/common/createSeasonRobot";
    } else {
        postData.append("nickname=" + name);
        postData.append("&weapon=" + weapon);
        postData.append("&helmet=" + helmet);
        postData.append("&armor=" + armor);
        postData.append("&portrait_id=" + portrait_id);
        postData.append("&exp=" + exp);
        postData.append("&gender=" + gender);
        postData.append("&pvp_score=" + QString::number(pvpScore, 10));
        postData.append("&monsters=" + monster_json);

        url = "http://" + server_address + "/The_Wall/common/createPvpRobot";
    }

    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("Content-Type","application/x-www-form-urlencoded");

    r_manager->post(request, postData);
}

//结果显示
void robot::resultAppear(QNetworkReply *reply) {
    create_num--;

    if(reply->error() == QNetworkReply::NoError){
        QString result = QString(reply->readAll());

        if (result == "success") {
           create_log_browser->append("创建成功,剩余" + QString::number(create_num, 10) + "个");
        } else {
           create_log_browser->append("创建失败," + result + ",剩余" + QString::number(create_num, 10) + "个");
        }

        r_manager->clearConnectionCache();
        r_manager->clearAccessCache();

    } else {
        create_log_browser->append("创建失败,网络异常,剩余" + QString::number(create_num, 10) + "个");
    }

    if (create_num <= 0) {
        endCreate();
    } else {
        continueCreate();
    }
}

//生成结束
void robot::endCreate() {
    create_robot_num_line->setEnabled(true);
    game_master_exp_min_line->setEnabled(true);
    game_master_exp_max_line->setEnabled(true);
    game_master_gender_male_line->setEnabled(true);
    game_master_gender_female_line->setEnabled(true);
    user_monster_id_line->setEnabled(true);
    user_monster_weight_line->setEnabled(true);
    user_monster_num_min_line->setEnabled(true);
    user_monster_num_max_line->setEnabled(true);
    user_weapon_id_line->setEnabled(true);
    user_weapon_weight_line->setEnabled(true);
    user_helmet_id_line->setEnabled(true);
    user_helmet_weight_line->setEnabled(true);
    user_armor_id_line->setEnabled(true);
    user_armor_weight_line->setEnabled(true);
    user_pvp_score_min_line->setEnabled(true);
    user_pvp_score_max_line->setEnabled(true);
    season_checkbox->setEnabled(true);
    user_season_num_line->setEnabled(true);

    user_monster_add_button->setEnabled(true);
    user_weapon_add_button->setEnabled(true);
    user_helmet_add_button->setEnabled(true);
    user_armor_add_button->setEnabled(true);
}

//随机生成
QString robot::makeRand(QStringList stringList) {
    int len;
    len = stringList.length();

    int r = rand()%len;

    QString string = stringList.at(r);

    return string;
}

QString robot::makeRand(QMap <QString, int> map) {
    QMap <QString, int>::iterator i;
    int sumWeight = 0;
    for (i=map.begin();i!=map.end();i++) {
        sumWeight += i.value();
    }

    int r;
    r = rand()%sumWeight;
    int nowWeight = 0;
    QString key;

    key = "";
    for (i=map.begin();i!=map.end();i++) {
        nowWeight += i.value();
        if (nowWeight > r) {
            key = i.key();
            return key;
        }
    }
    return key;
}

//确定服务器的信息
void robot::setServerInfo(QNetworkReply *reply) {
    //处理返回的JSON数据
    QJsonParseError error;

    QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll(), &error);
    if (error.error == QJsonParseError::NoError) {
        if (jsonDocument.isObject()) {
            QVariantMap result = jsonDocument.toVariant().toMap();

            foreach(QVariant serverListT, result["server_list"].toList()) {
                QVariantMap serverList = serverListT.toMap();
                sf sd1;
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

//根据服务器的编号获取服务器的address
QString robot::getAddressByServerNum(QString server_num) {
    return serverMap[server_num].server_address;
}
