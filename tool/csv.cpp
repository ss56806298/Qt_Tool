#include "csv.h"
#include <QListView>
#include <QTreeView>
#include "windows.h"
#include <QEventLoop>

csv::csv(Ui *ui, QWidget *parent) {
    server_area = ui->area_box->currentText();

    setWindowTitle(server_area);

    main_server_ip = ui->main_server_ip;

    //获取现在有哪些渠道组
    c_manager = new QNetworkAccessManager(this);
    connect(c_manager, &QNetworkAccessManager::finished, this, &csv::setServerInfo); //关联信号和槽

    QUrl url = "http://" + main_server_ip + "/original_server/common/serverSearch?area=" + server_area;

    c_manager->get(QNetworkRequest(url));//发送请求

    int i = 0;
    layout->addWidget(server_num_box, i, 0, 1, 1);
    i++;
    layout->addWidget(select_file_button, i, 0, 1, 1);
    layout->addWidget(upload_begin_button, i, 1, 1, 1);
    i++;
    layout->addWidget(select_folder_button, i, 0, 1, 1);
    i++;
    layout->addWidget(upload_file_browser, i, 0, 1, 1);
    layout->addWidget(upload_result_browser, i, 1, 1, 1);

    widget->setLayout(layout);

    setCentralWidget(widget);

    connect(select_file_button, &QPushButton::clicked, this, &csv::selectFile);
    connect(select_folder_button, &QPushButton::clicked, this, &csv::selectFolder);
    connect(upload_begin_button, &QPushButton::clicked, this, &csv::uploadFilesBegin);
}

//选择CSV
void csv::selectFile() {
    QString path = QFileDialog::getOpenFileName(this, QString(), QString(), "*.csv");

    //判断文件是否存在
    if (!path.isEmpty()) {
        QFileInfo fileInfo(path);
        fileMap[fileInfo.fileName()] = fileInfo.filePath();
    }

    showUploadFiles();
}

//选择文件夹
void csv::selectFolder() {
    QString path = QFileDialog::getExistingDirectory(this);

    QDir dir(path);

    //判断路径是否存在
    if (!dir.exists()) {
        return;
    }

    QStringList filters;
    filters << "*.csv";
    dir.setNameFilters(filters);

    QFileInfoList list = dir.entryInfoList();
    QString FileName;
    QString FilePath;

    for (int i=0;i<list.length();i++) {
        QFileInfo fileInfo = list.at(i);
        FileName = fileInfo.fileName();
        FilePath = fileInfo.filePath();
        fileMap[FileName] = FilePath;
    }

    showUploadFiles();
}

//显示处于上传中的文件
void csv::showUploadFiles() {
    upload_file_browser->clear();

    QMap <QString, QString>::iterator i;
    for (i=fileMap.begin();i!=fileMap.end();i++) {
        upload_file_browser->append(i.key());
    }
}

//上传文件开始
void csv::uploadFilesBegin() {
    if (fileMap.size() == 0) {
        return;
    }
    QString server_address = getAddressByServerNum(server_num_box->currentText());

    QMap <QString, QString>::iterator i;

    c_manager = new QNetworkAccessManager();

    QNetworkReply *reply;

    for (i=fileMap.begin();i!=fileMap.end();i++) {
        QEventLoop loop; //阻塞事件

        //读取CSV
        QFile file(i.value());
        file.open(QIODevice::ReadOnly);
        int file_len = file.size();
        QDataStream in(&file);
        char* m_buf = new char[file_len];
        in.readRawData(m_buf, file_len);
        file.close();

        //交互
        QNetworkRequest request;
        request.setUrl("http://" + server_address + "/The_Wall/common/uploadCsv?filename=" + i.key());
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
        QByteArray arr = QByteArray(m_buf, file_len);
        connect(c_manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
        reply = c_manager->post(request, arr);
        loop.exec();
        if (reply->error() == QNetworkReply::NoError){
            upload_result_browser->append(i.key() + ":" + reply->readAll());
        } else {
            upload_result_browser->append(i.key() + ":网络异常上传失败");
        }
    }
//    for (int i = 0; i<=100;i++) {
//        reply  = c_manager->get(QNetworkRequest(QUrl("http://" + main_server_ip + "/original_server/common/areaSearch")));
//        QEventLoop loop;
//        connect(c_manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
//        loop.exec();
//        qDebug() <<reply->readAll();

//    }

//    uploadFilesContinue();
}

//继续上传文件
//void csv::uploadFilesContinue(){
////    QString server_address = getAddressByServerNum(server_num_box->currentText());

////    c_manager = new QNetworkAccessManager();

////    QFile file(i_file.value());
////    file.open(QIODevice::ReadOnly);
////    int file_len = file.size();
////    QDataStream in(&file);
////    m_buf = new char[file_len];
////    in.readRawData(m_buf, file_len);
////    file.close();

////    QNetworkRequest request;
////    request.setUrl("http://" + server_address + "/common/uploadCsv");
////    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
////    QByteArray arr = QByteArray(m_buf, file_len);
////    QNetworkReply reply = c_manager->post(request, arr);
//}

void csv::setServerInfo(QNetworkReply *reply) {
    //处理返回的JSON数据
    QJsonParseError error;

    QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll(), &error);
    if (error.error == QJsonParseError::NoError) {
        if (jsonDocument.isObject()) {
            QVariantMap result = jsonDocument.toVariant().toMap();

            foreach(QVariant serverListT, result["server_list"].toList()) {
                QVariantMap serverList = serverListT.toMap();
                sd sd1;
                sd1.server_num = serverList["server_num"].toString();
                sd1.server_name = serverList["server_name"].toString();
                sd1.server_address = serverList["server_address"].toString();
                serverMap[serverList["server_num"].toString()] = sd1;

                server_num_box->addItem(sd1.server_num);
            }

        }
    } else {
        qDebug()<<error.errorString();
    }

    c_manager->clearConnectionCache();
    c_manager->clearAccessCache();

    reply->deleteLater();//最后要释放reply对象
}

//根据服务器的编号获取服务器的address
QString csv::getAddressByServerNum(QString server_num) {
    return serverMap[server_num].server_address;
}
