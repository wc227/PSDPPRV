#ifndef DLGWORKFLOW_H
#define DLGWORKFLOW_H

#include <QDialog>
#include "cwidget.h"

class DlgWorkFlow : public CWidget
{
    Q_OBJECT

public:
    DlgWorkFlow(QWidget *parent = 0);
    ~DlgWorkFlow();

public slots:
    void setIniPath(QString p) { CWidget::setIniPath(p); }
};

#endif // DLGWORKFLOW_H
