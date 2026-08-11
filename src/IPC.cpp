#include "IPC.h"
#include <QLocalSocket>
#include <QObject>
#include <QString>

HyprlandIPC::HyprlandIPC(QString socketPath)
    : _eventSocketPath(socketPath + "/.socket2.sock"),
      _commandSocketPath(socketPath + "/socket.sock") {
    _eventSocket.connectToServer(_eventSocketPath);
    _commandSocket.connectToServer(_commandSocketPath);

    QObject::connect(&_eventSocket, &QLocalSocket::readyRead, [this]() { _handleEvent(); });
}

void HyprlandIPC::writeCommand(QString command) { _commandSocket.write(command.toUtf8()); }

void HyprlandIPC::onEvent(std::function<void(const QByteArray&)> callback) {
    _eventCallback = callback;
}

void HyprlandIPC::_handleEvent() {
    _eventBuffer += _eventSocket.readAll();
    int newline = _eventBuffer.indexOf('\n');
    while (newline != -1) {
        QByteArray event = _eventBuffer.left(newline);
        _eventBuffer.remove(0, newline + 1);

        if ((event.startsWith("monitoraddedv2") || event.startsWith("monitorremovedv2")) &&
            _eventCallback) {
            _eventCallback(event);
        }
        newline = _eventBuffer.indexOf('\n');
    }
}
