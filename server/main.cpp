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
    //number of clients
    int clientNum = 0;


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

    clientNum ++; // accepts client and adds them to client pool
    
    // accept second client
    // accept second call
    sockaddr_in client2;
    socklen_t clientSize2 = sizeof(client);
    char host2[NI_MAXHOST]; //buffers
    char svc2[NI_MAXSERV];

    int clientSocket2 = accept(listening, (sockaddr*)&client2, &clientSize2);

    if (clientSocket2 == -1){
        cerr << "Problem with client connecting!";
        return -4;
    }
    
    clientNum ++; // accepts second client and adds them

    //close listening socket
    close(listening);

    memset(host, 0, NI_MAXHOST);
    memset(svc, 0, NI_MAXSERV);

    memset(host2, 0, NI_MAXHOST);
    memset(svc2, 0, NI_MAXSERV);
    
    
    int result = getnameinfo((sockaddr*)&client, clientSize, host, NI_MAXHOST, svc, NI_MAXSERV, 0);
    int result2 = getnameinfo((sockaddr*)&client2, clientSize2, host2, NI_MAXHOST, svc2, NI_MAXSERV, 0);
    
    if(result){
        cout << host << " connected on " << svc << endl; // auto
    } else { 
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST); // oposite of inet_pton 
        cout << host << " connected on " << ntohs(client.sin_port) << endl; // manual
    }

    if(result2){
        cout << host2 << " connected on " << svc2 << endl; //auto
    } else {
        inet_ntop(AF_INET, &client2.sin_addr, host2, NI_MAXHOST);
        cout << host2 << " connected on " << ntohs(client2.sin_port) << endl; //manual
    }


    //while receiving- display message and echo message 
    char buf[4096];
    char buf2[4096];
    while(true){
        //clear buffer 
        memset(buf, 0, 4096);
        memset(buf2,0, 4096);
        //wait for message
        int bytesRecv = recv(clientSocket, buf, 4096, 0);
        int bytesRecv2 = recv(clientSocket2, buf2, 4096, 0);
        if ((bytesRecv == -1) || (bytesRecv2 == -1)){
            cerr << "There was a connection issue" << endl;
            break;
        }

        if ((bytesRecv == 0) || (bytesRecv == 0)){ 
            cout << "The client disconnected" << endl;
            break;
        }

        //display message
        cout << "Recieved from client1: " << string(buf, 0, bytesRecv) << endl; 
        cout << "Recieved from client2: " << string(buf2, 0, bytesRecv2) << endl;
        //resend message
        send(clientSocket2, buf, bytesRecv + 1, 0);
        send(clientSocket, buf2, bytesRecv + 1, 0); 
    }
    //close socket
    close(clientSocket);
    close(clientSocket2);

    return 0;
}
