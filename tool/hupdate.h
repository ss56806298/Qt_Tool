#ifndef UPDATE_H
#define UPDATE_H

#include "ui.h"

#include <QTextBrowser>

class hupdate: public QMainWindow
{
    public:
        QString server_area;    //渠道组

        QString main_server_ip;

        QMap <QString, QString> fileMap;    //上传的文件

        const char *qiniu_access_key = "rx5Yhf5yf5TJQQxCiCCUq1i-XIxiVhnqftYaqFjJ";

        const char *qiniu_secret_key = "qAHwW3Vnc0StwCmZlQlBsmLTsg1J9dbJIK9T44UX";
    private:
        QNetworkAccessManager *h_manager;
    Q_OBJECT
    public:
        hupdate(Ui *ui, QWidget *parent = Q_NULLPTR);

        //布局
        QGridLayout *layout = new QGridLayout;
        QHBoxLayout *bucket_layout = new QHBoxLayout;
        QHBoxLayout *bucket_add_layout = new QHBoxLayout;
        QHBoxLayout *version_layout = new QHBoxLayout;

        //button
        QPushButton *select_file_button = new QPushButton("选择文件");
        QPushButton *select_folder_button = new QPushButton("选择文件夹");
        QPushButton *upload_begin_button = new QPushButton("开始上传");
        QPushButton *bucket_add_button = new QPushButton("添加存储空间");

        //富文本
        QTextBrowser *upload_file_browser = new QTextBrowser;
        QTextBrowser *upload_result_browser = new QTextBrowser;

        //文本输入
        QLineEdit *bucket_add_line = new QLineEdit;
        QLineEdit *version_line = new QLineEdit;

        //下拉框
        QComboBox *bucket_box = new QComboBox;

        //label
        QLabel *bucket_label = new QLabel("七牛存储空间");
        QLabel *version_label = new QLabel("版本号");

        //窗口
        QWidget *widget = new QWidget;
        QWidget *version_widget = new QWidget;
        QWidget *bucket_widget = new QWidget;
        QWidget *bucket_add_widget = new QWidget;

    private Q_SLOTS:
        void getMAuth(QNetworkReply *reply);    //获取管理凭证
        void getBuckets(QNetworkReply *reply);  //获取现在的buckets
        void addBucket();   //增加BUCKET

        void selectFile();
        void selectFolder();
        void showUploadFiles();
        void uploadFilesBegin();
//        void uploadFilesContinue();
//        void uploadFilesEnd();
        QString createUploadToken(QString bucket, QString filename);

        void traversalFolder(QString path, QString relative_path);
};

#endif // UPDATE_H
