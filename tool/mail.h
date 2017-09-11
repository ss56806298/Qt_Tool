#ifndef MAIL_H
#define MAIL_H

#include "ui.h"
#include <QTextEdit>
#include <QTextBrowser>
#include <QCheckBox>

typedef struct reward_items {
    QLineEdit* id;
    QLineEdit* num;
}rw;

class mail: public QMainWindow {
    public:
        QString main_server_ip;

        QString server_area;

        QList<rw> items_info;

        QMap <QString, sd> serverMap;
    private:
        QNetworkAccessManager *m_manager;
    Q_OBJECT
    public:
        mail(ui *ui, QWidget *parent = Q_NULLPTR);

        //布局
        QHBoxLayout *layout = new QHBoxLayout;
        QGridLayout *left_layout = new QGridLayout;

        //小窗口
        QWidget *left_widget = new QWidget;
        QWidget *widget = new QWidget;

        //label
        QLabel *mail_title_label = new QLabel("邮件ID");
        QLabel *mail_message_label = new QLabel("邮件正文");
        QLabel *mail_sender_label = new QLabel("邮件署名");
        QLabel *expiry_date_label = new QLabel("有效期(天)");
        QLabel *reward_id_label = new QLabel("奖励");
        QLabel *reward_num_label = new QLabel("数量");

        //富文本
        QTextBrowser *send_result_browser = new QTextBrowser();

        //输入框
        QLineEdit *mail_title_line = new QLineEdit();
        QTextEdit *mail_message_line = new QTextEdit();
        QLineEdit *mail_sender_line = new QLineEdit();
        QLineEdit *expiry_date_line = new QLineEdit();

        //button
        QPushButton *send_by_account_button = new QPushButton("按账号发送");
        QPushButton *send_by_userid_button = new QPushButton("按玩家ID发送");
        QPushButton *send_to_all_button = new QPushButton("全部发送");

        //下拉框
        QComboBox *server_num_box = new QComboBox;

        //对勾选项框
        QCheckBox *limit_flag_box = new QCheckBox("打勾后有效期内的新注册玩家不可以领取");
    private Q_SLOTS:
        void setServerInfo(QNetworkReply *reply);
        QString getAddressByServerNum(QString server_num);

        void sendToAll();
};

#endif // MAIL_H
