#ifndef FORMWEBMAP_H
#define FORMWEBMAP_H

#include <QWidget>

class WebAxWidget;

class FormWebMap : public QWidget
{
    Q_OBJECT

public:
    explicit FormWebMap(QWidget *parent = 0);
    ~FormWebMap();

public slots:
    void loadUrl(QString url);

private:
    WebAxWidget* m_webWidget;
};

#endif // FORMWEBMAP_H
