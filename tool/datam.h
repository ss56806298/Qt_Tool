#ifndef DATAM_H
#define DATAM_H

#include "ui.h"
#include <QTextBrowser>

class datam:public QMainWindow
{
    public:
        QString server_area;   //渠道组

        QString main_server_ip;

        QMap <QString, sd> serverMap;

        bool edit_flag = false; //是否可以提交编辑

        QValidator *num_validator = new QRegExpValidator(QRegExp("[0-9]+$"));   //只能输入数字
    private:
        QNetworkAccessManager *d_manager;
    Q_OBJECT
    public:
        datam(Ui *ui, QWidget *parent = Q_NULLPTR);

        //标签
        QLabel *user_id_label = new QLabel("角色ID");

        QLabel *game_master_label = new QLabel("人物信息");
        QLabel *game_master_level_label = new QLabel("等级");
        QLabel *game_master_nickname_label = new QLabel("昵称");
        QLabel *game_master_exp_label = new QLabel("经验");
        QLabel *game_master_stone_label = new QLabel("符石");
        QLabel *game_master_coin_label = new QLabel("金币");
        QLabel *game_master_stt_label = new QLabel("扫荡券");
        QLabel *game_master_lpt_label = new QLabel("天梯点");
        QLabel *game_master_gdp_label = new QLabel("公会币");

        QLabel *game_master_hp_label = new QLabel("hp");
        QLabel *game_master_attack_label = new QLabel("攻击力");
        QLabel *game_master_defense_label = new QLabel("防御力");
        QLabel *game_master_stamina_label = new QLabel("体力");
        QLabel *game_master_max_stamina_label = new QLabel("体力值上限");
        QLabel *game_master_sps_label = new QLabel("总战力");

        QLabel *vip_label = new QLabel("会员信息");
        QLabel *vip_level_label = new QLabel("等级");
        QLabel *vip_exp_label = new QLabel("经验");

        //输入框
        QLineEdit *user_id_line = new QLineEdit;

        QLineEdit *game_master_level_line = new QLineEdit;
        QLineEdit *game_master_nickname_line = new QLineEdit;
        QLineEdit *game_master_exp_line = new QLineEdit;
        QLineEdit *game_master_stone_line = new QLineEdit;
        QLineEdit *game_master_coin_line = new QLineEdit;
        QLineEdit *game_master_stt_line = new QLineEdit;
        QLineEdit *game_master_lpt_line = new QLineEdit;
        QLineEdit *game_master_gdp_line = new QLineEdit;

        QLineEdit *game_master_hp_line = new QLineEdit;
        QLineEdit *game_master_attack_line = new QLineEdit;
        QLineEdit *game_master_defense_line = new QLineEdit;
        QLineEdit *game_master_stamina_line = new QLineEdit;
        QLineEdit *game_master_max_stamina_line = new QLineEdit;
        QLineEdit *game_master_sps_line = new QLineEdit;

        QLineEdit *vip_level_line = new QLineEdit;
        QLineEdit *vip_exp_line = new QLineEdit;

        //窗体
        QWidget *game_master_widget = new QWidget;
        QWidget *game_master_status_widget  = new QWidget;
        QWidget *vip_widget = new QWidget;

        QWidget *top_widget = new QWidget;
        QWidget *up_widget = new QWidget;
        QWidget *down_widget = new QWidget;

        QWidget *widget = new QWidget;

        //布局
        QGridLayout *game_master_layout = new QGridLayout;
        QGridLayout *game_master_status_layout = new QGridLayout;
        QGridLayout *vip_layout = new QGridLayout;

        QHBoxLayout *top_layout = new QHBoxLayout;
        QHBoxLayout *up_layout = new QHBoxLayout;
        QHBoxLayout *down_layout = new QHBoxLayout;

        QVBoxLayout *layout = new QVBoxLayout;

        //富文本框
        QTextBrowser *result_browser = new QTextBrowser;

        //按钮
        QPushButton *search_button = new QPushButton("开始检索");

        QPushButton *monster_button = new QPushButton("英雄信息");
        QPushButton *weapon_button = new QPushButton("武器信息");
        QPushButton *guard_button = new QPushButton("装甲信息");

        QPushButton *game_master_button = new QPushButton("保存人物信息");
        QPushButton *vip_info_button = new QPushButton("保存VIP信息");

        //下拉框
        QComboBox *server_num_box = new QComboBox;
    private Q_SLOTS:
        void setServerInfo(QNetworkReply *reply);
        QString getAddressByServerNum(QString server_num);

        void searchUser();
        void setGameMaster(QVariantMap game_master);
        void setVipInfo(QVariantMap vip_info);

        void modifyGameMaster();
        void modifyVipInfo();

        void userIdChanged();
};

#endif // DATAM_H
