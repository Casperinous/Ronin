#include <stdlib.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include "../ronin/log.h"

static char * socket_get_ip(struct sockaddr *addr)
{
	char * ip = NULL;

	if (addr->sa_family == AF_INET) 
	{ 
		struct sockaddr_in * addrptr = (struct sockaddr_in *)addr;
		ip = inet_ntoa(addrptr->sin_addr);
	}

	return ip;
}


unsigned int mconnect(int socket, const struct sockaddr *address, socklen_t address_len)
{
	LOGX(INFO, "%s","[*] Inside the modified connect function of libc");
	if(socket > 0 && address != NULL)
		LOGX(INFO, "[+] Socket number [%d] attempting to connect to --> %s",socket, socket_get_ip(address));

}

void __attribute__ ((constructor)) main()
{
	LOGX(INFO,"%s", "[+] Library for network operations was loaded.");
}
