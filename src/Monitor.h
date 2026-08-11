#pragma once

#include <QObject>
#include <QProperty>

class Monitor : public QObject {
    Q_OBJECT
    Q_PROPERTY(int width READ width)
    Q_PROPERTY(int height READ height)

    Q_PROPERTY(int id READ id)
    Q_PROPERTY(QString output READ output)
    Q_PROPERTY(QString name READ name)

    Q_PROPERTY(int x READ x NOTIFY xChanged)
    Q_PROPERTY(int y READ y NOTIFY yChanged)
    Q_PROPERTY(double scale READ scale)

  signals:
    void xChanged();
    void yChanged();

  public:
    Monitor(int width, int height, int id, QString output, QString name, int x, int y, double scale)
        : _width(width), _height(height), _id(id), _output(output), _name(name), _x(x), _y(y),
          _scale(scale) {};
    int width() const { return _width; }
    int height() const { return _height; }
    int id() const { return _id; }
    QString output() const { return _output; }
    QString name() const { return _name; }
    int x() const { return _x; }
    int y() const { return _y; }
    double scale() const { return _scale; }

  private:
    int _width;
    int _height;
    int _id;
    QString _output;
    QString _name;
    int _x;
    int _y;
    double _scale;
};
