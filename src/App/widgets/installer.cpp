/****************************************************************************
** Copyright (C) 2016 Olaf Japp
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

#include "installer.h"

#include <QCryptographicHash>
#include <QDebug>
#include <QDir>
#include <QFile>

void Installer::installFiles(QString sourceDir, QString targetDir, bool readOnly, bool recursive)
{
    QDir dir(sourceDir);
    dir.mkpath(targetDir);
    if(recursive)
    {
        foreach (QString d, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
        {
            QString dst_path = targetDir + QDir::separator() + d;
            dir.mkpath(dst_path);
            installFiles(sourceDir + QDir::separator() + d, dst_path, readOnly, recursive);
        }
    }
    foreach(QString source, dir.entryList(QDir::NoDotAndDotDot | QDir::Files))
    {
        QFile target(targetDir + QDir::separator() + source);
        // only override readonly files like plugins or themes

        if(target.exists())
        {
            if(readOnly)
            {
                QCryptographicHash md5genTarget(QCryptographicHash::Md5);
                if(target.open(QFile::ReadOnly))
                {
                    md5genTarget.addData(target.readAll());
                    target.close();
                }
                QFile sourceFile(sourceDir + QDir::separator() + source);
                QCryptographicHash md5genSource(QCryptographicHash::Md5);
                if(sourceFile.open(QFile::ReadOnly))
                {
                    md5genSource.addData(sourceFile.readAll());
                    sourceFile.close();
                }

                if(md5genTarget.result() != md5genSource.result())
                {
                    target.remove();
                    installFile(sourceDir + QDir::separator() + source, targetDir + QDir::separator() + source, readOnly);
                }
            }
        }
        else
        {
            installFile(sourceDir + QDir::separator() + source, targetDir + QDir::separator() + source, readOnly);
        }
    }
}

void Installer::installFile(QString sourceFile, QString targetFile, bool readOnly)
{
    qDebug() << "installing file" << targetFile;
    QFile::copy(sourceFile, targetFile);
    QFile target(targetFile);
    if(readOnly)
        target.setPermissions(QFileDevice::ReadOwner);
    else
        target.setPermissions(QFileDevice::WriteOwner | QFileDevice::ReadOwner);
}
