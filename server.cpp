#using "system.dll"

using namespace System;
using namespace System::IO;
using namespace System::Net;
using namespace System::Net::Sockets;

#include <cstdlib>

int main(array<String^>^ args) {
	int port = 2345;
	
	IPAddress^ ip = Dns::GetHostEntry("localhost")->AddressList[0];

	IPEndPoint^ endpoint = gcnew IPEndPoint(ip, port);

	Socket^ listener = gcnew Socket(ip->AddressFamily, SocketType::Stream, ProtocolType::Tcp);

	listener->Bind(endpoint);
	listener->Listen(2);

	Console::Write("Server is listening... ");

	Socket^ server = listener->Accept();  // accept one connection from the listener.
	Console::WriteLine("Connection established.");

	listener->Close();

	NetworkStream^ ns = gcnew NetworkStream(server);
	BinaryReader^ reader = gcnew BinaryReader(ns);
	BinaryWriter^ writer = gcnew BinaryWriter(ns);

	// Only accepts a single connection, then exits.
	try {
		int value = 0;

		// Run this while the server is getting input. A specific error will end the stream.
		while (true) {
			Console::WriteLine("Attempting to read data.");
			value = reader->ReadInt32();

			Console::WriteLine("Data recieved was {0}", value);
			Console::Write("Sending {0} + 100... ", value);

			value += 100;
			writer->Write(value);
			Console::WriteLine("Sent.");
		}
	}
	catch (EndOfStreamException^ e) {
		Console::WriteLine("The client has terminated the connection.");
	}
	catch (IOException^ e) {
		Console::WriteLine("An error has occurred:\n{0}", e);
	}
	
	server->Shutdown(SocketShutdown::Both);
	server->Close();
	ns->Close();

	Console::WriteLine("Shutting down... ");
	Console::Write("Press any key...");
	Console::ReadKey();
}