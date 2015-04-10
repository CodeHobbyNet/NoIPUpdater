#include <iostream>
//#include <ostream>
#include <string>
#include <vector>
#include <sstream>
#include <stdio.h>
//#include <curl/curl.h>

class IP
{//To represent a single IP address.
	private:
		std::vector<int> bytes;//To represent the four bytes of the IP address initialized to all 0s.

	public:
		friend std::ostream& operator<< (std::ostream &out, const IP &address);
		IP( int first, int second, int third, int fourth );//Constructor with the four bytes of the address.
		IP();//Default constructor. Sets the address to all 0s.
		void setBytes( int first, int second, int third, int fourth );//Set the four bytes of the address.
		int getByte( int index ) const;//Return the byte at specified index. The const is so it can be called on a const object.
		int operator[]( int index ) const;//Return the byte at the specified index. The const is so it can be called on a const object.
		std::string getIPString() const;//Return the address as a string. The const is so it can be called on a const object.
		void setString( std::string ipString );//Set the IP address with a string.

};

std::ostream& operator<< (std::ostream &out, const IP &address)
{//Puts a string of the address to the output stream.
	//out << bytes[0] << "." << bytes[1] << "." << bytes[2] << "." << bytes[3];
	out << address.getByte(0) << "." << address.getByte(1) << "." << address.getByte(2) << "." << address.getByte(3);
	return out;
}

IP::IP( int first, int second, int third, int fourth )
{//Constructor with the four bytes of the address.
	bytes.clear();
	bytes.push_back( first );
	bytes.push_back( second );
	bytes.push_back( third );
	bytes.push_back( fourth );
/*
	bytes[0] = first;
	bytes[1] = second;
	bytes[2] = third;
	bytes[3] = fourth;
*/
}

IP::IP()
{//Default constructor. Sets the address to all 0s.
	bytes.clear();
	bytes.push_back(0);
	bytes.push_back(0);
	bytes.push_back(0);
	bytes.push_back(0);
}

void IP::setBytes( int first, int second, int third, int fourth )
{//Set the four bytes of the address.
	bytes[0] = first;
	bytes[1] = second;
	bytes[2] = third;
	bytes[3] = fourth;
}

int IP::getByte( int index ) const
{//Return the byte at specified index. The index starts at 0. The const is so it can be called on a const object.
	return bytes[index];
}

int IP::operator[]( int index ) const
{//Return the byte at the specified index. The index starts at 0. The const is so it can be called on a const object.
	return bytes[index];
}

std::string IP::getIPString() const
{//Return the address as a string. The const is so it can be called on a const object.
	std::ostringstream ipStream;

	ipStream << bytes[0] << "." << bytes[1] << "." << bytes[2] << "." << bytes[3];
	return ipStream.str();
}

void IP::setString( std::string ipString )
{//Set the IP address with a string in the format X.X.X.X .
	sscanf( ipString.c_str(), "%d.%d.%d.%d", &bytes[0], &bytes[1], &bytes[2], &bytes[3] );
}
