/*
 * TcpServerWorker.cpp
 *
 *  Created on: May 25, 2016
 *      Author: z.j
 */
 
#include "TcpServerWorker.h"
#include "NetLogger.h"
#include "TcpSocket.h"
#include "Worker.h"

using namespace net;
using namespace cm;
using namespace std;

// -------------------------------------------
TcpServerWorker::TcpServerWorker(Worker* theWorker) 
: m_worker(theWorker), m_connectionIdCounter(0)
{
    NetLogger::initConsoleLog();
}

// -------------------------------------------
TcpServerWorker::~TcpServerWorker() {
    // Empty
}

// -------------------------------------------
void TcpServerWorker::onConnectionCreated(TcpSocket* theNewSocket) {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "onConnectionCreated(), new fd: " << theNewSocket->getSocket());   
    createConnection(theNewSocket);
}

// -------------------------------------------
void TcpServerWorker::handleRecvResult(TcpSocket* theSocket, int numOfBytesRecved) {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "TcpServerWorker::handleRecvResult, new fd: " << theSocket->getSocket());
    
    // TODO add a new task to handle received data in worker thread
    
    // for test
    map<unsigned int, TcpConnection*>::iterator it = m_connMap.find((size_t)theSocket->getUserData());
    if (it != m_connMap.end()) {
        TcpConnection* tcpConn = it->second;
        tcpConn->onDataReceive(numOfBytesRecved);
    }
}
// -------------------------------------------
void TcpServerWorker::createConnection(TcpSocket* theNewSocket) {
    // TODO limit the max connection
    m_connectionIdCounter++;
    TcpConnection* newTcpConn = new TcpConnection(theNewSocket, m_connectionIdCounter, this);
    
    std::pair<map<unsigned int, TcpConnection*>::iterator, bool> result = 
        m_connMap.insert(map<unsigned int, TcpConnection*>::value_type(m_connectionIdCounter, newTcpConn));
    
    // if the connection id is used, try 1000 times more
    if (!result.second) {
        for (int i=0; i<1000; ++i) {
            m_connectionIdCounter++;
            result = m_connMap.insert(map<unsigned int, TcpConnection*>::value_type(m_connectionIdCounter, newTcpConn));
            // if success, update the connection id and exit the loop
            if (result.second) {
                newTcpConn->setConnectionId(m_connectionIdCounter);
                break;
            }
        }
    }
    
    // if still fail to find a connection id, consider it as unrecoverable error, crash
    if (!result.second) {
        LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "Fail to allocate a connection for new connection, exit process.");
        _exit(-1);
    }
    
    // TODO start timer, disconnect if no heart beat?
    
    // set connection id in socket for later use when receiving data from the socket
    theNewSocket->setUserData((void*)m_connectionIdCounter);
    
    // start receiving data from socket. remove the connection if fail to receive
    if (!newTcpConn->recvDataFromSocket()) {
        m_connMap.erase(result.first);
        delete newTcpConn;
    }
}