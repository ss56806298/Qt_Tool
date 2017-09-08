#ifndef CSV_H
#define CSV_H

#include "ui.h"

#include <QTextBrowser>

class csv: public QMainWindow
{
    public:
        QString server_area;    //渠道组

        QString const CONFIG_CSV = "./csv.config";

        QString main_server_ip;

        QMap <QString, sd> serverMap;

        QMap <QString, QString> fileMap;    //上传的文件
    private:
        QNetworkAccessManager *c_manager;
    Q_OBJECT
    public:
        csv(ui *ui, QWidget *parent = Q_NULLPTR);

        //布局
        QGridLayout *layout = new QGridLayout;

        //button
        QPushButton *select_file_button = new QPushButton("选择文件");
        QPushButton *select_folder_button = new QPushButton("选择文件夹");
        QPushButton *upload_begin_button = new QPushButton("开始上传");

        //label
        QLabel *csv_upload_label = new QLabel("CSV");

        //富文本
        QTextBrowser *upload_file_browser = new QTextBrowser;
        QTextBrowser *upload_result_browser = new QTextBrowser;

        //下拉框
        QComboBox *server_num_box = new QComboBox;

        //窗口
        QWidget *widget = new QWidget;

    private Q_SLOTS:
        void setServerInfo(QNetworkReply *reply);
        QString getAddressByServerNum(QString server_num);

        void selectFile();
        void selectFolder();
        void showUploadFiles();
        void uploadFilesBegin();
//        void uploadFilesContinue();
//        void uploadFilesEnd();

};

#endif // CSV_H
