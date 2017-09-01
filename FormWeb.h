#ifndef FORMWEBMAP_H
#define FORMWEBMAP_H

#include <QWidget>

class WebAxWidget;

class FormWebBase : public QWidget
{
    Q_OBJECT

public:
    explicit FormWebBase(QWidget *parent = 0);
    ~FormWebBase();

public slots:
    void loadUrl(QString url);

private:
    WebAxWidget* m_webWidget;
};

#endif // FORMWEBMAP_H
