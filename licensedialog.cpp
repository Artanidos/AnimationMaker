#include "licensedialog.h"
#include "license.h"
#include "mainwindow.h"
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDesktopServices>
#include <QUrl>

LicenseDialog::LicenseDialog(MainWindow *parent)
    : QDialog(parent)
{
    m_mainWindow = parent;
    m_errorMsg = new QLabel;
    QPalette palette = m_errorMsg->palette();
    palette.setColor(QPalette::WindowText, QColor(255, 125, 0));
    m_errorMsg->setPalette(palette);

    QPushButton *cancelButton = new QPushButton("&Cancel", this);
    connect(cancelButton, SIGNAL(clicked(bool)), this, SLOT(cancelClicked(bool)));

    m_okButton = new QPushButton("&Ok", this);
    m_okButton->setDefault(true);
    m_okButton->setEnabled(false);
    connect(m_okButton, SIGNAL(clicked(bool)), this, SLOT(okClicked(bool)));

    m_email = new QLineEdit();
    m_email->setMaxLength(50);
    m_email->setMinimumWidth(300);
    m_email->setText(m_mainWindow->email());
    connect(m_email, SIGNAL(textChanged(QString)), this, SLOT(emailChanged(QString)));

    m_key = new QLineEdit();
    m_key->setMaxLength(50);
    m_key->setText(m_mainWindow->licenseKey());
    connect(m_key, SIGNAL(textChanged(QString)), this, SLOT(keyChanged(QString)));

    QGridLayout *grid = new QGridLayout();
    QLabel *desc = new QLabel("Here you can enter your license key to switch to commercial version. The license key can be bought here: <a href='https://artanidos.github.io/AnimationMaker/#buy'>https://artanidos.github.io/AnimationMaker/#buy</a>");
    desc->setWordWrap(true);
    desc->setTextFormat(Qt::RichText);
    connect(desc, SIGNAL(linkActivated(QString)), this, SLOT(linkClicked(QString)));
    grid->addWidget(desc, 0, 0, 1, 2);
    grid->addWidget(m_errorMsg, 1, 0, 1, 2);
    grid->addWidget(new QLabel("Email"), 2, 0);
    grid->addWidget(new QLabel("License Key"), 3, 0);
    grid->addWidget(m_email, 2, 1);
    grid->addWidget(m_key, 3, 1);
    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addStretch();
    hbox->addWidget(m_okButton);
    hbox->addWidget(cancelButton);
    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addLayout(grid);
    vbox->addLayout(hbox);
    setLayout(vbox);
}

void LicenseDialog::okClicked(bool)
{
    if(checkLicense(m_email->text(), m_key->text()))
    {
        m_mainWindow->setEmail(m_email->text());
        m_mainWindow->setLicenseKey(m_key->text());
        m_mainWindow->setLicensed(true);
        this->close();
    }
    else
    {
        QPalette palette = m_key->palette();
        palette.setColor(QPalette::Base, QColor(255, 125, 0));
        palette.setColor(QPalette::Text, Qt::white);
        m_key->setPalette(palette);
        m_errorMsg->setText("An error occured: License key does not match to the email.");
    }
}

void LicenseDialog::cancelClicked(bool)
{
    this->close();
}

void LicenseDialog::linkClicked(QString url)
{
    QDesktopServices::openUrl(QUrl(url));
}

void LicenseDialog::emailChanged(QString email)
{
    if(email.length() > 0 && m_key->text().length() > 0)
        m_okButton->setEnabled(true);
    else
        m_okButton->setEnabled(false);
}

void LicenseDialog::keyChanged(QString key)
{
    if(key.length() > 0 && m_email->text().length() > 0)
        m_okButton->setEnabled(true);
    else
        m_okButton->setEnabled(false);
}
