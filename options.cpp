#include "options.h"
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>

Options::Options()
{
    setWindowTitle("Options");
    m_pluginsDirEdit = new QLineEdit();
    m_pluginsDirEdit->setMinimumWidth(400);
    QPushButton *okButton = new QPushButton("OK");
    okButton->setDefault(true);
    QPushButton *cancelButton = new QPushButton("Cancel");
    QPushButton *browseButton = new QPushButton();
    browseButton->setIcon(QIcon(":/images/lupe.png"));
    QGridLayout *layout = new QGridLayout();
    layout->addWidget(new QLabel("Plugin directory"), 0, 0);
    layout->addWidget(m_pluginsDirEdit, 1, 0, 1, 5);
    layout->addWidget(browseButton, 1, 5);
    layout->addWidget(okButton, 2, 3);
    layout->addWidget(cancelButton, 2, 4, 1, 2);
    setLayout(layout);

    connect(okButton, SIGNAL(clicked(bool)), this, SLOT(okClicked()));
    connect(cancelButton, SIGNAL(clicked(bool)), this, SLOT(reject()));
    connect(browseButton, SIGNAL(clicked(bool)), this, SLOT(browse()));
}

void Options::okClicked()
{
    m_pluginsDir = m_pluginsDirEdit->text();
    this->accept();
}

void Options::setPluginsDir(QString dir)
{
    m_pluginsDir = dir;
    m_pluginsDirEdit->setText(dir);
}

void Options::browse()
{
    QString dirName;
    QFileDialog *dialog = new QFileDialog();
    dialog->setFileMode(QFileDialog::Directory);
    dialog->setWindowTitle(tr("Choose Directory"));
    dialog->setOption(QFileDialog::DontUseNativeDialog, true);
    dialog->setOption(QFileDialog::ShowDirsOnly, true);
    dialog->setAcceptMode(QFileDialog::AcceptOpen);
    if(dialog->exec())
        dirName = dialog->selectedFiles().first();
    delete dialog;
    if (dirName.isEmpty())
        return;
    m_pluginsDirEdit->setText(dirName);
}
