#ifndef CDKEY_H
#define CDKEY_H

#include "ui.h"

#include <QTextBrowser>

class cdkey : public QMainWindow
{
    public:
        int create_num = 0; //生成的数量
        QString main_server_ip;

        QFile *file;
    private:
        QNetworkAccessManager *c_manager;
    Q_OBJECT
    public:
        cdkey(Ui *ui, QWidget *parent = Q_NULLPTR);

        //布局
        QGridLayout *layout = new QGridLayout(this);

        //button
        QPushButton *submit_add_cdkeys_button = new QPushButton("开始生成");

        //LABEL
        QLabel *create_num_label = new QLabel("生成数量");
        QLabel *event_id_label = new QLabel("活动编号");
        QLabel *event_id_warning_label = new QLabel("为空可以任意次参与");
        QLabel *event_comment_label = new QLabel("备注");
        QLabel *platform_label = new QLabel("渠道");
        QLabel *used_times_label = new QLabel("使用次数");
        QLabel *used_times_warning_label = new QLabel("为0则代表无限使用");
        QLabel *start_time_label = new QLabel("开始时间");
        QLabel *end_time_label = new QLabel("结束时间");
        QLabel *reward_id_label = new QLabel("奖励");
        QLabel *reward_num_label = new QLabel("数量");

        //输入框
        QLineEdit *create_num_line = new QLineEdit();
        QLineEdit *event_id_line = new QLineEdit();
        QLineEdit *event_comment_line = new QLineEdit();
        QLineEdit *platform_line = new QLineEdit();
        QLineEdit *used_times_line = new QLineEdit();
        QLineEdit *start_time_line = new QLineEdit("2017-01-11 00:00:00");
        QLineEdit *end_time_line = new QLineEdit("2017-01-11 00:00:00");
        QLineEdit *reward_id1_line = new QLineEdit();
        QLineEdit *reward_num1_line = new QLineEdit();
        QLineEdit *reward_id2_line = new QLineEdit();
        QLineEdit *reward_num2_line = new QLineEdit();
        QLineEdit *reward_id3_line = new QLineEdit();
        QLineEdit *reward_num3_line = new QLineEdit();
        QLineEdit *reward_id4_line = new QLineEdit();
        QLineEdit *reward_num4_line = new QLineEdit();
        QLineEdit *reward_id5_line = new QLineEdit();
        QLineEdit *reward_num5_line = new QLineEdit();

        //富文本
        QTextBrowser *create_result = new QTextBrowser;



        QWidget *widget = new QWidget();

    private Q_SLOTS:
        void beginCreate();
        void continueCreate();
        void resultAppear(QNetworkReply *reply);
        void endCreate();
        void searchEventId();
        void eventCommentAppear(QNetworkReply *reply);
};

#endif // CDKEY_H
