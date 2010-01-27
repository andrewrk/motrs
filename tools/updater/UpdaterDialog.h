#ifndef UPDATERDIALOG_H
#define UPDATERDIALOG_H

#include <QtGui/QDialog>
#include <QtCore/QVector>
#include <QtCore/QTimer>
#include <QtCore/QFile>
#include <QtNetwork/QHttp>
#include <QtNetwork/QHttpResponseHeader>

namespace Ui {
    class UpdaterDialog;
}

class UpdaterDialog : public QDialog {
    Q_OBJECT
public:
    UpdaterDialog(QWidget *parent = 0);
    ~UpdaterDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::UpdaterDialog *m_ui;

    typedef struct {
        QString url; // on remote server
        QString destination; // on local file system
        QString md5sum;
    } DownloadQueueItem;

    enum State {
        // download the files list from the internet
        Initializing,
        // go through the list, checking the hash with the hash of the local
        // file. Build a list of DownloadQueueItems
        CheckingFilesList,
        // download the queued items
        DownloadingUpdates,
        Done,
        Cancelling,
        Error,
    };

    State m_state;
    QVector<DownloadQueueItem> m_queue;
    QTimer m_timer;
    QHttp * m_http;
    QFile * m_file;
    bool m_httpRequestAborted;
    bool m_downloadFinished;
    int m_httpGetId;
    int m_maxQueueSize;

    void downloadFile(QString url, QString outpath);
    QString md5sum(QString file);


private slots:
    void on_btnCancel_clicked();
    void updateScreen();
    void httpRequestFinished(int requestId, bool error);
    void readResponseHeader(const QHttpResponseHeader &responseHeader);
    void updateDataReadProgress(int bytesRead, int totalBytes);
};

#endif // UPDATERDIALOG_H
