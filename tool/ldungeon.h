#ifndef LDUNGEON_H
#define LDUNGEON_H

#include "ui.h"

#include <QTextBrowser>

class ldungeon: public QMainWindow
{
    public:
        QString server_area; //渠道组

        QString main_server_ip;

        QMap <QString, sd> serverMap;
    private:
        QNetworkAccessManager *l_manager;
    Q_OBJECT
    public:
        ldungeon(Ui *ui, QWidget *parent = Q_NULLPTR);

        //布局
        QGridLayout *layout = new QGridLayout();

        //button
        QPushButton *submit_cal_button = new QPushButton("开始结算");
        QPushButton *ldungeon_history_button = new QPushButton("查询");

        //标签
        QLabel *ldungeon_label = new QLabel("史诗副本(降临)结算");
        QLabel *ldungeon_no_label = new QLabel("降临area编号");
        QLabel *ldungeon_cal_history_label = new QLabel("降临结算历史");
        QLabel *ldungeon_frame_label = new QLabel("前100名发送的头像框id");

        //输入框
        QLineEdit *ldungeon_no_line = new QLineEdit;
        QLineEdit *ldungeon_frame_line = new QLineEdit;

        //下拉框
        QComboBox *server_num_box = new QComboBox(this);

        //文本
        QTextBrowser *ldungeon_cal_history_browser = new QTextBrowser;

        //窗口
        QWidget *widget = new QWidget;
    private Q_SLOTS:
        void setServerInfo(QNetworkReply *reply);
        QString getAddressByServerNum(QString server_num);

        void searchLdungeonCalHistory();
        void ldungeonHistoryShow(QNetworkReply *reply);
        void calLdungeon();
        void ldungeonCalResultShow(QNetworkReply *reply);
};

#endif // LDUNGEON_H
