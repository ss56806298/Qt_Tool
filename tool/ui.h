#ifndef UI_H
#define UI_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QGridLayout>
#include <QAction>

#include <QMainWindow>
#include "ui_tool.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <QJsonDocument>
#include <QJsonParseError>
#include <QVariantMap>

#include <QTextCodec>
#include <QString>

class ui : public QMainWindow
{
    Q_OBJECT
    public:
        ui(QWidget *parent = Q_NULLPTR);
        //按钮
        QPushButton *search_button = new QPushButton("查询");
        QPushButton *area_version_button = new QPushButton("提交版本号");
        //标签
        QLabel *area_name_label = new QLabel("渠道组");
        QLabel *area_name_content_label = new QLabel();
        QLabel *area_version_label = new QLabel("渠道版本号");
        QLabel *area_version_change_label = new QLabel();
        QLabel *area_servers_label = new QLabel("渠道服务器");
        QLabel *area_servers_content_label = new QLabel();

        //输入框
        QLineEdit *area_version_content_line = new QLineEdit();
        //下拉框
        QComboBox *area_box = new QComboBox(this);
        QComboBox *upload_num_box = new QComboBox(this);
        //布局
        QGridLayout *layout = new QGridLayout(this);
        //窗口
        QWidget *widget = new QWidget;
        //动作
        QAction *openAction;

    private Q_SLOTS:
        void areaFill(QNetworkReply *reply);
        void clickAreaSearch();
        void clickVersionChange();
        void areaInfoFill(QNetworkReply *reply);
        void changeVersion(QNetworkReply *reply);

    private:

        QNetworkAccessManager *n_manager;
};

#endif // UI_H
