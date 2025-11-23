/* TCP Server for Chat App
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
    //create socket
    int listening = socket(AF_INET, SOCK_STREAM, 0); // ipv4

    if(listening == -1){
        cerr << "Can't create a socket!";
        return -1;
    }

    //bind socket to an IP / port (works for any ip on machine)
    sockaddr_in hint; //ipv4
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000); // host to network short, converts host numbers to net
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr); //internet command, number to array


    if(bind(listening, (sockaddr*)&hint, sizeof(hint)) == -1){ // bind socket "listening" to AF_INET format (ipv4) to this hint int struct
        cerr << "Cant bind to IP/port";
        return -2;
    }
    //mark socket for listening in 
    if (listen(listening, SOMAXCONN) == -1){
        cerr << "Can't listen";
        return -3;
    }
    
    //accapt a call
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    char host[NI_MAXHOST]; // buffers
    char svc[NI_MAXSERV];
    
    int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

    if (clientSocket == -1){
        cerr << "Problem with client connecting!";
        return -4;
    }


    //close listening socket
    close(listening);

    memset(host, 0, NI_MAXHOST);
    memset(svc, 0, NI_MAXSERV);
    
    
    int result = getnameinfo((sockaddr*)&client, clientSize, host, NI_MAXHOST, svc, NI_MAXSERV, 0);
    if(result){
        cout << host << " connected on " << svc << endl; // auto
    } else { 
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST); // oposite of inet_pton 
        cout << host << " connected on " << ntohs(client.sin_port) << endl; // manual
    }
    //while receiving- display message and echo message 
    char buf[4096];
    while(true){
        //clear buffer 
        memset(buf, 0, 4096);
        //wait for message
        int bytesRecv = recv(clientSocket, buf, 4096, 0);
        if (bytesRecv == -1){
            cerr << "There was a connection issue" << endl;
            break;
        }

        if (bytesRecv == 0){ 
            cout << "The client disconnected" << endl;
            break;
        }

        //display message
        cout << "Recieved: " << string(buf, 0, bytesRecv) << endl; 
        
        //resend message
        send(clientSocket, buf, bytesRecv + 1, 0); 
    }
    //close socket
    close(clientSocket);

    return 0;
}
