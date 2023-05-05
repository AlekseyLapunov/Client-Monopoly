#ifndef GAMETRANSMITTEROBJECT_H
#define GAMETRANSMITTEROBJECT_H

#include <QObject>

class GameTransmitterObject : public QObject
{
    Q_OBJECT
public:
    explicit GameTransmitterObject(QObject *parent = nullptr);

signals:
    void startStageAnimation(uint8_t stageNumber);
};

#endif // GAMETRANSMITTEROBJECT_H
