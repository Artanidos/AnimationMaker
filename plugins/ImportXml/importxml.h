#ifndef IMPORTXML_H
#define IMPORTXML_H

#include "interfaces.h"
#include <QObject>
#include <QDomDocument>

class ImportXml : public ImportMetaInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.artanidos.AnimationMaker.ImportMetaInterface" FILE "imports.json")
    Q_INTERFACES(ImportMetaInterface)
public:
    ImportXml();
    QString displayName() const override;
    QString filter() const override;
    QString title() const override;
    void importMeta(QString filename, AnimationMaker::Animation *animation, QStatusBar *bar) override;

private:
    void readKeyframes(QDomElement *element, AnimationMaker::AnimationItem *item);
};

#endif // IMPORTXML_H
