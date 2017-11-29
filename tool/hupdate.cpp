#include "hupdate.h"
#include <QListView>
#include <QTreeView>
#include "windows.h"
#include <QEventLoop>
#include <time.h>
#include <qDebug>

hupdate::hupdate(Ui *ui, QWidget *parent) {

    server_area = ui->area_box->currentText();

    setWindowTitle(server_area);

    main_server_ip = ui->main_server_ip;

    int i = 0;
    layout->addWidget(select_file_button, i, 0, 1, 1);
    select_file_button->setEnabled(false);
    layout->addWidget(upload_begin_button, i, 1, 1, 1);
    upload_begin_button->setEnabled(false);
    i++;
    layout->addWidget(select_folder_button, i, 0, 1, 1);
    layout->addWidget(bucket_widget, i, 1, 1, 1);
    i++;
    layout->addWidget(version_widget, i, 0, 1, 1);
    layout->addWidget(bucket_add_widget, i, 1, 1, 1);
    i++;
    layout->addWidget(upload_file_browser, i, 0, 1, 1);
    layout->addWidget(upload_result_browser, i, 1, 1, 1);

    version_layout->addWidget(version_label);
    version_layout->addWidget(version_line);

    bucket_layout->addWidget(bucket_label);
    bucket_layout->addWidget(bucket_box);

    bucket_add_layout->addWidget(bucket_add_line);
    bucket_add_layout->addWidget(bucket_add_button);

    //获取现在有哪些BUCKET
    h_manager = new QNetworkAccessManager(this);
    QUrl url = "http://" + main_server_ip + "/original_server/common/qiniuMAuth?url_string=http://rs.qbox.me/buckets&body&content_type=application/x-www-form-urlencoded";
    connect(h_manager, &QNetworkAccessManager::finished, this, &hupdate::getMAuth);
    h_manager->get(QNetworkRequest(url));
    upload_result_browser->append("上传组件初始化中");

    bucket_widget->setLayout(bucket_layout);
    version_widget->setLayout(version_layout);
    bucket_add_widget->setLayout(bucket_add_layout);

    widget->setLayout(layout);

    setCentralWidget(widget);

    connect(select_file_button, &QPushButton::clicked, this, &hupdate::selectFile);
    connect(select_folder_button, &QPushButton::clicked, this, &hupdate::selectFolder);
    connect(upload_begin_button, &QPushButton::clicked, this, &hupdate::uploadFilesBegin);
    connect(bucket_add_button, &QPushButton::clicked, this, &hupdate::addBucket);
}

//选择文件
void hupdate::selectFile() {
    QString path = QFileDialog::getOpenFileName(this);

    //判断文件是否存在
    if (!path.isEmpty()) {
        QFileInfo fileInfo(path);
        fileMap[fileInfo.fileName()] = fileInfo.filePath();
    }

    showUploadFiles();
}

//选择文件夹
void hupdate::selectFolder() {
    QString path = QFileDialog::getExistingDirectory(this);

    traversalFolder(path, "");

    showUploadFiles();
}

//将文件夹下面的文件目录打印出来
void hupdate::traversalFolder(QString path, QString relative_path) {
    QDir dir(path);

    //判断路径是否存在
    if (!dir.exists()) {
        return;
    }

    QFileInfoList list = dir.entryInfoList();
    QString FileName;
    QString FilePath;

    for (int i=0;i<list.length();i++) {
        QFileInfo fileInfo = list.at(i);
        FileName = fileInfo.fileName();
        FilePath = fileInfo.filePath();
        //目录削除
        if (FileName == "." || FileName == "..") {
            continue;
        }
        //如果is文件夹
        if (fileInfo.isDir()) {
            traversalFolder(FilePath, relative_path + FileName + "/");
        } else {
            fileMap[relative_path + FileName] = FilePath;
        }
    }
}

//显示处于上传中的文件
void hupdate::showUploadFiles() {
    upload_file_browser->clear();

    QMap <QString, QString>::iterator i;
    for (i=fileMap.begin();i!=fileMap.end();i++) {
        upload_file_browser->append(i.key());
    }
}

//上传文件开始
void hupdate::uploadFilesBegin() {
    if (fileMap.size() == 0) {
        return;
    }

    QString bucket = bucket_box->currentText();

    QNetworkReply *reply;
    QNetworkRequest request;
    QEventLoop loop;

    //获取存储空间地址凭证
    upload_result_browser->append("获取存储空间域名凭证");
    h_manager = new QNetworkAccessManager(this);

    QUrl url = "http://" + main_server_ip + "/original_server/common/qiniuGetAddressAuth?url_string=http://api.qiniu.com/v6/domain/list?tbl=" + bucket + "&bucket=" + bucket + "&content_type=application/x-www-form-urlencoded";
    connect(h_manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    reply = h_manager->get(QNetworkRequest(url));
    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        upload_result_browser->append("获取存储空间域名凭证失败,请联系管理员");
        return;
    }
    upload_result_browser->append("凭证获取成功");

    //获取存储空间地址
    upload_result_browser->append("获取存储空间域名中");
    request.setUrl(QUrl("http://api.qiniu.com/v6/domain/list?tbl=" + bucket));
    request.setRawHeader("Authorization", "QBox " + reply->readAll());
    connect(h_manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    reply = h_manager->get(request);
    loop.exec();
    if (reply->error() != QNetworkReply::NoError) {
        upload_result_browser->append("获取存储空间域名失败,请联系管理员");
        return;
    }
    QJsonParseError error;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll(), &error);
    if (error.error != QJsonParseError::NoError) {
        upload_result_browser->append("数据解析失败,请联系管理员");
        return;
    }
    QVariantList address_lists = jsonDocument.toVariant().toList();
    QString zone_address = address_lists.at(0).toString();
    upload_result_browser->append("获取存储空间成功");

        //开始循环
        QMap <QString, QString>::iterator i;

        for (i=fileMap.begin();i!=fileMap.end();i++) {
            //读取文件
            QFile file(i.value());
            file.open(QIODevice::ReadOnly);
            QByteArray file_content = file.readAll();

            //上传凭证创建
            QString relative_path = i.key();
            QFileInfo fileinfo(i.value());
            QString filename = fileinfo.fileName();

            upload_result_browser->append("开始上传:" + filename);

            QString uploadToken = createUploadToken(bucket, filename);
            if (uploadToken == "") {
                upload_result_browser->append(filename + "上传失败");
                continue;
            }

            //开始请求
            QString boundary = "EmiriatanMajitenshi";

            request.setUrl(QUrl("http://up-z2.qiniu.com"));
            request.setRawHeader("Content-Type",QString("multipart/form-data; boundary=" + boundary).toUtf8());

            QByteArray postData;

            //<uploadToken>
            postData.append("--" + boundary + "\r\n");
            postData.append("Content-Disposition:       form-data; name=\"token\"\r\n");
            postData.append("\r\n");
            postData.append(uploadToken + "\r\n");
            //<key>
            postData.append("--" + boundary + "\r\n");
            postData.append("Content-Disposition:       form-data; name=\"key\"\r\n");
            postData.append("\r\n");
            postData.append(filename + "\r\n");
            //<file>
            postData.append("--" + boundary + "\r\n");
            postData.append("Content-Disposition:       form-data; name=\"file\"; filename=\""+ filename +"\"\r\n");
            postData.append("Content-Type:              application/octet-stream\r\n");
            postData.append("Content-Transfer-Encoding: binary\r\n");
            postData.append("\r\n");
            postData.append(file_content + "\r\n");
            postData.append("--" + boundary + "--" + "\r\n");

            connect(h_manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);

            reply = h_manager->post(request, postData);
            loop.exec();

            if (reply->error() == QNetworkReply::NoError) {
                upload_result_browser->append("七牛资源上传成功");
            } else {
                qDebug() << reply->readAll();
                upload_result_browser->append("七牛资源上传失败");
                continue;
            }

            //将资源导入到数据库
            upload_result_browser->append("正在将资源地址导入到账号服务器");
            QString version = version_line->text();
            QString size = QString::number(file.size(), 10);
            QByteArray md5Byte = QCryptographicHash::hash(file.readAll(), QCryptographicHash::Md5);
            QString md5String;
            md5String.append(md5Byte.toHex());

            //拼接url
            QString file_address = "http://" + zone_address + "/" + filename;
            QString url = "http://" + main_server_ip + "/original_server/common/uploadUpdateResource?version=" + version + "&area=" + server_area + "&path=" + relative_path+ "&size=" + size + "&md5=" + md5String + "&url=" + file_address;
            connect(h_manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);

            reply = h_manager->get(QNetworkRequest(QUrl(url)));
            loop.exec();

            QTextCodec *codec = QTextCodec::codecForName("utf8");
            QString str = codec->toUnicode(reply->readAll());

            if (reply->error() != QNetworkReply::NoError || str != "success") {
                upload_result_browser->append("导入失败,请联系管理员");
                return;
            }
            upload_result_browser->append("资源上传成功");

            h_manager->clearConnectionCache();
            h_manager->clearAccessCache();

            reply->deleteLater();//最后要释放reply对象
        }

}

//上传凭证创建
QString hupdate::createUploadToken(QString bucket, QString filename) {
    QNetworkReply *reply;
    QEventLoop loop;

    h_manager = new QNetworkAccessManager(this);

    connect(h_manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);

    reply = h_manager->get(QNetworkRequest(QUrl("http://" + main_server_ip + "/original_server/common/qiniuUAuth?bucket=" + bucket + "&filename=" + filename)));

    loop.exec();

    QString result;
    if (reply->error() == QNetworkReply::NoError) {
        upload_result_browser->append("获取上传凭证成功");
        result = reply->readAll();
    } else {
        upload_result_browser->append("获取上传凭证失败");
        result = "";
    }

    h_manager->clearConnectionCache();
    h_manager->clearAccessCache();

    reply->deleteLater();//最后要释放reply对象

    return result;
}

//获取管理凭证
void hupdate::getMAuth(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        upload_result_browser->append("初始化成功");

        h_manager = new QNetworkAccessManager();
        QUrl url = QUrl("http://rs.qbox.me/buckets");
        QNetworkRequest request;
        request.setUrl(url);
        request.setRawHeader("Authorization", "QBox " + reply->readAll());
        connect(h_manager, &QNetworkAccessManager::finished, this, &hupdate::getBuckets);
        h_manager->get(request);
        upload_result_browser->append("正在获取现有的存储空间");
    } else {
        upload_result_browser->append("初始化失败,请联系管理员");
    }
}

//获取现有的buckets
void hupdate::getBuckets(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        //解析JSON
        QJsonParseError error;
        QByteArray json_byte = reply->readAll();
        QJsonDocument jsonDocument = QJsonDocument::fromJson(json_byte, &error);
        if (error.error == QJsonParseError::NoError) {
            QVariantList buckets = jsonDocument.toVariant().toList();

            foreach (QVariant bucket, buckets) {
                bucket_box->addItem(bucket.toString());
            }

            upload_result_browser->append("获取存储空间成功,可以开始上传");
            upload_begin_button->setEnabled(true);

        } else if (json_byte == "null"){
             upload_result_browser->append("存储空间为空,可以开始上传");
             upload_begin_button->setEnabled(true);
        } else {
            upload_result_browser->append("获取存储空间失败,请联系管理员");
        }
    } else {
        upload_result_browser->append("获取存储空间失败,请联系管理员");
    }
}

//增加新的BUCKET
void hupdate::addBucket() {
    QString bucket = bucket_add_line->text();

    if (bucket == "") {
        upload_result_browser->append("存储空间添加失败,名字不能为空");
        return;
    }
    upload_result_browser->append("正在获取凭证");
    h_manager = new QNetworkAccessManager(this);
    QNetworkRequest request;
    QNetworkReply *reply;
    QEventLoop loop;

    request.setUrl(QUrl("http://" + main_server_ip + "/original_server/common/qiniuAddBucketAuth?url_string=http://rs.qiniu.com/mkbucketv2/&bucket=" + bucket + "&content_type=application/x-www-form-urlencoded"));
    connect(h_manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);

    reply = h_manager->get(request);
    loop.exec();
    if (reply->error() == QNetworkReply::NoError) {
        QJsonParseError json_error;
        QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll(), &json_error);
        if (json_error.error != QJsonParseError::NoError) {
            upload_result_browser->append("数据解析失败");
            return;
        }
        upload_result_browser->append("凭证获取成功");
        QVariantMap result = jsonDocument.toVariant().toMap();

        QByteArray accessToken;
        accessToken.append(result["access_token"].toString());
        QString bucketEncoded = result["bucket_encoded"].toString();
        //创建存储空间
        request.setUrl(QUrl("http://rs.qiniu.com/mkbucketv2/" + bucketEncoded));
        request.setRawHeader("Authorization", "QBox " + accessToken);
        QByteArray postData;
//        postData.append("EncodeBucketName=" + bucketEncoded);
        connect(h_manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
        reply = h_manager->post(request, postData);

        loop.exec();

        if (reply->error() == QNetworkReply::NoError) {
            bucket_box->addItem(bucket);
            upload_result_browser->append("存储区域创建成功");
        } else {
            upload_result_browser->append("创建失败,请确认空间名称不含有非法字符");
        }
    } else {
        upload_result_browser->append("凭证获取失败");
    }
}

//    //上传凭证创建
//    Qiniu_Mac mac;
//    mac.accessKey = qiniu_access_key;
//    mac.secretKey = qiniu_secret_key;

//    //开始循环
//    QMap <QString, QString>::iterator i;

//    for (i=fileMap.begin();i!=fileMap.end();i++) {

//        char *bucket = version.toUtf8().data();
//        char *key = i.key().toUtf8().data();
//        char *localFile = i.value().toUtf8().data();

//        Qiniu_Io_PutRet putRet;
//        Qiniu_Client client;
//        Qiniu_RS_PutPolicy putPolicy;
//        Qiniu_Io_PutExtra putExtra;
//        Qiniu_Zero(putPolicy);
//        Qiniu_Zero(putExtra);

//        putPolicy.scope = bucket;

//        char *uptoken = Qiniu_RS_PutPolicy_Token(&putPolicy, &mac);
//        //设置机房域名
//        //Qiniu_Use_Zone_Beimei(Qiniu_False);
//        //Qiniu_Use_Zone_Huabei(Qiniu_True);
////        Qiniu_Use_Zone_Huadong(Qiniu_False);
////        Qiniu_Use_Zone_Huanan(Qiniu_True);

//        Qiniu_Client_InitMacAuth(&client, 1024, &mac);
//        Qiniu_Error error = Qiniu_Io_PutFile(&client, &putRet, uptoken, key, localFile, &putExtra);
//        if (error.code != 200) {
//            upload_result_browser->append("七牛资源上传发生错误");
//            upload_result_browser->append("err_code:" + QString::number(error.code, 10));
//            upload_result_browser->append("err_msg:" + QString(error.message));
//        } else {
//          /*200, 正确返回了, 你可以通过statRet变量查询一些关于这个文件的信息*/
//          upload_result_browser->append("七牛上传失败");
//        }
//        Qiniu_Free(uptoken);
//        Qiniu_Client_Cleanup(&client);

//    Qiniu_Io_PutRet putRet;
//    Qiniu_Client client;
//    Qiniu_RS_PutPolicy putPolicy;
//    Qiniu_Io_PutExtra putExtra;
//    Qiniu_Zero(putPolicy);
//    Qiniu_Zero(putExtra);
//    putPolicy.scope = bucket;
//    char *uptoken = Qiniu_RS_PutPolicy_Token(&putPolicy, &mac);
//    //设置机房域名
//    //Qiniu_Use_Zone_Beimei(Qiniu_False);
//    //Qiniu_Use_Zone_Huabei(Qiniu_True);
//    Qiniu_Use_Zone_Huadong(Qiniu_False);
//    //Qiniu_Use_Zone_Huanan(Qiniu_True);
//    Qiniu_Client_InitMacAuth(&client, 1024, &mac);
//    Qiniu_Error error = Qiniu_Io_PutFile(&client, &putRet, uptoken, key, localFile, &putExtra);
//    if (error.code != 200) {
//      printf("upload file %s:%s error.\n", bucket, key);
//      debug_log(&client, error);
//    } else {
//      /*200, 正确返回了, 你可以通过statRet变量查询一些关于这个文件的信息*/
//      printf("upload file %s:%s success.\n\n", bucket, key);
//      printf("key:\t%s\n",putRet.key);
//      printf("hash:\t%s\n", putRet.hash);
//    }
//    Qiniu_Free(uptoken);
//    Qiniu_Client_Cleanup(&client);

//    QMap <QString, QString>::iterator i;

//    h_manager = new QNetworkAccessManager();

//    QNetworkReply *reply;

//    for (i=fileMap.begin();i!=fileMap.end();i++) {
//        QEventLoop loop; //阻塞事件

//        //读取文件
//        QFile file(i.value());
//        file.open(QIODevice::ReadOnly);
//        int file_len = file.size();
//        QDataStream in(&file);
//        char* m_buf = new char[file_len];
//        in.readRawData(m_buf, file_len);
//        file.close();

//        //交互
//        QNetworkRequest request;
//        request.setUrl("http://" + server_address + "/The_Wall/common/uploadCsv?filename=" + i.key());
//        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
//        QByteArray arr = QByteArray(m_buf, file_len);
//        connect(c_manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
//        reply = c_manager->post(request, arr);
//        loop.exec();
//        if (reply->error() == QNetworkReply::NoError){
//            upload_result_browser->append(i.key() + ":" + reply->readAll());
//        } else {
//            upload_result_browser->append(i.key() + ":网络异常上传失败");
//        }
//    }
//    for (int i = 0; i<=100;i++) {
//        reply  = c_manager->get(QNetworkRequest(QUrl("http://" + main_server_ip + "/original_server/common/areaSearch")));
//        QEventLoop loop;
//        connect(c_manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
//        loop.exec();
//        qDebug() <<reply->readAll();

//    }

//    uploadFilesContinue();
//}
