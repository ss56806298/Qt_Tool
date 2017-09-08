#include "ui.h"
#include "user.h"
#include "robot.h"
#include "cdkey.h"
#include "season.h"
#include "csv.h"

ui::ui(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("tool");

    //获取现在有哪些渠道组
    n_manager = new QNetworkAccessManager(this);
    connect(n_manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(areaFill(QNetworkReply *))); //关联信号和槽
    n_manager->get(QNetworkRequest(QUrl("http://" + main_server_ip + "/original_server/common/areaSearch")));//发送请求

    //布局
    int i = 0;
    layout->addWidget(area_box, i, 0, 1, 1);
    layout->addWidget(search_button, i, 1, 1, 1);
    i++;
    layout->addWidget(user_operate_button, i, 0, 1, 1);
    layout->addWidget(robot_create_button, i, 1, 1, 1);
    layout->addWidget(cdkey_create_button, i, 2, 1, 1);
    layout->addWidget(season_cal_button, i, 3, 1, 1);
    i++;
    layout->addWidget(csv_upload_button, i, 0, 1, 1);
    i++;
    layout->addWidget(area_name_label, i, 0, 1, 1);
    layout->addWidget(area_name_content_label, i, 1, 1, 1);
    i++;
    layout->addWidget(area_version_label, i, 0, 1, 1);
    layout->addWidget(area_version_content_line, i, 1, 1, 1);
    layout->addWidget(area_version_button, i, 2, 1, 1);
    layout->addWidget(area_version_change_label, i ,3 , 1, 1);
    i++;
    layout->addWidget(area_servers_label, i, 0, 1, 1);
    layout->addWidget(area_servers_content_label, i, 1, 1, 1);
    i++;
//    layout->addWidget(upload_num_box, i, 0, 1, 1);
    layout->addWidget(open_file_button, i, 0, 1, 1);
    i++;
    layout->addWidget(file_path_label, i, 0, 1, 1);
    layout->addWidget(file_path_line, i, 1, 1, 1);
    i++;
    layout->addWidget(file_size_label, i, 0, 1, 1);
    layout->addWidget(file_size_content_label, i, 1, 1, 1);
    i++;
    layout->addWidget(file_md5_label, i, 0, 1, 1);
    layout->addWidget(file_md5_content_label, i, 1, 1, 1);
    i++;
    layout->addWidget(file_version_label, i, 0, 1, 1);
    layout->addWidget(file_version_line, i, 1, 1, 1);
    layout->addWidget(search_version_button, i, 2, 1, 1);
    i++;
    layout->addWidget(file_url_label, i, 0, 1, 1);
    layout->addWidget(file_url_line, i, 1, 1, 1);
    i++;
    layout->addWidget(upload_update_resource_button, i, 0, 1, 1);
    layout->addWidget(upload_update_resource_result_label, i, 1, 1, 1);

    widget->setLayout(layout);

    setCentralWidget(widget);

    //信号
    connect(search_button, &QPushButton::clicked, this, &ui::clickAreaSearch);
    connect(area_version_button, &QPushButton::clicked, this, &ui::clickVersionChange);
    connect(open_file_button, &QPushButton::clicked, this, &ui::openFile);
    connect(upload_update_resource_button, &QPushButton::clicked, this, &ui::uploadFile);
    connect(search_version_button, &QPushButton::clicked, this, &ui::searchVersionResource);
    connect(user_operate_button, &QPushButton::clicked, this, &ui::userOperate);
    connect(robot_create_button, &QPushButton::clicked, this, &ui::createRobot);
    connect(cdkey_create_button, &QPushButton::clicked, this, &ui::createCdkey);
    connect(season_cal_button, &QPushButton::clicked, this, &ui::seasonCal);
    connect(csv_upload_button, &QPushButton::clicked, this, &ui::uploadCsv);
}

//获取渠道组信息并填充
void ui::areaFill(QNetworkReply *reply)
{
    //使用utf8编码, 这样可以显示中文
//    QTextCodec *codec = QTextCodec::codecForName("utf8");
//    QString str = codec->toUnicode(reply->readAll());

    //处理返回的JSON数据
    QJsonParseError error;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll(), &error);
    if (error.error == QJsonParseError::NoError) {
        if (jsonDocument.isObject()) {
            QVariantMap result = jsonDocument.toVariant().toMap();

            foreach (QVariant area_name, result["area_name"].toList()) {
                area_box->addItem(area_name.toString());
            }
        }
    } else {
        qDebug()<<error.errorString();
    }

    reply->deleteLater();//最后要释放replay对象
}

//将特定渠道组的version输出
void ui::areaInfoFill(QNetworkReply *reply)
{
    //处理返回的JSON数据
    QJsonParseError error;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll(), &error);
    if (error.error == QJsonParseError::NoError) {
        if (jsonDocument.isObject()) {
            QVariantMap result = jsonDocument.toVariant().toMap();

            area_name_content_label->setText(result["area_name"].toString());
            area_version_content_line->setText(result["version"].toString());
            area_servers_content_label->setText(result["server_nums"].toString());
        }
    } else {
        qDebug()<<error.errorString();
    }

    reply->deleteLater();//最后要释放replay对象
}

//点击查询按钮
void ui::clickAreaSearch()
{
    QString area = area_box->currentText();

    //获取渠道的信息
    n_manager = new QNetworkAccessManager(this);
    connect(n_manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(areaInfoFill(QNetworkReply *))); //关联信号和槽

    //拼接url
    QString url = "http://" + main_server_ip + "/original_server/common/areaVersion?area=" + area;

    n_manager->get(QNetworkRequest(QUrl(url)));//发送请求

}

//点击提交版本号按钮
void ui::clickVersionChange()
{
    QString version = area_version_content_line->text();
    QString area = area_box->currentText();
    //获取渠道的信息
    n_manager = new QNetworkAccessManager(this);
    connect(n_manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(changeVersion(QNetworkReply *))); //关联信号和槽

    //拼接url
    QString url = "http://" + main_server_ip + "/original_server/common/areaVersionChange?version=" + version + "&area=" + area;

    n_manager->get(QNetworkRequest(QUrl(url)));//发送请求

}

//改变版本号和服务器交互
void ui::changeVersion(QNetworkReply *reply)
{
    //使用utf8编码, 这样可以显示中文
    QTextCodec *codec = QTextCodec::codecForName("utf8");
    QString str = codec->toUnicode(reply->readAll());
    if (str == "success") {
        area_version_change_label->setText(str);
    }
}

//打开文件
void ui::openFile()
{
    QString path = QFileDialog::getOpenFileName(this);

    if (!path.isEmpty()) {
        QFile file(path);
        file.open(QIODevice::ReadOnly);
        int size = file.size();
        QByteArray md5Byte = QCryptographicHash::hash(file.readAll(), QCryptographicHash::Md5);
        QString md5String;
        md5String.append(md5Byte.toHex());

        QString filename = file.fileName();

        file_path_line->setText(filename);
        file_size_content_label->setText(QString::number(size, 10));
        file_md5_content_label->setText(md5String);

        file.close();
    }

}

//上传热更新资源
void ui::uploadFile()
{
    QString area = area_box->currentText();
    QString version = file_version_line->text();
    QString path = file_path_line->text();
    QString size = file_size_content_label->text();
    QString md5 = file_md5_content_label->text();
    QString s_url = file_url_line->text();

    n_manager = new QNetworkAccessManager(this);
    connect(n_manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(uploadFileResult(QNetworkReply *))); //关联信号和槽

    //拼接url
    QString url = "http://" + main_server_ip + "/original_server/common/uploadUpdateResource?version=" + version + "&area=" + area + "&path=" + path+ "&size=" + size + "&md5=" + md5+ "&url=" + s_url;

    n_manager->get(QNetworkRequest(QUrl(url)));//发送请求
}

//上传热更新资源的结果
void ui::uploadFileResult(QNetworkReply *reply)
{
    //使用utf8编码, 这样可以显示中文
    QTextCodec *codec = QTextCodec::codecForName("utf8");
    QString str = codec->toUnicode(reply->readAll());

    //获取当前文件PATH
    QString path = file_path_line->text();

    if (str == "success") {
        upload_update_resource_result_label->setText(path + " " + str);
    }
}

//查询当前版本的资源
void ui::searchVersionResource(){
    QString area = area_box->currentText();
    QString version = file_version_line->text();

    n_manager = new QNetworkAccessManager(this);
    connect(n_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(appearVersionResource(QNetworkReply *)));

    //拼接URL
    QString url = "http://" + main_server_ip + "/original_server/common/versionResourceSearch?version=" + version + "&area=" + area;

    n_manager->get(QNetworkRequest(QUrl(url)));
}

//将查询到的版本资源显示出来
void ui::appearVersionResource(QNetworkReply *reply) {
    QString version = file_version_line->text();

    //使用utf8编码, 这样可以显示中文
//    QTextCodec *codec = QTextCodec::codecForName("utf8");
//    QString str = codec->toUnicode(reply->readAll());

    QMainWindow *windowR = new QMainWindow;

    windowR->setWindowTitle(version);

    QGridLayout *layout_version = new QGridLayout();

    QWidget *widget_version = new QWidget;

    QLabel *path_resource_title_label = new QLabel("路径");
    QLabel *size_resource_title_label = new QLabel("文件大小");
    QLabel *md5_resource_title_label = new QLabel("MD5值");
    QLabel *url_resource_title_label = new QLabel("资源下载链接");

    layout_version->addWidget(path_resource_title_label, 0, 0, 1, 1);
    layout_version->addWidget(size_resource_title_label, 0, 1, 1, 1);
    layout_version->addWidget(md5_resource_title_label, 0, 2, 1, 1);
    layout_version->addWidget(url_resource_title_label, 0, 3, 1, 1);

    //JSON数据放到新窗口
    QJsonParseError error;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll(), &error);
    if (error.error == QJsonParseError::NoError) {
        if (jsonDocument.isObject()) {
            QVariantMap result = jsonDocument.toVariant().toMap();
            int i = 1;
            foreach (QVariant updateResourceT, result["update_resource"].toList()) {
//                QString path_resource = updateResource["path"].toString();
//                QString size_resource = updateResource["size"].toString();
//                QString md5_resource = updateResource["md5"].toString();
//                QString url_resource = updateResource["url"].toString();
                QVariantMap updateResource = updateResourceT.toMap();
                QLabel *path_resource_label = new QLabel(updateResource["path"].toString());
                QLabel *size_resource_label = new QLabel(updateResource["size"].toString());
                QLabel *md5_resource_label = new QLabel(updateResource["md5"].toString());
                QLabel *url_resource_label = new QLabel(updateResource["url"].toString());

                layout_version->addWidget(path_resource_label, i, 0, 1, 1);
                layout_version->addWidget(size_resource_label, i, 1, 1, 1);
                layout_version->addWidget(md5_resource_label, i, 2, 1, 1);
                layout_version->addWidget(url_resource_label, i, 3, 1, 1);

                i++;
            }
        }
    }

    widget_version->setLayout(layout_version);

    windowR->setCentralWidget(widget_version);

    windowR->show();
}

//打开用户操作的界面
void ui::userOperate() {
    user *u = new user(this);

    u->show();
}

//打开生成机器人的界面
void ui::createRobot() {
    robot *r = new robot(this);

    r->show();
}


//打开生成cdkey的界面
void ui::createCdkey() {
    cdkey *c = new cdkey(this);

    c->show();
}

//打开结算赛季的界面
void ui::seasonCal() {
    season *s = new season(this);

    s->show();
}

//打开上传CSV的界面
void ui::uploadCsv() {
    csv *c = new csv(this);

    c->show();
}
