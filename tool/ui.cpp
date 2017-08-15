#include "ui.h"

ui::ui(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("tool");

    //获取现在有哪些渠道组
    n_manager = new QNetworkAccessManager(this);
    connect(n_manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(areaFill(QNetworkReply *))); //关联信号和槽
    n_manager->get(QNetworkRequest(QUrl("http://106.75.36.193:82/original_server/common/areaSearch")));//发送请求

    //布局
    layout->addWidget(area_box, 0, 0, 1, 1);
    layout->addWidget(search_button, 0, 1, 1, 1);

    layout->addWidget(area_name_label, 1, 0, 1, 1);
    layout->addWidget(area_name_content_label, 1, 1, 1, 1);
    layout->addWidget(area_version_label, 2, 0, 1, 1);
    layout->addWidget(area_version_content_line, 2, 1, 1, 1);
    layout->addWidget(area_version_button, 2, 2, 1, 1);
    layout->addWidget(area_version_change_label, 2 ,3 , 1, 1);
    layout->addWidget(area_servers_label, 3, 0, 1, 1);
    layout->addWidget(area_servers_content_label, 3, 1, 1, 1);
    layout->addWidget(upload_num_box, 4, 0, 1, 1);

    //增加上传栏目

    widget->setLayout(layout);

    setCentralWidget(widget);

    //信号
    connect(search_button, &QPushButton::clicked, this, &ui::clickAreaSearch);
    connect(area_version_button, &QPushButton::clicked, this, &ui::clickVersionChange);
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
    QString url = "http://106.75.36.193:82/original_server/common/areaVersion?area=" + area;

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
    QString url = "http://106.75.36.193:82/original_server/common/areaVersionChange?version=" + version + "&area=" + area;

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
