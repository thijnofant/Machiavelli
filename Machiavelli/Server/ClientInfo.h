//
// ClientInfo.h
//
// Created by Jeroen de Haas on 22/11/2016.
// Copyright (c) 2016 Avans Hogeschool, 's-Hertogenbosch. All rights reserved.
//

#ifndef _CLIENT_INFO_H
#define _CLIENT_INFO_H

#include <string>
#include <utility>

#include "Socket.h"
#include "User.h"

class ClientInfo {
    Socket _socket;
	User _user;
public:
    ClientInfo(Socket socket, User user)
        : _socket{std::move(socket)}, _user{std::move(user)} {}
    Socket& get_socket() { return _socket; }
    const Socket& get_socket() const { return _socket; }
	User& get_user() { return _user; }
    const User& get_user() const { return _user; }
};

#endif
