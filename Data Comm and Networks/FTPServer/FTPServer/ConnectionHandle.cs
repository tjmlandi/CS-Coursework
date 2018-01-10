//Thomas Landi - Data Comm and Networks

using System.IO;
using System;
using System.Threading.Tasks;
using System.Net.Sockets;
using System.Net;

namespace FTPServer
{
    public class ConnectionHandle
    {
        TcpClient client;
        TcpClient activeClient;
        TcpListener pasvListener;
        StreamReader reader;
        StreamWriter writer;
        IPEndPoint clientPoint;
        string username;
        string currentTransferType = "A";
        bool isPassive = true;

        public TcpClient Client
        {
            get
            {
                return client;
            }

            set
            {
                client = value;
            }
        }

        public string Username
        {
            get
            {
                return username;
            }

            set
            {
                username = value;
            }
        }

        //Constructor initializes variables, and starts the task which runs the interaction between the 
        //server and a single client
        public ConnectionHandle(TcpClient tcpClient)
        {
            Client = tcpClient;            
            Task ftpThread = new Task(startTransfer);
            ftpThread.Start();
        }

        //Get the username from the client, and start the main chat method
        private void startTransfer()
        {
            //Get the client streams
            reader = new StreamReader(Client.GetStream(), System.Text.Encoding.ASCII);
            writer = new StreamWriter(Client.GetStream(), System.Text.Encoding.ASCII);

            //Send the welcome message
            writer.WriteLine("220 Welcome the my ftp server!");
            writer.Flush();
            
            runFTP();
        }

        //Runs the main ftp loop
        private void runFTP()
        {
            try
            {
                while (true)
                {
                    //Grab the latest command from the client
                    string line = reader.ReadLine();

                    Console.WriteLine("line: " + line);
                    
                    //Separate out the command from the arguments
                    string command = line.Split(' ')[0].ToUpper();
                    string argString = line.Substring(command.Length).Trim();
                    string[] args = argString.Split(' ');

                    Console.WriteLine("Command: " + command);
                    Console.WriteLine("Args: " + argString);

                    //Main switch for supported commands
                    switch (command)
                    {
                        case "PASS":
                            PassCheck(argString);
                            break;
                        case "USER":
                            UserRegister(username);
                            break;
                        case "TYPE":
                            ChangeType(args[0], (args.Length > 1) ? args[1] : null);
                            break;
                        case "PWD":
                            writer.WriteLine("257 \"/\" is the current directory.");
                            writer.Flush();
                            break;
                        case "PORT":
                            PortConnection(argString);
                            break;
                        case "PASV":
                            PassiveConnection();
                            break;
                        case "RETR":
                            RetreiveFile(argString);
                            break;
                        default:
                            writer.WriteLine("200 Command not supported");
                            writer.Flush();
                            break;
                    }
                }
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
                FTPServer.Connections.Remove(this);
            }
        }

        //Method for retreiving a file from the server
        private void RetreiveFile(string filepath)
        {    
            if (File.Exists(filepath))
            {
                if (isPassive)
                {
                    pasvListener.BeginAcceptTcpClient(Retreival, filepath);
                }
                else
                {
                    activeClient = new TcpClient();
                    activeClient.BeginConnect(clientPoint.Address, clientPoint.Port, Retreival, filepath);
                }

                writer.WriteLine("150 Opening " + (isPassive ? "passive" : "active") + " mode data transfer for RETR");
                writer.Flush();
            }
            else
            {
                writer.WriteLine("550 File Not Found");
                writer.Flush();
            }
        }

        //Method for handling the passive listener/ active client and transferring data from the file stream to the data connection
        private void Retreival(IAsyncResult result)
        {
            if (isPassive)
                activeClient = pasvListener.EndAcceptTcpClient(result);
            else
                activeClient.EndConnect(result);

            string filepath = (string)result.AsyncState;

            using (NetworkStream dataStream = activeClient.GetStream())
            using (FileStream fileStream = new FileStream(filepath, FileMode.Open, FileAccess.Read))
            {
                TransferData(fileStream, dataStream);

                activeClient.Close();
                activeClient = null;
            }

            writer.WriteLine("226 Data Connection Closed");
            writer.Flush();
        }

        //Generic method to transfer information from one stream to another
        private void TransferData(Stream origin, Stream destination)
        {
            //Handles binary transfers
            if (currentTransferType == "I")
            {
                byte[] buffer = new byte[4096];
                int count = 0;

                while ((count = origin.Read(buffer, 0, buffer.Length)) > 0)
                    destination.Write(buffer, 0, count);
            }
            //Handles ASCII transfers
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

        //Enables passive connection on the server
        private void PassiveConnection()
        {
            //Starts the passive listener
            isPassive = true;
            IPAddress pasvAddress = ((IPEndPoint)client.Client.LocalEndPoint).Address;

            pasvListener = new TcpListener(pasvAddress, 0);
            pasvListener.Start();

            IPEndPoint pasvEndpoint = ((IPEndPoint)pasvListener.LocalEndpoint);

            //Let's the client know the server is listening on the passive data connection
            byte[] address = pasvEndpoint.Address.GetAddressBytes();
            short port = (short)pasvEndpoint.Port;

            byte[] portArray = BitConverter.GetBytes(port);

            if (BitConverter.IsLittleEndian)
                Array.Reverse(portArray);

            writer.WriteLine(string.Format("227 Passive Mode Enabled on ({0},{1},{2},{3},{4},{5})", address[0], address[1], address[2], address[3], portArray[0], portArray[1]));
            writer.Flush();
        }

        //Enables active data transfer
        private void PortConnection(string args)
        {
            //Gets the address and port to connect to from the client
            isPassive = false;
            string[] ipPort = args.Split(',');

            byte[] ipAddress = new byte[4];
            byte[] port = new byte[2];

            for (int i = 0; i < 4; i++)
                ipAddress[i] = Convert.ToByte(ipPort[i]);

            for (int i = 4; i < 6; i++)
                port[i - 4] = Convert.ToByte(ipPort[i]);
            
            int intPort = port[0] * 256 + port[1];

            clientPoint = new IPEndPoint(new IPAddress(ipAddress), intPort);

            writer.WriteLine("200 Data Connection Established");
            writer.Flush();
        }

        //Checks the password (always allowed)
        private void PassCheck(string password)
        {
            writer.WriteLine("230 User Logged In");
            writer.Flush();
        }

        //Saves the username
        private void UserRegister(string username)
        {
            Username = username;
            writer.WriteLine("331 Username accepted");
            writer.Flush();
        }

        //Changes between ASCII and binary transfer types
        private void ChangeType(string code, string format)
        {
            string response;

            switch (code)
            {
                case "A":
                    response = "200 ASCII Enabled";
                    currentTransferType = "A";
                    break;
                case "I":
                    response = "200 Binary Enabled";
                    currentTransferType = "I";
                    break;
                default:
                    response = "504 Type not supported";
                    break;
            }

            if (format != null)
            {
                switch (format)
                {
                    case "N":
                        response = "200 Ok";
                        break;
                    default:
                        response = "504 Type not supported";
                        break;
                }
            }

            if (response != null)
            {
                writer.WriteLine(response);
                writer.Flush();
            }
        }
    }
}
