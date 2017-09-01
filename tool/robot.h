#ifndef ROBOT_H
#define ROBOT_H

#include <QMainWindow>

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTextBrowser>

#include "ui.h"

class robot:public QMainWindow {
    public:
        QString server_area;   //渠道组

        int create_num; //创造的机器人的数量

        QStringList first_name;
        QStringList middle_name;
        QStringList last_name;
        QStringList portrait;

        QMap <QString, int> monsters;   //怪物的集合
        QMap <QString, int> weapons;    //武器的集合
        QMap <QString, int> helmets;    //头盔的集合
        QMap <QString, int> armors;     //装甲的集合
    private:
        void refreshMonsters();
        void refreshWeapons();
        void refreshHelmets();
        void refreshArmors();

    Q_OBJECT
    public:
        robot(ui *ui, QWidget *parent = Q_NULLPTR);

        //布局
        QGridLayout *menu_layout = new QGridLayout(this);
        QVBoxLayout *create_layout = new QVBoxLayout(this);
        QHBoxLayout *up_layout = new QHBoxLayout(this);
        QHBoxLayout *down_layout = new QHBoxLayout(this);
        QVBoxLayout *layout = new QVBoxLayout(this);

        //选项窗口
        QTableWidget *monsterWidget = new QTableWidget;//怪物信息窗口
        QTableWidget *weaponWidget = new QTableWidget;//武器信息窗口
        QTableWidget *helmetWidget = new QTableWidget;//头盔信息窗口
        QTableWidget *armorWidget = new QTableWidget;//装甲信息窗口

        //普通窗口
        QWidget *centralWidget = new QWidget;   //中央窗口
        QWidget *menuWidget = new QWidget;
        QWidget *upWidget = new QWidget;
        QWidget *downWidget = new QWidget;
        QWidget *createWidget = new QWidget;//生成信息窗口

        //label
        QLabel *create_robot_num_label = new QLabel("生成机器人的数量");
        QLabel *game_master_label = new QLabel("人物信息");
        QLabel *game_master_exp_label = new QLabel("经验");
        QLabel *game_master_nickname_label = new QLabel("昵称");
        QLabel *game_master_gender_label = new QLabel("性别");
        QLabel *game_master_portrait_label = new QLabel("头像");
        QLabel *user_monster_label = new QLabel("英雄信息");
        QLabel *user_monster_add_label = new QLabel("添加英雄");
        QLabel *user_monster_num_label = new QLabel("携带英雄数量");
        QLabel *user_weapon_label = new QLabel("武器信息");
        QLabel *user_weapon_add_label = new QLabel("添加武器");
        QLabel *user_helmet_label = new QLabel("头盔信息");
        QLabel *user_helmet_add_label = new QLabel("添加头盔");
        QLabel *user_armor_label = new QLabel("装甲信息");
        QLabel *user_armor_add_label = new QLabel("添加装甲");
        QLabel *user_pvp_label = new QLabel("pvp信息");
        QLabel *user_pvp_score_label = new QLabel("pvp分数");

        //输入框
        QLineEdit *create_robot_num_line = new QLineEdit();
        QLineEdit *game_master_exp_min_line = new QLineEdit("最小值");
        QLineEdit *game_master_exp_max_line = new QLineEdit("最大值");
        QLineEdit *game_master_gender_male_line = new QLineEdit("男性权重");
        QLineEdit *game_master_gender_female_line = new QLineEdit("女性权重");
        QLineEdit *user_monster_id_line = new QLineEdit("英雄ID");
        QLineEdit *user_monster_weight_line = new QLineEdit("权重");
        QLineEdit *user_monster_num_min_line = new QLineEdit("最小值");
        QLineEdit *user_monster_num_max_line = new QLineEdit("最大值");
        QLineEdit *user_weapon_id_line = new QLineEdit("武器ID");
        QLineEdit *user_weapon_weight_line = new QLineEdit("权重");
        QLineEdit *user_helmet_id_line = new QLineEdit("头盔ID");
        QLineEdit *user_helmet_weight_line = new QLineEdit("权重");
        QLineEdit *user_armor_id_line = new QLineEdit("装甲ID");
        QLineEdit *user_armor_weight_line = new QLineEdit("权重");
        QLineEdit *user_pvp_score_min_line = new QLineEdit("最低分");
        QLineEdit *user_pvp_score_max_line = new QLineEdit("最高分");

        //下拉框
        QComboBox *first_name_box = new QComboBox(this);
        QComboBox *middle_name_box = new QComboBox(this);
        QComboBox *last_name_box = new QComboBox(this);
        QComboBox *portrait_box = new QComboBox(this);

        //button
        QPushButton *user_monster_add_button = new QPushButton("增加");
        QPushButton *user_weapon_add_button = new QPushButton("增加");
        QPushButton *user_helmet_add_button = new QPushButton("增加");
        QPushButton *user_armor_add_button = new QPushButton("增加");

        QPushButton *create_robot_button = new QPushButton("开始生成");

        //文本输出
        QTextBrowser *create_log_browser = new QTextBrowser;
    private Q_SLOTS:
        void addMonster();
        void addWeapon();
        void addHelmet();
        void addArmor();

        void beginCreate();
        void continueCreate();
        void endCreate();
};

#endif // ROBOT_H
