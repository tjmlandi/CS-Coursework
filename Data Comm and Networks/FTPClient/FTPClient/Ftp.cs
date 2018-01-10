//Thomas Landi - Data Comm and Networks

using System;
using System.Net;
using System.IO;
using System.Net.Sockets;

namespace FTP
{
    class FTP
    {
        static TcpClient client;
        static TcpClient pasvClient;
        static TcpListener activeListener;
        static IPEndPoint pasvEndpoint;
        static bool debug = true;
        static string addressStr;
        static string activeAddress;
        static string activePort = "2121";
        static StreamWriter writer;
        static StreamReader reader;
        static bool isPassive = true;
        static string currentType = "A";

        static readonly String[] HELP_MESSAGE = {
    "ascii      --> Set ASCII transfer type",
    "binary     --> Set binary transfer type",
    "cd <path>  --> Change the remote working directory",
    "cdup       --> Change the remote working directory to the",
        "               parent directory (i.e., cd ..)",
    "debug      --> Toggle debug mode",
    "dir        --> List the contents of the remote directory",
    "get path   --> Get a remote file",
    "help       --> Displays this text",
    "passive    --> Toggle passive/active mode",
    "put path   --> Transfer the specified file to the server",
    "pwd        --> Print the working directory on the server",
    "quit       --> Close the connection to the server and terminate",
    "user login --> Specify the user name (will prompt for password" };

        static void Main(string[] args)
        {
            //Make sure we have the correct arguments being provided
            if (args.Length != 1)
            {
                Console.WriteLine("Usage: [mono] Ftp server");
                return;
            }

            try
            {
                //Grab the IP address of the machine running the client to use for active transfer 
                //Note: this should be run with a machine that has a global IP
                var host = Dns.GetHostEntry(Dns.GetHostName());
                foreach (var ip in host.AddressList)
                {
                    if (ip.AddressFamily == AddressFamily.InterNetwork && ip.ToString().Substring(0,3) == "129")
                    {
                        activeAddress = ip.ToString();
                    }
                }

                //Grab the ftp server address
                addressStr = args[0];
                //Initialize variables, connect tcp client
                IPAddress address;
                IPAddress.TryParse(addressStr, out address);
                //Connect to the ftp server
                IPEndPoint endpoint = new IPEndPoint(address, 21);
                client = new TcpClient();
                client.Connect(endpoint);

                //Save the stream reader/writer
                writer = new StreamWriter(client.GetStream());
                reader = new StreamReader(client.GetStream());

                //Get the welcome message
                string welcome = reader.ReadLine();

                //Exit if we fail to connect
                if (welcome.Substring(0, 3) != "220")
                {
                    Console.WriteLine("Error: Connection Failed. Please try running the client again.");
                    return;
                }
                Console.WriteLine(welcome);


                //Send the username and password
                Console.Write("Username: ");
                string username = Console.ReadLine();

                SendCommand("USER " + username);

                Console.Write("Password: ");
                string password = Console.ReadLine();

                SendCommand("PASS " + password);

                TogglePassive();

                //Main command loop
                while (true)
                {
                    Console.Write("ftp> ");
                    string line = Console.ReadLine();

                    string command = line.Split(' ')[0].ToLower();
                    string argString = line.Substring(command.Length).Trim();
                    string[] arguments = argString.Split(' ');

                    //Switch statement for all the commands
                    switch (command)
                    {
                        case "ascii":
                            ChangeType("A");
                            break;
                        case "binary":
                            ChangeType("I");
                            break;
                        case "cd":
                            SendCommand("CWD " + argString);
                            break;
                        case "cdup":
                            SendCommand("CDUP");
                            break;
                        case "debug":
                            debug = debug ? false : true;
                            break;
                        case "dir":
                            CurrentDirectory();
                            break;
                        case "get":
                            GetFile(argString);
                            break;
                        case "help":
                            foreach (string msg in HELP_MESSAGE)
                                Console.WriteLine(msg);
                            break;
                        case "passive":
                            TogglePassive();
                            break;
                        case "put":
                            PutFile(argString);
                            break;
                        case "pwd":
                            SendCommand("PWD");
                            break;
                        case "quit":
                            Console.WriteLine("Goodbye");
                            return;
                        case "user":
                            break;
                        default:
                            Console.WriteLine("Command not recognized. Valid commands below: ");
                            foreach (string msg in HELP_MESSAGE)
                                Console.WriteLine(msg);
                            break;
                    }
                }
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
            }
        }

        //Method for getting the requested file from the ftp server
        static void GetFile(string filename)
        {
            string response = SendCommand("RETR " + filename);
            if (response[0] != '5' || response[0] != '4') 
            {
                //Handle differently based on if we are in active or passive mode
                if (isPassive)
                {
                    pasvClient = new TcpClient();
                    pasvClient.BeginConnect(pasvEndpoint.Address, pasvEndpoint.Port, FileSave, filename);
                }
                else
                {
                    activeListener.BeginAcceptTcpClient(FileSave, filename);
                }
            }
            else
            {
                Console.WriteLine("File Not Found");
            }
        }

        //Saves the file to the local directory
        static void FileSave(IAsyncResult result)
        {
            //Get the stream data from either the active or passive data connection
            if (isPassive)
                pasvClient.EndConnect(result);
            else
                pasvClient = activeListener.EndAcceptTcpClient(result);

            string filepath = (string)result.AsyncState;

            //Transfer the data and close the tcp client for the data connection
            using (NetworkStream dataStream = pasvClient.GetStream())
            using (FileStream fileStream = File.Open(filepath, FileMode.CreateNew))
            {
                TransferData(dataStream, fileStream);

                pasvClient.Close();
                pasvClient = null;
            }

            Console.WriteLine("File Saved");
        }

        //Method to push the provided file to the server
        static void PutFile(string filename)
        {
            string response = SendCommand("STOR " + filename);
            if (File.Exists(filename))
            {
                //Handle differently based on if we are in active or passive mode
                if (isPassive)
                {
                    pasvClient = new TcpClient();
                    pasvClient.BeginConnect(pasvEndpoint.Address, pasvEndpoint.Port, Retreival, filename);
                }
                else
                {
                    activeListener.BeginAcceptTcpClient(Retreival, filename);
                }                
            }
            else
            {
                Console.WriteLine("File Not Found");
            }
        }

        //Method for retrieving a file from the local directory and transferring it to the data stream
        private static void Retreival(IAsyncResult result)
        {
            //Get the stream data from either the active or passive data connection
            if (isPassive)
                pasvClient.EndConnect(result);
            else
                pasvClient = activeListener.EndAcceptTcpClient(result);

            string filepath = (string)result.AsyncState;

            //Transfer the data and close the tcp client for the data connection
            using (NetworkStream dataStream = pasvClient.GetStream())
            using (FileStream fileStream = new FileStream(filepath, FileMode.Open, FileAccess.Read))
            {
                TransferData(fileStream, dataStream);

                pasvClient.Close();
                pasvClient = null;
            }

            Console.WriteLine("File Transferred");
        }

        //Generic function for transferring data from one stream to another
        private static void TransferData(Stream origin, Stream destination)
        {
            //Handles binary transfer
            if (currentType == "I")
            {
                byte[] buffer = new byte[4096];
                int count = 0;

                while ((count = origin.Read(buffer, 0, buffer.Length)) > 0)
                    destination.Write(buffer, 0, count);
            }
            //Handles ASCII transfer
            else
            {
                char[] buffer = new char[4096];
                int count = 0;

                using (StreamReader tmpReader = new StreamReader(origin))
                using (StreamWriter tmpWriter = new StreamWriter(destination, System.Text.Encoding.ASCII))
                {
                    while ((count = tmpReader.Read(buffer, 0, buffer.Length)) > 0)
                        tmpWriter.Write(buffer, 0, count);
                }
            }            
        }

        //Method for getting the information for the current directory on the ftp server
        static void CurrentDirectory()
        {
            string response = SendCommand("LIST");
            //Similar to the setup for the data transfer
            if (isPassive)
            {
                pasvClient = new TcpClient();
                pasvClient.BeginConnect(pasvEndpoint.Address, pasvEndpoint.Port, ListRetreival, "test");
            }
            else
            {
                activeListener.BeginAcceptTcpClient(ListRetreival, "test");
            }
        }

        //Retrieves the directory infromation from the data connection
        //Same as the retreival method, but transfers the data from the connection stream to the console out stream instead
        private static void ListRetreival(IAsyncResult result)
        {
            if (isPassive)
                pasvClient.EndConnect(result);
            else
                pasvClient = activeListener.EndAcceptTcpClient(result);
        

            using (NetworkStream dataStream = pasvClient.GetStream())
            {
                TransferData(dataStream, Console.OpenStandardOutput());

                pasvClient.Close();
                pasvClient = null;
            }

            Console.WriteLine("File Transferred");
        }

        //Toggles between passive and active data transfer
        static void TogglePassive()
        {
            //If we are already passive, send the ip address and port of the active connection to the server, and start listening to the port
            if (isPassive)
            {
                ushort number = Convert.ToUInt16(activePort);
                byte upper = (byte)(number >> 8);
                byte lower = (byte)(number & 0xff);
                string[] ip = activeAddress.Split('.');
                string response = SendCommand("PORT " + ip[0] + "," + ip[1] + "," + ip[2] + "," + ip[3] + "," + upper + "," + lower);

                IPAddress address;
                IPAddress.TryParse(activeAddress, out address);
                IPEndPoint activeEndpoint = new IPEndPoint(address, int.Parse(activePort));

                activeListener = new TcpListener(activeEndpoint);
                activeListener.Start();

                isPassive = false;
            }
            //Otherwise, listen for the server's data port and save the endpoint to connect to later
            else
            {
                string response = SendCommand("PASV");

                string ipStr = response.Split('(', ')')[1];

                string[] ipPort = ipStr.Split(',');

                byte[] ipAddress = new byte[4];
                byte[] port = new byte[2];

                for (int i = 0; i < 4; i++)
                    ipAddress[i] = Convert.ToByte(ipPort[i]);

                for (int i = 4; i < 6; i++)
                    port[i - 4] = Convert.ToByte(ipPort[i]);

                int intPort = port[0] * 256 + port[1];

                pasvEndpoint = new IPEndPoint(new IPAddress(ipAddress), intPort);               

                isPassive = true;
            }
        }

        //Sends a command to the netwwork stream, and returns the server's response
        static string SendCommand(string command)
        {
            writer.WriteLine(command);
            writer.Flush();
            string response = reader.ReadLine();
            //Write out the response if it is an error
            if (response[0] == '4' || response[0] == '5')
                Console.WriteLine("The command: " + command + " received a negative response of " + response);
            if (debug) Console.WriteLine(response);
            return response;
        }

        //Change the current data type to the provided type
        static void ChangeType(string type)
        {
            currentType = type;
            SendCommand("TYPE " + type);
        }
    }
}
