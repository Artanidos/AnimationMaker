#include "options.h"
#include <QLayout>
#include <QLabel>
#include <QPushButton>

Options::Options()
{
    setWindowTitle("Options");
    m_pluginsDirEdit = new QLineEdit();
    m_pluginsDirEdit->setMinimumWidth(400);
    QPushButton *okButton = new QPushButton("OK");
    okButton->setDefault(true);
    QPushButton *cancelButton = new QPushButton("Cancel");
    QPushButton *browse = new QPushButton("Browse...");
    QGridLayout *layout = new QGridLayout();
    layout->addWidget(new QLabel("Plugin directory"), 0, 0);
    layout->addWidget(m_pluginsDirEdit, 1, 0, 1, 4);
    layout->addWidget(browse, 1, 4);
    layout->addWidget(okButton, 2, 3);
    layout->addWidget(cancelButton, 2, 4);
    setLayout(layout);

    connect(okButton, SIGNAL(clicked(bool)), this, SLOT(okClicked()));
    connect(cancelButton, SIGNAL(clicked(bool)), this, SLOT(reject()));
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
