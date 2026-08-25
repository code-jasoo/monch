#include "MonitorModel.h"
#include "IPC.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QTimer>

MonitorModel::MonitorModel(QObject* parent) : QAbstractListModel(parent), IPC(socketPath) {
    IPC.onEvent([this](const QByteArray& event) { _onEvent(event); });
    IPC.onCommandResponse([this](const QByteArray& resp) { _onResponse(resp); });
}

void MonitorModel::_onEvent(const QByteArray& event) {
    // When monitor is added or removed
    qDebug() << QString::fromUtf8(event);
    // Reload the monitors
    reloadMonitors();
}
void MonitorModel::_onResponse(const QByteArray& resp) {
    qDebug() << QString::fromUtf8(resp);
    QJsonDocument doc = QJsonDocument::fromJson(resp);

    if (!doc.isArray()) {
        return;
    } // check if valid

    beginResetModel();

    QJsonArray array = doc.array();

    m_monitors.clear();

    for (const QJsonValue& value : array) {
        QJsonObject obj = value.toObject();
        Monitor* t_monitor =
            new Monitor(obj["width"].toInt(), obj["height"].toInt(), obj["id"].toInt(),
                        obj["name"].toString(), obj["description"].toString(), obj["x"].toInt(),
                        obj["y"].toInt(), obj["scale"].toDouble());

        m_monitors.append(t_monitor);
    }
    endResetModel();
    emit monitorsChanged();
}

int MonitorModel::rowCount(const QModelIndex& parent) const { return m_monitors.count(); }

QVariant MonitorModel::data(const QModelIndex& index, int role) const {
    if (index.row() < 0 || index.row() >= m_monitors.count()) {
        return QVariant();
    }
    Monitor* monitor = m_monitors[index.row()];
    switch (role) {
    case IDRole:
        return monitor->id();
    case WidthRole:
        return monitor->width();
    case HeightRole:
        return monitor->height();
    case XRole:
        return monitor->x();
    case YRole:
        return monitor->y();
    case ScaleRole:
        return monitor->scale();
    case NameRole:
        return monitor->name();
    case OutputRole:
        return monitor->output();
    }
    return QVariant();
}

QHash<int, QByteArray> MonitorModel::roleNames() const {
    QHash<int, QByteArray> roles;

    roles[WidthRole] = "width";
    roles[HeightRole] = "height";
    roles[XRole] = "x";
    roles[YRole] = "y";
    roles[NameRole] = "name";
    roles[IDRole] = "id";
    roles[OutputRole] = "output";
    roles[ScaleRole] = "scale";

    return roles;
}

void MonitorModel::reloadMonitors() { IPC.writeCommand("j/monitors"); }

void MonitorModel::queueMonitor(QString id, int x, int y, int width, int height, int scale) {
    /*
    QString cmd = QString("eval 'hl.monitor({ output = \"%1\", mode = \"%2x%3@60\", "
                          "position = \"%4x%5\", scale = \"%6\"})'")
                      .arg(id)
                      .arg(x)
                      .arg(y)
                      .arg(width)
                      .arg(height)
                      .arg(scale);
    */

    QString cmd = QString("keyword monitor %1,%2x%3@60,%4x%5,%6")
                      .arg(id)
                      .arg(width)
                      .arg(height)
                      .arg(x)
                      .arg(y)
                      .arg(scale);

    qDebug() << cmd;
    monitorQueue[id] = cmd;
}

void MonitorModel::writeMonitors() {
    QString cmd = "[[BATCH]]";
    for (const auto& [key, value] : monitorQueue) {
        cmd += value + ";";
    }
    qDebug() << cmd;
    IPC.writeCommand(cmd);
    // NOTE: delay the reload by 100ms as hyprland doesnt configure monitors instantly.
    // additionally the command response, "ok" seems to happen before the configuration
    // so using that as an event to reload doesnt work + various other issues.
    // for now this will do until i find some workaround
    QTimer::singleShot(100, [this]() {
        qDebug() << "reloading...";
        reloadMonitors();
    });
}

QVariantList MonitorModel::values() const {
    QVariantList result;
    for (Monitor* m : m_monitors) {
        result.append(QVariant::fromValue(m));
    }
    return result;
}

QRectF MonitorModel::bounds() const {
    qreal mx1, my1, mx2, my2;

    if (m_monitors.isEmpty()) {
        return QRectF();
    }

    Monitor* mon = m_monitors[0];

    mx1 = mon->x();
    my1 = mon->y();
    mx2 = mx1 + mon->width() / mon->scale();
    my2 = my1 + mon->height() / mon->scale();

    for (Monitor* m : m_monitors) {
        if (!m) {
            continue;
        }
        if (m->x() < mx1) {
            mx1 = m->x();
        }
        if (m->y() < my1) {
            my1 = m->y();
        }
        if (m->x() + m->width() / m->scale() > mx2) {
            mx2 = m->x() + m->width() / m->scale();
        }
        if (m->y() + m->height() / m->scale() > my2) {
            my2 = m->y() + m->height() / m->scale();
        }
    }
    return QRectF(mx1, my1, mx2 - mx1, my2 - my1);
}
