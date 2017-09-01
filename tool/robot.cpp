#include "robot.h"

robot::robot(ui *ui, QWidget *parent) {
    server_area = ui->area_box->currentText();

    setWindowTitle(server_area);

    //昵称的默认值
    first_name << "无助的" << "圣光的" << "腐化的" << "强大的"<<"神圣的"<<"惋惜的"<<"危险的"<<"优雅的"<<"无言的"<<"蔼然的";
    middle_name << "强横"<< "鲁莽"<< "帅气"<< "残暴"<< "暴虐"<< "残忍"<< "仁慈"<< "霸气"<< "淡定"<< "寂寞";
    last_name<<"领主"<<"恶魔"<<"宗师"<<"军曹"<<"士兵"<<"天使"<<"女王"<<"虫子"<<"兽人"<<"勇士";
    portrait<<"10001"<<"10002"<<"10003"<<"10004"<<"10005"<<"10006"<<"10007"<<"10008";

    //布局
    //人物信息
    int i = 0;
    menu_layout->addWidget(create_robot_num_label, i, 0, 1, 1);
    menu_layout->addWidget(create_robot_num_line, i, 1, 1, 1);
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
}

//增加生成的英雄的种类
void robot::addMonster() {
    QString monster_id = user_monster_id_line->text();
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
    QString weapon_id = user_weapon_id_line->text();
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
    QString helmet_id = user_helmet_id_line->text();
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
    QString armor_id = user_armor_id_line->text();
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

    continueCreate();
}

void robot::continueCreate() {

}
