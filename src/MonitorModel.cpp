#include "MonitorModel.h"
#include "IPC.h"

MonitorModel::MonitorModel(QObject* parent) : QAbstractListModel(parent), IPC(socketPath) {
    IPC.onEvent([this](const QByteArray& event) { cb(event); });
}

void MonitorModel::cb(const QByteArray& event) { qDebug() << QString::fromUtf8(event); }

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

    return roles;
}
