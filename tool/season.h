#ifndef SEASON_H
#define SEASON_H

#include "ui.h"
#include "user.h"

#include <QTextBrowser>

class season : public QMainWindow
{
    public:
        QString server_area;    //渠道组

        QString main_server_ip;

        QMap <QString, sd> serverMap;
    private:
        QNetworkAccessManager *s_manager;
    Q_OBJECT
    public:
        season(Ui *ui, QWidget *parent = Q_NULLPTR);

        //布局
        QGridLayout *layout = new QGridLayout();

        //button
        QPushButton *submit_cal_button = new QPushButton("开始结算");
        QPushButton *season_history_button = new QPushButton("查询");

        //标签
        QLabel *season_label = new QLabel("赛季结算");
        QLabel *season_no_label = new QLabel("赛季编号");
        QLabel *season_cal_history_label = new QLabel("赛季结算历史");

        //输入框
        QLineEdit *season_no_line = new QLineEdit();

        //下拉框
        QComboBox *server_num_box = new QComboBox(this);

        //文本
        QTextBrowser *season_cal_history_browser = new QTextBrowser;

        //窗口
        QWidget *widget = new QWidget();
    private Q_SLOTS:
        void setServerInfo(QNetworkReply *reply);
        QString getAddressByServerNum(QString server_num);

        void searchSeasonCalHistory();
        void seasonHistoryShow(QNetworkReply *reply);
        void calSeason();
        void seasonCalResultShow(QNetworkReply *reply);
};

#endif // SEASON_H
