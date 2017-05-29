#ifndef EXPORTGIF_H
#define EXPORTGIF_H

#include "interfaces.h"
#include <QObject>
#include <QProcess>

class ExportGif : public ExportMovieInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.artanidos.AnimationMaker.ExportGifInterface" FILE "exports.json")
    Q_INTERFACES(ExportMovieInterface)

public:
    ExportGif();
    QString displayName() const override;
    QString filter() const override;
    QString title() const override;
    void exportMovie(QString filename, QGraphicsView *view, int length, int fps, QStatusBar *bar) override;
signals:
    void setFrame(int);

private slots:
    void paletteGenFinished(int);
    void paletteGenError(QProcess::ProcessError);
    void paletteUseFinished(int);
    void paletteUseError(QProcess::ProcessError);

private:
    QStatusBar *m_bar;
    QProcess *m_paletteGen;
    QProcess *m_paletteUse;
    QString m_dir;
    QString m_tempFileName;
    QString m_fileName;
};

#endif // EXPORTGIF_H
