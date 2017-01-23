//
//  Player.hpp
//  socketexample
//
//  Created by Bob Polis on 23-11-15.
//  Copyright © 2015 Avans Hogeschool, 's-Hertogenbosch. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp

#include <string>

class User {
public:
	User() {}
	User(const std::string& name) : name {name} {}

    std::string get_name() const { return name; }
    void set_name(const std::string& new_name) { name = new_name; }

	int GetToken()const { return token; }
	void SetToken(int new_token) { token = new_token; }

private:
    std::string name;
	int token;
};

#endif /* Player_hpp */
