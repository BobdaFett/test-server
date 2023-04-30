#using "system.dll"

using namespace System;
using namespace System::Text;
using namespace System::IO;
using namespace System::Net;
using namespace System::Net::Sockets;

int Add(int, int);
int Subtract(int, int);

int main(array<String^>^ args) {
	int port = 2345;
	
	IPAddress^ ip = Dns::GetHostEntry("localhost")->AddressList[0];

	IPEndPoint^ endpoint = gcnew IPEndPoint(ip, port);

	Socket^ listener = gcnew Socket(ip->AddressFamily, SocketType::Stream, ProtocolType::Tcp);

	listener->Bind(endpoint);
	listener->Listen(1);

	Console::Write("Server is listening on {0}... ", ip);

	Socket^ server = listener->Accept();  // accept one connection from the listener.
	Console::WriteLine("Connection established.");

	listener->Close();

	NetworkStream^ ns = gcnew NetworkStream(server);
	BinaryReader^ reader = gcnew BinaryReader(ns);
	BinaryWriter^ writer = gcnew BinaryWriter(ns);

	// Only accepts a single connection, then exits.
	try {
		String^ value = "";

		// Run this while the server is getting input. An EndOfStreamException will exit this loop.
		// Could rearrange this to accept multiple different connections, and create some sort of "sessions."
		while (true) {
			Console::WriteLine("Attempting to read data.");
			value = reader->ReadString();
			Console::WriteLine("Data recieved was \"{0}\"", value);

			array<String^>^ commands = value->Split(' ');
			int endValue = 0;
			if (commands[0] == "ADD") {
				Console::WriteLine("Adding values.");
				// Get values from the command string.
				int num1 = Int32::Parse(commands[1]);
				int num2 = Int32::Parse(commands[2]);
				endValue = Add(num1, num2);
			}
			else if (commands[0] == "SUBTRACT") {
				Console::WriteLine("Subtracting values.");
				// Get values from the command string.
				int num1 = Int32::Parse(commands[1]);
				int num2 = Int32::Parse(commands[2]);
				endValue = Subtract(num1, num2);
			}
			else {
				Console::WriteLine("Command not found.");
			}

			Console::Write("Sending response... ", value);
			String^ response = "Recieved.";
			writer->Write(endValue);
			Console::WriteLine("Sent {0}", endValue);
		}
	}
	catch (EndOfStreamException^ e) {
		Console::WriteLine("\n\nThe client has terminated the connection.");
	}
	catch (IOException^ e) {
		Console::WriteLine("An error has occurred:\n{0}", e);
	}
	
	Console::Write("Shutting down... ");

	server->Shutdown(SocketShutdown::Both);
	server->Close();
	ns->Close();
	
	Console::WriteLine("Complete.");
	Console::Write("\nPress any key to exit...");
	Console::ReadKey();
}

int Add(int a, int b) {
	// Add values and return new value.
	return a + b;
}

int Subtract(int a, int b) {
	// Subtract values and return new value.
	return a - b;
}