#include "UpdaterDialog.h"
#include "ui_UpdaterDialog.h"

#include <QtCore/QDebug>
#include <QtCore/QUrl>
#include <QtCore/QFileInfo>
#include <QApplication>
#include <QtCore/QDir>
#include <QtCore/QCryptographicHash>

#include "moc_UpdaterDialog.cxx"

UpdaterDialog::UpdaterDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::UpdaterDialog),
    m_state(Initializing),
    m_timer(),
    m_http(new QHttp(this))
{
    m_ui->setupUi(this);

    m_timer.setInterval(50);
    m_timer.setSingleShot(false);
    m_timer.start();
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(updateScreen()));

    connect(m_http, SIGNAL(requestFinished(int, bool)),
            this, SLOT(httpRequestFinished(int, bool)));
    connect(m_http, SIGNAL(dataReadProgress(int, int)),
            this, SLOT(updateDataReadProgress(int, int)));
    connect(m_http, SIGNAL(responseHeaderReceived(const QHttpResponseHeader &)),
            this, SLOT(readResponseHeader(const QHttpResponseHeader &)));

    updateScreen();
}

UpdaterDialog::~UpdaterDialog()
{
    delete m_ui;
}

void UpdaterDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void UpdaterDialog::on_btnCancel_clicked()
{
    m_state = Cancelling;
    updateScreen();
}

void UpdaterDialog::updateScreen()
{
    m_ui->btnCancel->setDisabled(m_state == Cancelling);

    switch(m_state) {
        case Initializing:
            // download the files list from the internet
            m_ui->lblStatus->setText(tr("Downloading file index..."));
            m_state = CheckingFilesList;
            downloadFile("http://motrs.superjoesoftware.com/files", "file_index");
            break;
        case CheckingFilesList:
            // go through the list, checking the hash with the hash of the local
            // file. Build a list of DownloadQueueItems
            if( m_downloadFinished ) {
                QFile file("file_index");
                if( ! file.open(QIODevice::ReadOnly | QIODevice::Text) ){
                    m_state = Error;
                    m_ui->lblStatus->setText(tr("Error opening index file for reading."));
                    updateScreen();
                    return;
                }

                // parse the file index
                QDir dir(QApplication::applicationDirPath());
                m_queue.clear();
                QTextStream in(&file);
                while( ! in.atEnd() ) {
                    QString line = in.readLine().trimmed();
                    if( line.size() == 0 )
                        continue;

                    QStringList data = line.split(" ");
                    DownloadQueueItem item;
                    item.md5sum = data.takeFirst();
                    item.destination = data.join("");
                    item.url = QString("http://motrs.superjoesoftware.com/release/newest/world-editor/") + item.destination;
                    m_queue.append(item);
                }

                // delete queue items that are already updated
                for(int i=0; i<m_queue.size(); ++i) {
                    DownloadQueueItem * item = &m_queue[i];
                    QString localmd5 = md5sum(item->destination);
                    if( item->md5sum.compare(localmd5) == 0 ) {
                        // delete this index
                        m_queue.remove(i);
                        --i;
                        continue;
                    }
                }
                m_maxQueueSize = m_queue.size();

                m_ui->lblStatus->setText(tr("Downloading updates..."));
                m_state = DownloadingUpdates;
                updateScreen();
                return;
            }
            break;
        case DownloadingUpdates:
            if( m_downloadFinished ) {
                if( m_queue.size() > 0 ) {
                    DownloadQueueItem & item = m_queue.last();
                    downloadFile(item.url, item.destination);
                    m_queue.remove(m_queue.size()-1);
                    m_ui->lblStatus->setText(
                        tr("Downloading updates... %1 of %2").arg(
                            QString::number(m_maxQueueSize-m_queue.size()),
                            QString::number(m_maxQueueSize)));
                } else {
                    m_state = Done;
                    m_ui->lblStatus->setText(tr("Successful update."));
                    m_ui->btnCancel->setText(tr("&Close"));
                }
            }
            break;
        case Done:
            // do nothing
            break;
        case Cancelling:
            m_http->abort();
            this->reject();
            break;
        case Error:
            // do nothing
            break;
        default:
            qDebug() << "Error: unknown state: " << m_state;
    }
}

void UpdaterDialog::downloadFile(QString _url, QString _outPath)
{
    m_ui->pgbStatus->setDisabled(false);
    m_ui->pgbStatus->reset();

    QUrl url(_url);
    QFileInfo fileInfo(url.path());
    QString fileName = fileInfo.fileName();
    if (fileName.isEmpty())
        fileName = "index.html";

    if (QFile::exists(_outPath))
        QFile::remove(_outPath);

    QDir(QApplication::applicationDirPath()).mkpath(QFileInfo(_outPath).path());
    m_file = new QFile(_outPath);
    if (!m_file->open(QIODevice::WriteOnly)) {
        m_state = Error;
        m_ui->lblStatus->setText(tr("Unable to save %1: %2.")
                                 .arg(fileName).arg(m_file->errorString()));
        delete m_file;
        m_file = NULL;
        return;
    }

    QHttp::ConnectionMode mode = url.scheme().toLower() == "https" ? QHttp::ConnectionModeHttps : QHttp::ConnectionModeHttp;
    m_http->setHost(url.host(), mode, url.port() == -1 ? 0 : url.port());

    if (!url.userName().isEmpty())
        m_http->setUser(url.userName(), url.password());

    m_httpRequestAborted = false;
    QByteArray path = QUrl::toPercentEncoding(url.path(), "!$&'()*+,;=:@/");
    if (path.isEmpty())
        path = "/";
    m_httpGetId = m_http->get(path, m_file);
    m_downloadFinished = false;
}

void UpdaterDialog::httpRequestFinished(int requestId, bool error)
{
    if (requestId != m_httpGetId)
        return;
    if (m_httpRequestAborted) {
        if (m_file) {
            m_file->close();
            m_file->remove();
            delete m_file;
            m_file = NULL;
        }
        return;
    }

    if (requestId != m_httpGetId)
        return;

    m_ui->pgbStatus->setDisabled(true);
    m_file->close();
    m_downloadFinished = true;

    if (error) {
        m_file->remove();
        m_ui->lblStatus->setText(tr("Download failed: %1.")
                                 .arg(m_http->errorString()));
        m_state = Error;
    }

    delete m_file;
    m_file = NULL;
}

void UpdaterDialog::readResponseHeader(const QHttpResponseHeader &responseHeader)
{
    switch (responseHeader.statusCode()) {
    case 200:                   // Ok
    case 301:                   // Moved Permanently
    case 302:                   // Found
    case 303:                   // See Other
    case 307:                   // Temporary Redirect
        // these are not error conditions
        break;

    default:
        m_state = Error;
        m_ui->lblStatus->setText(tr("Download failed: %1.")
                                 .arg(responseHeader.reasonPhrase()));
        m_httpRequestAborted = true;
        m_ui->pgbStatus->setDisabled(true);
        m_http->abort();
    }
}

void UpdaterDialog::updateDataReadProgress(int bytesRead, int totalBytes)
{
    if (m_httpRequestAborted)
        return;

    m_ui->pgbStatus->setMinimum(0);
    m_ui->pgbStatus->setMaximum(totalBytes);
    m_ui->pgbStatus->setValue(bytesRead);
}

QString UpdaterDialog::md5sum(QString filename)
{
    QFile file(filename);
    if( ! file.open(QIODevice::ReadOnly) ) {
        qDebug() << "Error opening " << filename << " for input.";
        return QString();
    }

    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(file.readAll());
    return hash.result().toHex();
}
