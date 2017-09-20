#ifndef TPSDEVTS_H
#define TPSDEVTS_H

#ifndef  EVT_FILE_TYPE
#define  EVT_FILE_TYPE      0       //201402020 文件事件类型
#define  EVT_TASK_TYPE      1       //201402020 任务事件类型
#define  EVT_STATUS_TYPE    2       //201402018 状态类型，主要是记录保持的状态变量，只能是0或1（假或真）
#define  EVT_APP_TYPE       3       //201402018 增加进程状态标记，不与普通事件标记共享区域（一般用于内部通讯）
#endif

#include <QObject>

/**事件处理类，得到事件号处理得到类型与计数器
 * @brief The TPsdEvts class
 */
class TPsdEvts : public QObject
{
    Q_OBJECT
public:
    TPsdEvts(QObject *parent = NULL);
    ~TPsdEvts();

    /**
     * @brief setEventList   根据事件值列表触发该事件
     * @param arList         事件值列表
     * @param val
     */
    void setEventList(QList<QString> &arList, int val);
    /**
     * @brief eventType      分析事件类型
     * @param pchEvent       需要得到的原事件字符串
     * @param pnEventNo      将pchEvent字符串转成对应的事件
     * @return               事件类型
     */
    int eventType(QString &pchEvent, int *pnEventNo );//事件值类型

    /**
     * @brief evtCounter     计算事件计数器
     * @param pchEvent       事件字符串
     * @return               返回事件计数器的值
     */
    int evtCounter(QString &pchEvent);
protected:
public slots:
};

#endif // TPSDEVTS_H
