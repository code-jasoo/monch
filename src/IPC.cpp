#include "IPC.h"
#include <QDebug>
#include <QLocalSocket>
#include <QObject>
#include <QString>
#include <QTimer>
#include <queue>

HyprlandIPC::HyprlandIPC(QString socketPath)
    : _eventSocketPath(socketPath + "/.socket2.sock"),
      _commandSocketPath(socketPath + "/.socket.sock") {
    _eventSocket.connectToServer(_eventSocketPath);
    // Connect event socket signal to callback
    QObject::connect(&_eventSocket, &QLocalSocket::readyRead, [this]() { _handleEvent(); });
    // Connect command socket signal to callback
    QObject::connect(&_commandSocket, &QLocalSocket::readyRead, [this]() { _responseData(); });
    // Connect command exit signal to callback
    QObject::connect(&_commandSocket, &QLocalSocket::disconnected, [this]() {
        _handleResponse();
        if (_commandQueue.empty()) {
            return;
        }
        qDebug() << "reconnecting...";
        QTimer::singleShot(0, [this]() {
            qDebug() << "connecting...";
            _commandSocket.connectToServer(_commandSocketPath);
        });
    });

    // When command socket is connected, run next command
    QObject::connect(&_commandSocket, &QLocalSocket::connected, [this]() { _handleCommands(); });
}

void HyprlandIPC::writeCommand(QString command) {
    _commandQueue.push(command);
    if (_commandSocket.state() == QLocalSocket::UnconnectedState) { // if socket is not connected
        qDebug() << "asdad";
        _commandSocket.connectToServer(_commandSocketPath); // and its doing nothing
    }
}

void HyprlandIPC::_handleCommands() {
    _commandSocket.write(_commandQueue.front().toUtf8());
    _commandQueue.pop();
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
