#include "IPC.h"
#include <QLocalSocket>
#include <QObject>
#include <QString>

HyprlandIPC::HyprlandIPC(QString socketPath)
    : _eventSocketPath(socketPath + "/.socket2.sock"),
      _commandSocketPath(socketPath + "/.socket.sock") {
    _eventSocket.connectToServer(_eventSocketPath);
    // Connect event socket signal to callback
    QObject::connect(&_eventSocket, &QLocalSocket::readyRead, [this]() { _handleEvent(); });
    // Connect command socket signal to callback
    QObject::connect(&_commandSocket, &QLocalSocket::readyRead, [this]() { _responseData(); });
    // Connect command exit signal to callback
    QObject::connect(&_commandSocket, &QLocalSocket::disconnected, [this]() { _handleResponse(); });
}

void HyprlandIPC::writeCommand(QString command) {
    _commandSocket.connectToServer(_commandSocketPath);
    _commandSocket.write(command.toUtf8());
}

void HyprlandIPC::onEvent(std::function<void(const QByteArray&)> callback) {
    _eventCallback = callback;
}

void HyprlandIPC::onCommandResponse(std::function<void(const QByteArray&)> callback) {
    _commandResponseCallback = callback;
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

void HyprlandIPC::_responseData() { _responseBuffer += _commandSocket.readAll(); }

void HyprlandIPC::_handleResponse() {
    _commandResponseCallback(_responseBuffer);
    _responseBuffer.clear();
}
