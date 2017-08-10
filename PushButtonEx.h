/*
    改造过的按钮，可设置背景图。需要准备至少三张背景图，分别是normal,hover,pressed
    例如：min.png，min_hover.png,min_pressed.png
	原作者：╰☆奋斗ing❤孩子`
	博客地址：http://blog.sina.com.cn/liang19890820
	QQ：550755606
	Qt分享、交流群：26197884
	改动：xiongxuanwen

	注：请尊重原作者劳动成果，仅供学习使用，请勿盗用，违者必究！
*/

#ifndef PUSHBUTTONEX_H
#define PUSHBUTTONEX_H

#include <QPushButton>
#include <QPainter>
#include <QMouseEvent>

class PushButtonEx : public QPushButton
{
	Q_OBJECT

public:
	explicit PushButtonEx(QWidget *parent = 0);
	~PushButtonEx();
	void setPicName(QString pic_name);

protected:
	void enterEvent(QEvent *);
	void leaveEvent(QEvent *);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void paintEvent(QPaintEvent *);

private:
	//枚举按钮的几种状态
	enum ButtonStatus{NORMAL, ENTER, PRESS, NOSTATUS};
	ButtonStatus status;
	QString pic_name;

	int btn_width; //按钮宽度
	int btn_height; //按钮高度
	bool mouse_press; //按钮左键是否按下
};

#endif //PUSHBUTTONEX_H

