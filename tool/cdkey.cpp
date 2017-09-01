#include "cdkey.h"

#include <QMessageBox>

cdkey::cdkey(ui *ui, QWidget *parent)
{
    setWindowTitle("cdkey");

    main_server_ip = ui->main_server_ip;
    //布局
    int i = 0;
    layout->addWidget(create_num_label, i, 0, 1, 1);
    layout->addWidget(create_num_line, i, 1, 1, 1);
    i++;
    layout->addWidget(event_id_label, i, 0, 1, 1);
    layout->addWidget(event_id_line, i, 1, 1, 1);
    layout->addWidget(event_id_warning_label, i, 2, 1, 1);
    i++;
    layout->addWidget(event_comment_label, i, 0, 1, 1);
    layout->addWidget(event_comment_line, i, 1, 1, 1);
    i++;
    layout->addWidget(platform_label, i, 0, 1, 1);
    layout->addWidget(platform_line, i, 1, 1, 1);
    i++;
    layout->addWidget(used_times_label, i, 0, 1, 1);
    layout->addWidget(used_times_line, i, 1, 1, 1);
    layout->addWidget(used_times_warning_label, i, 2, 1, 1);
    i++;
    layout->addWidget(start_time_label, i, 0, 1, 1);
    layout->addWidget(start_time_line, i, 1, 1, 1);
    i++;
    layout->addWidget(end_time_label, i, 0, 1, 1);
    layout->addWidget(end_time_line, i, 1, 1, 1);
    i++;
    layout->addWidget(reward_id_label, i, 0, 1, 1);
    layout->addWidget(reward_num_label, i, 1, 1, 1);
    i++;
    layout->addWidget(reward_id1_line, i, 0, 1, 1);
    layout->addWidget(reward_num1_line, i, 1, 1, 1);
    i++;
    layout->addWidget(reward_id2_line, i, 0, 1, 1);
    layout->addWidget(reward_num2_line, i, 1, 1, 1);
    i++;
    layout->addWidget(reward_id3_line, i, 0, 1, 1);
    layout->addWidget(reward_num3_line, i, 1, 1, 1);
    i++;
    layout->addWidget(reward_id4_line, i, 0, 1, 1);
    layout->addWidget(reward_num4_line, i, 1, 1, 1);
    i++;
    layout->addWidget(reward_id5_line, i, 0, 1, 1);
    layout->addWidget(reward_num5_line, i, 1, 1, 1);
    i++;
    layout->addWidget(submit_add_cdkeys_button, i, 0, 1, 1);
    i++;
    layout->addWidget(create_result, i, 0, 3, 3);

    widget->setLayout(layout);

    setCentralWidget(widget);

//    QFile file("./ss.csv");

//    file.open(QIODevice::ReadWrite|QIODevice::Append);

//    file.write("dd,哈哈\n");
//    file.write("1,1\n");
//    file.close();
    //信号
    connect(submit_add_cdkeys_button, &QPushButton::clicked, this, &cdkey::beginCreate);
    connect(event_id_line, &QLineEdit::editingFinished, this, &cdkey::searchEventId);
}

//开始生成cdkey
void cdkey::beginCreate() {
    submit_add_cdkeys_button->setEnabled(false);

    create_num_line->setEnabled(false);
    event_id_line->setEnabled(false);
    event_comment_line->setEnabled(false);
    platform_line->setEnabled(false);
    used_times_line->setEnabled(false);
    start_time_line->setEnabled(false);
    end_time_line->setEnabled(false);
    reward_id1_line->setEnabled(false);
    reward_id2_line->setEnabled(false);
    reward_id3_line->setEnabled(false);
    reward_id4_line->setEnabled(false);
    reward_id5_line->setEnabled(false);
    reward_num1_line->setEnabled(false);
    reward_num2_line->setEnabled(false);
    reward_num3_line->setEnabled(false);
    reward_num4_line->setEnabled(false);
    reward_num5_line->setEnabled(false);

    //生成再开
    QString num = create_num_line->text();
    create_num = num.toInt();

    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString str = time.toString("yyyy-MM-dd-hh-mm-ss"); //设置显示格式

    file = new QFile("./csv/" + str + ".csv");
    file->open(QIODevice::ReadWrite|QIODevice::Append);

    file->write("cdkey\n");

    continueCreate();
}

//生成再开
void cdkey::continueCreate() {

    c_manager = new QNetworkAccessManager(this);

    //整理下参数
    QString event_id = event_id_line->text();
    QString event_comment = event_comment_line->text();
    QString platform = platform_line->text();
    QString used_times = used_times_line->text();
    QString start_time = start_time_line->text();
    QString end_time = end_time_line->text();

    QVariantList rewards;

    QString reward_id1 = reward_id1_line->text();
    int reward_num1 = reward_num1_line->text().toInt();
    if (reward_id1 != "" && reward_num1 != 0) {
        QVariantMap reward;
        reward.insert("id", reward_id1);
        reward.insert("num", reward_num1);

        rewards.append(reward);
    }
    QString reward_id2 = reward_id2_line->text();
    int reward_num2 = reward_num2_line->text().toInt();
    if (reward_id2 != "" && reward_num2 != 0) {
        QVariantMap reward;
        reward.insert("id", reward_id2);
        reward.insert("num", reward_num2);

        rewards.append(reward);
    }
    QString reward_id3 = reward_id3_line->text();
    int reward_num3 = reward_num3_line->text().toInt();
    if (reward_id3 != "" && reward_num3 != 0) {
        QVariantMap reward;
        reward.insert("id", reward_id3);
        reward.insert("num", reward_num3);

        rewards.append(reward);
    }
    QString reward_id4 = reward_id4_line->text();
    int reward_num4 = reward_num4_line->text().toInt();
    if (reward_id4 != "" && reward_num4 != 0) {
        QVariantMap reward;
        reward.insert("id", reward_id4);
        reward.insert("num", reward_num4);

        rewards.append(reward);
    }
    QString reward_id5 = reward_id5_line->text();
    int reward_num5 = reward_num5_line->text().toInt();
    if (reward_id5 != "" && reward_num5 != 0) {
        QVariantMap reward;
        reward.insert("id", reward_id5);
        reward.insert("num", reward_num5);

        rewards.append(reward);
    }

    QMap <QString, QVariant> reward_items;
    reward_items.insert("reward_items", rewards);
    QJsonDocument jsonDocument = QJsonDocument::fromVariant(QVariant(reward_items));

    QString reward_items_string = jsonDocument.toJson(QJsonDocument::Compact);

    connect(c_manager, &QNetworkAccessManager::finished, this, &cdkey::resultAppear);

    QByteArray postData;

    postData.append("event_id=" + event_id);
    postData.append("&event_comment=" + event_comment);
    postData.append("&platform=" + platform);
    postData.append("&used_times=" + used_times);
    postData.append("&start_time=" + start_time);
    postData.append("&end_time=" + end_time);
    postData.append("&reward_items=" + reward_items_string);

    QUrl url = "http://" + main_server_ip + "/original_server/common/createCdkey";

    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("Content-Type","application/x-www-form-urlencoded");

    c_manager->post(request, postData);
}

//结果显示
void cdkey::resultAppear(QNetworkReply *reply) {
    create_num--;

    if(reply->error() == QNetworkReply::NoError){
        QString result = QString(reply->readAll());

        if (result.length() == 6) {

           QByteArray ba = result.toUtf8();
           char *mm = ba.data();

           file->write(mm);
           file->write("\n");

           create_result->append("创建成功,剩余" + QString::number(create_num, 10) + "个");
        } else {
           create_result->append("创建失败,剩余" + QString::number(create_num, 10) + "个");
        }

        c_manager->clearConnectionCache();
        c_manager->clearAccessCache();

    } else {
        create_result->append("创建失败,剩余" + QString::number(create_num, 10) + "个");
    }

    if (create_num <= 0) {
        endCreate();
    } else {
        continueCreate();
    }

}

//结束生成
void cdkey::endCreate() {
    submit_add_cdkeys_button->setEnabled(true);

    create_num_line->setEnabled(true);
    event_id_line->setEnabled(true);
    event_comment_line->setEnabled(true);
    platform_line->setEnabled(true);
    used_times_line->setEnabled(true);
    start_time_line->setEnabled(true);
    end_time_line->setEnabled(true);
    reward_id1_line->setEnabled(true);
    reward_id2_line->setEnabled(true);
    reward_id3_line->setEnabled(true);
    reward_id4_line->setEnabled(true);
    reward_id5_line->setEnabled(true);
    reward_num1_line->setEnabled(true);
    reward_num2_line->setEnabled(true);
    reward_num3_line->setEnabled(true);
    reward_num4_line->setEnabled(true);
    reward_num5_line->setEnabled(true);

    file->close();
}

//查询当前eventId
void cdkey::searchEventId() {
    c_manager = new QNetworkAccessManager(this);

    event_comment_line->setEnabled(false);

    QString eventId = event_id_line->text();

    connect(c_manager, &QNetworkAccessManager::finished, this, &cdkey::eventCommentAppear);

    QUrl url = "http://" + main_server_ip + "/original_server/common/eventCommentSearch?event_id=" + eventId;

    c_manager->get(QNetworkRequest(url));
}

//当前eventID的活动comment显示
void cdkey::eventCommentAppear(QNetworkReply *reply) {
    if(reply->error() == QNetworkReply::NoError){
        QString eventComment = QString(reply->readAll());

        if (eventComment != "") {
            event_comment_line->setText(eventComment);
        } else {
            event_comment_line->setEnabled(true);
        }
    } else {
        event_comment_line->setEnabled(true);
    }
    c_manager->clearConnectionCache();
    c_manager->clearAccessCache();
}
