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
    User _player;
public:
    ClientInfo(Socket socket, User player)
        : _socket{std::move(socket)}, _player{std::move(player)} {}
    Socket& get_socket() { return _socket; }
    const Socket& get_socket() const { return _socket; }
	User& get_user() { return _player; }
	const User& get_user() const { return _player; }
};

#endif
