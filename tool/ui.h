#ifndef UI_H
#define UI_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QGridLayout>
#include <QFile>
#include <QFileDialog>
#include <QCryptographicHash>

#include <QMainWindow>
#include "ui_tool.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <QJsonDocument>
#include <QJsonParseError>
#include <QVariantMap>

#include <QTextCodec>
#include <QString>

typedef struct serverDetail{
    QString server_num;
    QString server_name;
    QString server_address;
}sd;

class ui : public QMainWindow
{
    public:
        QString main_server_ip = "taptap.yxlc.shadowpower.cn";
    Q_OBJECT
    public:
        ui(QWidget *parent = Q_NULLPTR);
        //按钮
        QPushButton *search_button = new QPushButton("查询");
        QPushButton *area_version_button = new QPushButton("提交版本号");
        QPushButton *open_file_button = new QPushButton("选择文件");
        QPushButton *upload_update_resource_button = new QPushButton("上传热更新文件");
        QPushButton *search_version_button = new QPushButton("查询资源");
        QPushButton *user_operate_button = new QPushButton("用户操作");
        QPushButton *robot_create_button = new QPushButton("生成机器人");
        QPushButton *cdkey_create_button = new QPushButton("生成cdkey");
        QPushButton *season_cal_button = new QPushButton("结算赛季");
        QPushButton *csv_upload_button = new QPushButton("上传csv");
        //标签
        QLabel *area_name_label = new QLabel("渠道组");
        QLabel *area_name_content_label = new QLabel();
        QLabel *area_version_label = new QLabel("渠道版本号");
        QLabel *area_version_change_label = new QLabel();
        QLabel *area_servers_label = new QLabel("渠道服务器");
        QLabel *area_servers_content_label = new QLabel();
        QLabel *file_path_label = new QLabel("路径");
        QLabel *file_size_label = new QLabel("文件大小");
        QLabel *file_size_content_label = new QLabel();
        QLabel *file_md5_label = new QLabel("文件MD5值");
        QLabel *file_md5_content_label = new QLabel();
        QLabel *file_version_label = new QLabel("资源版本");
        QLabel *file_url_label = new QLabel("资源地址");
        QLabel *upload_update_resource_result_label = new QLabel();
        //输入框
        QLineEdit *area_version_content_line = new QLineEdit();
        QLineEdit *file_path_line = new QLineEdit();
        QLineEdit *file_version_line = new QLineEdit();
        QLineEdit *file_url_line = new QLineEdit();
        //下拉框
        QComboBox *area_box = new QComboBox(this);
//        QComboBox *upload_num_box = new QComboBox(this);
        //布局
        QGridLayout *layout = new QGridLayout(this);
        //窗口
        QWidget *widget = new QWidget;

    private Q_SLOTS:
        void areaFill(QNetworkReply *reply);
        void clickAreaSearch();
        void clickVersionChange();
        void areaInfoFill(QNetworkReply *reply);
        void changeVersion(QNetworkReply *reply);
        void openFile();
        void uploadFile();
        void uploadFileResult(QNetworkReply *reply);
        void searchVersionResource();
        void appearVersionResource(QNetworkReply *reply);
        void userOperate();
        void createRobot();
        void createCdkey();
        void seasonCal();
        void uploadCsv();

    private:
        QNetworkAccessManager *n_manager;

};

#endif // UI_H
