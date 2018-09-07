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
    void commercialLinuxClicked();
    void commercialWindowsClicked();
    void communityClicked();

private:
    QPushButton *m_okButton;
    QLineEdit *m_email;
    QLineEdit *m_key;
    MainWindow *m_mainWindow;
    QLabel *m_errorMsg;
};

#endif // LICENSEDIALOG_H
