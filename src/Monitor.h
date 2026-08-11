#pragma once

#include <QObject>
#include <QProperty>

class Monitor : public QObject {
    Q_OBJECT
    Q_PROPERTY(int width READ width)
    Q_PROPERTY(int height READ height)

    Q_PROPERTY(QString id READ id)
    Q_PROPERTY(QString output READ output)
    Q_PROPERTY(QString name READ name)

    Q_PROPERTY(int x READ x NOTIFY xChanged)
    Q_PROPERTY(int y READ y NOTIFY yChanged)

  signals:
    void xChanged();
    void yChanged();

  public:
    int width() const { return _width; }
    int height() const { return _height; }
    QString id() const { return _id; }
    QString output() const { return _output; }
    QString name() const { return _name; }
    int x() const { return _x; }
    int y() const { return _y; }

  private:
    int _width;
    int _height;
    QString _id;
    QString _output;
    QString _name;
    int _x;
    int _y;
};
