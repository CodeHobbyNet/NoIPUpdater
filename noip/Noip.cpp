#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <stdio.h>
#include <curl/curl.h>
#include "IP.cpp"

static size_t writeCallback(char* buf, size_t size, size_t nmemb, void* userp)
{//Callback function to get the html. buf is a buffer provided by curl with the html characters. size*nmemb is the size of the buffer. userp is a null pointer provided by the program (in this case the htmlbuffer).
	((std::string*)userp)->append((char*)buf, size*nmemb );//Append size*nmemb characters to the html buffer string provided to curl.
	return size*nmemb;
}

class NoIP
{//For updating No-IP.
	private:
		std::string htmlBuffer;//To hold the text of the web page for the external IP.
		IP ipAddress;
		//char *userAgent = "No-IP Updater/1.0 luigi1015_noip@314ghz.com";//The user agent to set with HTPP requests. Format required by No-IP.
		std::string programName;//The official name of the program. For use in User Agent sent to No-IP.
		std::string programVersion;//The official version of the program. For use in User Agent sent to No-IP.
		std::string email;//The official client email address. For use in User Agent sent to No-IP.
		std::string noipUsername;//The username for NoIP.
		std::string noipPassword;//The password for NoIP.
		std::string noipHostname;//The NoIP hostname to update.

	public:
		NoIP();
		NoIP( std::string newEmail );
		void acquireIP();//Gets the external IP adress.
		void updateNoIP();//Updates NoIP with the IP address the program currently has.
		void setEmail( std::string newEmail );//Sets the client email address.
		std::string getEmail();//Returns the client email address.
		void setUsername( std::string newUsername );//Sets the NoIP username.
		std::string getUsername();//Returns the NoIP username.
		void setPassword( std::string newPassword );//Sets the NoIP password.
		void setHostname( std::string newHostname );//Sets the NoIP hostname to update.
		std::string getHostname();//Returns the NoIP hostname to update.
		std::string getUserAgent();//Returns a string of the User Agent text. Used for sending requests to No-IP.
};

NoIP::NoIP()
{//Defualt constructor.
	htmlBuffer = "";
	programName = "No-IP Updater";
	programVersion = "1.0";
	email = "";
}

NoIP::NoIP( std::string newEmail )
{//Defualt constructor.
	htmlBuffer = "";
	programName = "No-IP Updater";//The official name of the program. For use in User Agent sent to No-IP.
	programVersion = "1.0";//The official version of the program. For use in User Agent sent to No-IP.
	email = newEmail;
}

void NoIP::setEmail( std::string newEmail )
{//Sets the client email address.
	email = newEmail;
}

std::string NoIP::getEmail()
{//Returns the client email address.
	return email;
}
void NoIP::setUsername( std::string newUsername )
{//Sets the NoIP username.
	noipUsername = newUsername;
}

std::string NoIP::getUsername()
{//Returns the NoIP username.
	return noipUsername;
}

void NoIP::setPassword( std::string newPassword )
{//Sets the NoIP password.
	noipPassword = newPassword;
}

void NoIP::setHostname( std::string newHostname )
{//Sets the NoIP hostname to update.
	noipHostname = newHostname;
}

std::string NoIP::getHostname()
{//Returns the NoIP hostname to update.
	return noipHostname;
}

std::string NoIP::getUserAgent()
{//Returns a string of the User Agent text. Used for sending requests to No-IP.
	std::string ua  = programName + "/" + programVersion + " " + email;//User agent in format required by No-IP.
	return ua;
}

void NoIP::acquireIP()
{//Gets the external IP adress.
	CURL* curlIP;
	CURLcode res;

	htmlBuffer = "";//Clear the buffer.

	curl_global_init( CURL_GLOBAL_ALL );
	curlIP = curl_easy_init();
	curl_easy_setopt( curlIP, CURLOPT_URL, "http://icanhazip.com/" );//Set the URL to get.
	//curl_easy_setopt( curlIP, CURLOPT_USERAGENT, "No-IP Updater/1.0 luigi1015_noip@314ghz.com" );//Set the User Agent.
	curl_easy_setopt( curlIP, CURLOPT_USERAGENT, getUserAgent().c_str() );//Set the User Agent.
	curl_easy_setopt( curlIP, CURLOPT_WRITEFUNCTION, writeCallback );//Set the callback function.
	curl_easy_setopt( curlIP, CURLOPT_WRITEDATA, &htmlBuffer );//Set the string buffer to write to.
	res = curl_easy_perform(curlIP);
	if( res != CURLE_OK )
	{//If the action didn't return ok, give an error.
		fprintf( stderr, "In getIP(), error when trying to perform curl_easy_perform(): %s\n", curl_easy_strerror(res) );
	}
	else
	{
		curl_easy_cleanup( curlIP );
		//std::cout << htmlBuffer << std::endl;
		ipAddress.setString( htmlBuffer );
		std::cout << "IP: " << ipAddress << std::endl;
		//std::cout << "Byte one: " << ipAddress.getByte(0) << std::endl;
	}
}

void NoIP::updateNoIP()
{//Updates NoIP with the IP address the program currently has.
	CURL* curlIP;
	CURLcode res;
	std::string url = "";
	std::string authentication = "";

	htmlBuffer = "";//Clear the buffer.

	curl_global_init( CURL_GLOBAL_ALL );
	curlIP = curl_easy_init();
	//curl_easy_setopt( curlIP, CURLOPT_URL, "https://" + noipUsername + ":" + noipPassword + "@dynupdate.no-ip.com/nic/update?hostname=" + noipHostname + "&myip=" + ipAddress.getIPString() );//Set the URL to get.
	//url = "https://" + noipUsername + ":" + noipPassword + "@dynupdate.no-ip.com/nic/update?hostname=" + noipHostname + "&myip=" + ipAddress.getIPString();
	//url = "https://dynupdate.no-ip.com/nic/update?username=" + noipUsername + "&password=" + noipPassword + "&h[]=" + noipHostname + "&ip=" + ipAddress.getIPString();
	url = "https://dynupdate.no-ip.com/nic/update?hostname=" + noipHostname + "&myip=" + ipAddress.getIPString();
	authentication = noipUsername + ":" + noipPassword;//Username and password in a format for curl.
	std::cout << url << std::endl;
	curl_easy_setopt( curlIP, CURLOPT_URL, url.c_str() );//Set the URL to get.
	//curl_easy_setopt( curlIP, CURLOPT_USERAGENT, "No-IP Updater/1.0 luigi1015_noip@314ghz.com" );//Set the User Agent.
	//curl_easy_setopt( curlIP, CURLOPT_USERNAME, noipUsername.c_str() );//Set the username.
	//curl_easy_setopt( curlIP, CURLOPT_PASSWORD, noipPassword.c_str() );//Set the password.
	curl_easy_setopt( curlIP, CURLOPT_USERPWD, authentication.c_str() );//Set the username and password authentication.
	curl_easy_setopt( curlIP, CURLOPT_USERAGENT, getUserAgent().c_str() );//Set the User Agent.
	curl_easy_setopt( curlIP, CURLOPT_WRITEFUNCTION, writeCallback );//Set the callback function.
	curl_easy_setopt( curlIP, CURLOPT_WRITEDATA, &htmlBuffer );//Set teh string buffer to write to.
	res = curl_easy_perform(curlIP);
	if( res != CURLE_OK )
	{//If the action didn't return ok, give an error.
		fprintf( stderr, "In getIP(), error when trying to perform curl_easy_perform(): %s\n", curl_easy_strerror(res) );
	}
	else
	{
		curl_easy_cleanup( curlIP );
		//std::cout << htmlBuffer << std::endl;
		//ipAddress.setString( htmlBuffer );
		std::cout << "Return Code: " << htmlBuffer << std::endl;
		if( htmlBuffer.compare( 0, 4, "good" ) == 0 )
		{//Code for the successful update of NoIP.
			std::cout << "Update Successful!" << std::endl;
		}
		else if( htmlBuffer.compare( 0, 5, "nochg" ) == 0 )
		{//Code for no update since NoIP has the current address.
			std::cout << "NoIP has the current IP." << std::endl;
		}
		else if( htmlBuffer.compare( 0, 6, "nohost" ) == 0 )
		{//Error code for host does not exist at NoIP.
			std::cerr << "NoIP doesn't recognize the hostname." << std::endl;
		}
		else if( htmlBuffer.compare( 0, 7, "badauth" ) == 0 )
		{//Error code for bad NoIP authentication.
			std::cerr << "Invalid NoIP username and password." << std::endl;
		}
		else if( htmlBuffer.compare( 0, 8, "badagent" ) == 0 )
		{//Error code for invalid client agent (i.e. this program). If this error pops up, may want to update the program and User Agent field.
			std::cerr << "Invalid client agent." << std::endl;
		}
		else if( htmlBuffer.compare( 0, 7, "!donator" ) == 0 )
		{//Error code for invalid NoIP feature requested.
			std::cerr << "NoIP feature isn't available." << std::endl;
		}
		else if( htmlBuffer.compare( 0, 5, "abuse" ) == 0 )
		{//Error code for username is blocked by NoIP due to abuse.
			std::cerr << "NoIP username blocked due to abuse. Please read the NoIP terms of service." << std::endl;
		}
		else if( htmlBuffer.compare( 0, 3, "911" ) == 0 )
		{//Error code for fatal error from NoIP.
			std::cerr << "NoIP fatal error. Please try again in at least 30 minutes." << std::endl;
		}
	}
}

int main()
{
	NoIP ip;
	std::string username, password, hostname;

	ip.setEmail( "luigi1015_noip@314ghz.com" );
	std::cout << "Username: ";
	std::cin  >> username;
	ip.setUsername( username );

	std::cout << "Password: ";
	std::cin >> password;
	ip.setPassword( password );

	std::cout << "Hostname: ";
	std::cin  >> hostname;
	ip.setHostname( hostname );

	ip.acquireIP();
	ip.updateNoIP();
}
