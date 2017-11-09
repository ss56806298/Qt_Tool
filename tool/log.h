#ifndef LOG_H
#define LOG_H

#include <QMainWindow>
#include <QNetworkAccessManager>

#include <QPushButton>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QDateEdit>
#include <QLabel>
#include <QTextBrowser>

#include "constant.h"

class Ui;

class Log : public QMainWindow
{
    Q_OBJECT
public:
    Log(Ui *ui);

    QNetworkAccessManager *l_manager;

    QString server_ip;

    QMap <QString, sd> serverMap;

    //Button
    QPushButton *login_log_button = new QPushButton("登录日志");
    QPushButton *stone_log_button = new QPushButton("符石日志");
    QPushButton *pve_log_button = new QPushButton("PVE日志");
    QPushButton *pvp_log_button = new QPushButton("PVP日志");
    QPushButton *season_log_button = new QPushButton("赛季日志");
    //Label
    QLabel *user_id_label = new QLabel("玩家Id");

    QLabel *start_date_label = new QLabel("开始日期");
    QLabel *end_date_label = new QLabel("结束日期");

    //QDateEdit
    QDateEdit *start_date_edit = new QDateEdit;
    QDateEdit *end_date_edit = new QDateEdit;

    //Layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    QHBoxLayout *button_layout = new QHBoxLayout(this);
    QGridLayout *left_layout = new QGridLayout(this);

    //Widget
    QWidget *left_widget = new QWidget(this);
    QWidget *button_widget = new QWidget(this);
    QWidget *widget = new QWidget(this);

    //QLineEdit
    QLineEdit *user_id_line = new QLineEdit(this);

    //ComboBox
    QComboBox *server_num_box = new QComboBox(this);

    //TextBrowser
    QTextBrowser *result_browser = new QTextBrowser(this);

    //validator
    QValidator *num_validator = new QRegExpValidator(QRegExp("[0-9]+$"));   //只能输入数字
private:
    Ui *ui;

    void dealWithRawLogKV(QString key, QString value);

private Q_SLOTS:
    void setServerInfo(QNetworkReply *reply);
    QString getAddressByServerNum(QString server_num);

    void timeChanged(const QDate &date);    //日期发生变化

    void loginLogInquire();
    void stoneLogInquire();
    void pveLogInquire();
    void pvpLogInquire();
    void seasonLogInquire();

//    void updateLog();

};

#endif // LOG_H
