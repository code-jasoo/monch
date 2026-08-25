#include <QFileSystemWatcher>
#include <QObject>
#include <QProperty>
#include <QString>

class ThemeParser : public QObject {
    Q_OBJECT

    Q_PROPERTY(
        QString surface_container_lowest MEMBER m_surface_container_lowest NOTIFY themeChanged)
    Q_PROPERTY(QString surface_dim MEMBER m_surface_dim NOTIFY themeChanged)
    Q_PROPERTY(QString surface MEMBER m_surface NOTIFY themeChanged)
    Q_PROPERTY(QString surface_container_low MEMBER m_surface_container_low NOTIFY themeChanged)
    Q_PROPERTY(QString surface_container MEMBER m_surface_container NOTIFY themeChanged)
    Q_PROPERTY(QString surface_container_high MEMBER m_surface_container_high NOTIFY themeChanged)
    Q_PROPERTY(
        QString surface_container_highest MEMBER m_surface_container_highest NOTIFY themeChanged)
    Q_PROPERTY(QString outline MEMBER m_outline NOTIFY themeChanged)
    Q_PROPERTY(QString outline_variant MEMBER m_outline_variant NOTIFY themeChanged)
    Q_PROPERTY(QString on_surface MEMBER m_on_surface NOTIFY themeChanged)
    Q_PROPERTY(QString on_surface_variant MEMBER m_on_surface_variant NOTIFY themeChanged)
    Q_PROPERTY(QString primary MEMBER m_primary NOTIFY themeChanged)
    Q_PROPERTY(QString secondary MEMBER m_secondary NOTIFY themeChanged)
    Q_PROPERTY(QString tertiary MEMBER m_tertiary NOTIFY themeChanged)
    Q_PROPERTY(QString error MEMBER m_error NOTIFY themeChanged)
  signals:
    void themeChanged();

  public:
    ThemeParser();
    void reloadTheme();

  private:
    QString m_surface_container_lowest;
    QString m_surface_container_low;
    QString m_surface_container;
    QString m_surface_container_high;
    QString m_surface_container_highest;
    QString m_surface;
    QString m_surface_dim;
    QString m_on_surface;
    QString m_on_surface_variant;
    QString m_outline;
    QString m_outline_variant;
    QString m_primary;
    QString m_secondary;
    QString m_tertiary;
    QString m_error;

    QString themePath;
    QFileSystemWatcher fileWatcher;
};
