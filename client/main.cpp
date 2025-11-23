/* TCP Client for Chat App
 * by Axel Mizerovskiy
 */
#include <iostream>                                                                     
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

using namespace std;

int main(){
    //create socket (sockets ae just ints, refrences to the commmuncation port)
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    if(sock == -1){
        cout << "Could not create socket.\n"; 
        return 1; // cant create socket
    }

    //create a hunt structure for the server we are connecting to
    int port = 54000;
    string ipAddress = "127.0.0.1"; // change to this a non loopback one for actual server

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

    //connect to server on socket
    int connectRes = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if(connectRes == -1){
        cout << "Could not connect to server.\n";
        return 1; // cant connect to server
    }

    //loop
    char buf[4096];
    string userInput;

    do {
        //enter text lines
        cout << "> ";
        getline(cin, userInput); // get entire input line    
        //send to server
        int sendRes = send(sock, userInput.c_str(), userInput.size() + 1, 0); // c string have a trailing 0
        if (sendRes == -1){
            cout << "Could not send to server!\r\n";
            continue; // dont want to break just want to retry
        }
        
        // wait for responce 
        memset(buf, 0, 4096);
        int bytesRecieved = recv(sock, buf, 4096, 0);
        if (bytesRecieved == -1){
            cout << "There was an error getting responce from a server\r\n";
        } else {        
            // display responce 
            cout << "SERVER> " << string(buf, bytesRecieved) << "\r\n";
        }
    } while(true);

    //close socket
    close(sock);

    return 0;
}
