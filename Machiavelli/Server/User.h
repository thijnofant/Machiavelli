//
//  User.hpp
//  socketexample
//
//  Created by Bob Polis on 23-11-15.
//  Copyright © 2015 Avans Hogeschool, 's-Hertogenbosch. All rights reserved.
//

#ifndef User_hpp
#define User_hpp

#include "stdafx.h"

class User {
public:
	User() {}
	User(const std::string& name) : name {name} {}

    string get_name() const { return name; }
    void set_name(const std::string& new_name) { name = new_name; }

	int GetToken()const { return token; }
	void SetToken(int new_token) { token = new_token; }

private:
    std::string name;
	int token;
};

#endif /* User_hpp */
