#include "MonitorModel.h"
#include "IPC.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

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
