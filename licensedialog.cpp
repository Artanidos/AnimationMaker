/****************************************************************************
** Copyright (C) 2018 Olaf Japp
**
** This file is part of AnimationMaker.
**
**  AnimationMaker is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  AnimationMaker is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with AnimationMaker.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#include "licensedialog.h"
#include "license.h"
#include "mainwindow.h"
#include "flatbutton.h"
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDesktopServices>
#include <QUrl>
#include <QDesktopServices>

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

    FlatButton *commercialLinux = new FlatButton(":/images/commercial-linux.png", ":/images/commercial-linux-hover.png");
    connect(commercialLinux, SIGNAL(clicked()), this, SLOT(commercialLinuxClicked()));

    FlatButton *commercialWindows = new FlatButton(":/images/commercial-windows.png", ":/images/commercial-windows-hover.png");
    connect(commercialWindows, SIGNAL(clicked()), this, SLOT(commercialWindowsClicked()));

    FlatButton *community = new FlatButton(":/images/community.png", ":/images/community-hover.png");
    connect(community, SIGNAL(clicked()), this, SLOT(communityClicked()));

    QGridLayout *grid = new QGridLayout();
    QLabel *desc = new QLabel("Before you start to work with AnimationMaker you should get a license key and register it here.");

    desc->setWordWrap(true);
    desc->setTextFormat(Qt::RichText);
    connect(desc, SIGNAL(linkActivated(QString)), this, SLOT(linkClicked(QString)));

    QHBoxLayout *icons = new QHBoxLayout();
    icons->addWidget(community);
    icons->addWidget(commercialLinux);
    icons->addWidget(commercialWindows);
    grid->addWidget(desc, 0, 0, 1, 2);
    grid->addLayout(icons, 1, 0, 1, 2);
    grid->addWidget(m_errorMsg, 2, 0, 1, 2);
    grid->addWidget(new QLabel("Email"), 3, 0);
    grid->addWidget(new QLabel("License Key"), 4, 0);
    grid->addWidget(m_email, 3, 1);
    grid->addWidget(m_key, 4, 1);
    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addStretch();
    hbox->addWidget(m_okButton);
    hbox->addWidget(cancelButton);
    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addLayout(grid);
    vbox->addLayout(hbox);

    setLayout(vbox);
    setWindowTitle("AnimationMaker License");
}

void LicenseDialog::okClicked(bool)
{
    if(checkLicense(m_email->text(), m_key->text(), 0)) // Community
    {
        m_mainWindow->setEmail(m_email->text());
        m_mainWindow->setLicenseKey(m_key->text());
        m_mainWindow->setCommercial(false);
        m_mainWindow->setLicensed(true);
        this->close();
    }
#ifdef LINUX
    else if(checkLicense(m_email->text(), m_key->text(), 1)) // Linux
#else
    else if(checkLicense(m_email->text(), m_key->text(), 2)) // Windows
#endif
    {
        m_mainWindow->setEmail(m_email->text());
        m_mainWindow->setLicenseKey(m_key->text());
        m_mainWindow->setCommercial(true);
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

void LicenseDialog::commercialLinuxClicked()
{
    QDesktopServices::openUrl(QUrl("https://www.digistore24.com/product/236609"));
}

void LicenseDialog::commercialWindowsClicked()
{
    QDesktopServices::openUrl(QUrl("https://www.digistore24.com/product/223175"));
}

void LicenseDialog::communityClicked()
{
    QDesktopServices::openUrl(QUrl("https://www.digistore24.com/product/238646"));
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
