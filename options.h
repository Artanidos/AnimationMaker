#ifndef OPTIONS_H
#define OPTIONS_H

#include <QDialog>
#include <QLineEdit>

class Options : public QDialog
{
    Q_OBJECT
public:
    Options();

    inline QString pluginsDir() {return m_pluginsDir;}
    void setPluginsDir(QString);

private slots:
    void okClicked();
    void browse();

private:
    QLineEdit *m_pluginsDirEdit;
    QString m_pluginsDir;
};

#endif // OPTIONS_H
