#ifndef LICENSEDIALOG_H
#define LICENSEDIALOG_H

#include <QDialog>

class QLineEdit;
class QPushButton;
class MainWindow;
class QLabel;
class LicenseDialog : public QDialog
{
     Q_OBJECT
public:
    LicenseDialog(MainWindow *parent = Q_NULLPTR);

public slots:
    void okClicked(bool);
    void cancelClicked(bool);
    void linkClicked(QString url);
    void emailChanged(QString);
    void keyChanged(QString);

private:
    QPushButton *m_okButton;
    QLineEdit *m_email;
    QLineEdit *m_key;
    MainWindow *m_mainWindow;
    QLabel *m_errorMsg;
};

#endif // LICENSEDIALOG_H
