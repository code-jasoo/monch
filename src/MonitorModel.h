#include "IPC.h"
#include "Monitor.h"
#include <QAbstractListModel>
#include <QRectF>
#include <cstdlib>

class MonitorModel : public QAbstractListModel {
    Q_OBJECT

  public:
    MonitorModel(QObject* parent = nullptr);

    // TODO : add refresh rate and scale
    enum Roles {
        IDRole = Qt::UserRole + 1, // Hyprland given ID
        WidthRole,
        HeightRole,
        XRole,
        YRole,
        ScaleRole,
        NameRole,  // Monitor model name
        OutputRole // Output name like eDP-1
    };

    int rowCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role) const;
    QHash<int, QByteArray> roleNames() const;

    Q_PROPERTY(QVariantList values READ values NOTIFY monitorsChanged)
    Q_PROPERTY(QRectF bounds READ bounds NOTIFY monitorsChanged)

    Q_INVOKABLE void reloadMonitors();
  signals:
    void monitorsChanged();

  private:
    QList<Monitor*> m_monitors;

    QString XDG_RUNTIME_DIR = std::getenv("XDG_RUNTIME_DIR");
    QString HYPRLAND_INSTANCE = std::getenv("HYPRLAND_INSTANCE_SIGNATURE");
    QString socketPath = XDG_RUNTIME_DIR + "/hypr/" + HYPRLAND_INSTANCE;

    HyprlandIPC IPC;
    void _onEvent(const QByteArray& event);
    void _onResponse(const QByteArray& resp);

    QVariantList values() const;
    QRectF bounds() const;
};
