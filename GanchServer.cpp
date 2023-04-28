// LISTING 1
#using "system.dll"
using namespace System;
using namespace System::IO;
using namespace System::Net;
using namespace System::Net::Sockets;
#include <cstdlib>  //for system("PAUSE")

int main(array<String^>^ argv)
{
    int port = 0;
	if (argv->Length != 1)
    {
        //Console::WriteLine("Usage: Server port");
        //Environment::Exit(1);
		port = 9999; 		
    }
	else{		

		try
		{
			port = Int32::Parse(argv[0]);
		}
		catch (FormatException^ e)
		{
			Console::WriteLine("Port number {0} is ill-formed", argv[0]);
			Environment::Exit(2);
		}
	}
		 // The range of valid ports for this system, usually 0-65535
/*1*/    if (port < IPEndPoint::MinPort || port > IPEndPoint::MaxPort)
    {
        Console::WriteLine("Port number must be in the range {0}-{1}",
            IPEndPoint::MinPort, IPEndPoint::MaxPort);
        Environment::Exit(3);
    }

/*2*/    // three possible ways to set the IP address
	     // The current IP address of the server computer
		 // should be set the same way on the client
		 IPAddress^ ipAddress = (IPAddress^)
			Dns::GetHostEntry(Dns::GetHostName())->AddressList[0];
		 
		 // or localhost - no matter what the actual IP address is
		 //IPAddress^ ipAddress = 
		 //Dns::GetHostEntry("localhost")->AddressList[0];
		 
		 // or 127.0.0.1 - this is the IP address for localhost
		 //IPAddress^ ipAddress = 
		 //Dns::GetHostEntry("127.0.0.1")->AddressList[0];

/*3*/    IPEndPoint^ ipEndpoint = gcnew IPEndPoint(ipAddress, port);

/*4*/    // TCP/IP is connection-oriented: using streams
		 Socket^ listenerSocket = gcnew Socket(ipAddress -> AddressFamily,
            SocketType::Stream, ProtocolType::Tcp);

/*5*/    listenerSocket->Bind(ipEndpoint);
         
/*6*/	 listenerSocket->Listen(3);// the max length of the queue of
                                   // clients waiting for connection is 1
/*7*/	 // the default valus of prperty Blocking is true
		 Console::WriteLine("Server listener blocking status is {0}",
			                    listenerSocket->Blocking);

/*8*/	 // make a server-side socket to handle each new connection
		 // method Accept() does not finish until a client
		 // attempts to connect
		 Socket^ serverSocket = listenerSocket->Accept();
			Console::WriteLine("New connection accepted");

/*9*/	 // this program will accept only one connection - 
		 // don't need the listenerSocket more
		 listenerSocket->Close();

		 // There is a connection now
		 // make a stream and on it a reader and a writer
/*10*/	 NetworkStream^ netStream = gcnew NetworkStream(serverSocket);
/*11*/	 BinaryReader^ br = gcnew BinaryReader(netStream);
/*12*/	 BinaryWriter^ bw = gcnew BinaryWriter(netStream);

	     //
		 try
		 {
			int value1, value2;
			int result;

			while (true) // until client ends the input
			{
/*13*/			value1 = br->ReadInt32();
/*14*/			value2 = br->ReadInt32();						
				Console::Write("Received values {0,3} and {1,3}",
									   value1, value2);

				result = value1 + value2;
/*15*/			bw->Write(result);                       
				Console::WriteLine(", sent result {0,3}", result);
			}
	     } // end try
/*16*/	 catch (EndOfStreamException^ e) // client ended stream
		 {
			Console::WriteLine("End of client data.");
		 }
/*17*/   catch (IOException^ e) // some real problem
		 {
			Console::WriteLine("IOException {0}", e);
		 }
		  
/*18*/   // make sure all data is sent and received before closing socket
		 serverSocket->Shutdown(SocketShutdown::Both);
/*19*/   serverSocket->Close();
/*20*/   netStream->Close();
         Console::WriteLine("Shutting down server");
		 system("PAUSE");
		 
}

