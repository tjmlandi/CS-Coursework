using System;
using System.Threading.Tasks;
using System.Net;
using System.IO;



namespace ChatClient
{
    class Program
    {
        static System.Net.Sockets.TcpClient client;
        static int userLength;
        static string username;
        static bool gottenUser;

        static void Main(string[] args)
        {
            //Initialize variables, connect tcp client
            IPAddress address;
            IPAddress.TryParse("127.0.0.1", out address);
            int port = 7777;
            client = new System.Net.Sockets.TcpClient();
            Task wait = client.ConnectAsync(address, port);
            wait.Wait();

            //client.Connect(endpoint);

            //Start tasks to run the writing messages to the stream, and the reading of messages from the stream
            Task writeTask = new Task(write);
            Task readTask = new Task(read);

            gottenUser = false;

            writeTask.Start();
            readTask.Start();

            writeTask.Wait();
            readTask.Wait();
        }

        //Reads input from the user, and writes it to the client stream
        private static void write()
        {
            StreamWriter writer = new StreamWriter(client.GetStream());
            while (true)
            {
                string str = Console.ReadLine();
                //Saves the user's username from the first input
                if (!gottenUser)
                {
                    username = str;
                    userLength = str.Length + 1;
                    gottenUser = true;
                }
                writer.WriteLine(str);
                writer.Flush();
            }
        }

        //Reads messages from the chat room stream, and writes it to the console
        private static void read()
        {
            StreamReader reader = new StreamReader(client.GetStream());
            while (true)
            {
                string message = reader.ReadLine();

                //Check if the message contains an '@' mention
                int index = message.IndexOf("@" + username);
                
                //If so, highlight the user's name and play the windows 'beep' sound
                if (index >= 0)
                {
                    
                    Console.Write(message.Substring(0, index));
                    Console.ForegroundColor = ConsoleColor.Red;
                    Console.Write("@" + username);
                    Console.ForegroundColor = ConsoleColor.Gray;
                    Console.WriteLine(message.Substring(index + userLength));
                    Console.WriteLine(new string((char)7, 1));
                }
                //Otherwise, just write the message normally
                else
                {
                    Console.WriteLine(message);
                }
            }
        }
    }
}
