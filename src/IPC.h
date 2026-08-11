#pragma once

#include <QLocalSocket>
#include <QString>

// Read socket2 (event) and write to socket (command)
class HyprlandIPC {
  public:
    HyprlandIPC(QString socketPath);

    void writeCommand(QString command);
    void onEvent(std::function<void(const QByteArray&)>);
    void onCommandResponse(std::function<void(const QByteArray&)>);

  private:
    const QString _eventSocketPath;
    const QString _commandSocketPath;
    QLocalSocket _eventSocket;
    QLocalSocket _commandSocket;

    QByteArray _eventBuffer;
    QByteArray _responseBuffer;

    std::function<void(const QByteArray&)> _eventCallback;
    std::function<void(const QByteArray&)> _commandResponseCallback;

    void _monitorAdded();
    void _monitorRemoved();
    void _handleEvent();

    void _responseData();
    void _handleResponse();
};
