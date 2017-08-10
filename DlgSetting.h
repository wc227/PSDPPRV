#ifndef DLGSETTING_H
#define DLGSETTING_H

#include <QDialog>

namespace Ui {
class DlgSetting;
}

class DlgSetting : public QDialog
{
    Q_OBJECT

public:
    explicit DlgSetting(QWidget *parent = 0);
    ~DlgSetting();

    //更新界面或更新数据
    //bUI: true-更新界面，false-更新数据
    void updateUI(bool bUI);

private slots:
    void on_btn_OK_clicked();

private:
    Ui::DlgSetting *ui;

public:
    int nLength;
    int nGap;
    int nMax;
};

#endif // DLGSETTING_H
