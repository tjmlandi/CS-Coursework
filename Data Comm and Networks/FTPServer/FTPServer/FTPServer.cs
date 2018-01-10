//Thomas Landi - Data Comm and Networks

using System.Net;
using System;
using System.Threading.Tasks;
using System.Collections.Generic;

namespace FTPServer
{
    public class FTPServer
    {
        static System.Net.Sockets.TcpListener ftpServer;
        static List<ConnectionHandle> connections;
        public static List<ConnectionHandle> Connections
        {
            get
            {
                return connections;
            }

            set
            {
                connections = value;
            }
        }

        static void Main(string[] args)
        {
            Start();
        }

        //Starts the server listening on port 21, continually adds new connection handles for any new clients
        public static void Start()
        {
            //Initialization of variables/listener
            IPAddress address;
            IPAddress.TryParse("127.0.0.1", out address);
            ftpServer = new System.Net.Sockets.TcpListener(address, 21);
            ftpServer.Start();

            Connections = new List<ConnectionHandle>();

            //Start new connection handles for new users
            while (true)
            { 
                if (ftpServer.Pending())
                {
                    Task<System.Net.Sockets.TcpClient> wait = ftpServer.AcceptTcpClientAsync();
                    wait.Wait();
                    System.Net.Sockets.TcpClient ftpConnection = wait.Result;

                    Console.WriteLine("User has connected");

                    Connections.Add(new ConnectionHandle(ftpConnection));
                }
            }
        }        
    }
}
