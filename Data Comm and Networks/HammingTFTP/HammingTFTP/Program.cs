//Thomas Landi - Data Comm and Networks

using System;
using System.Net;
using System.IO;
using System.Net.Sockets;
using System.Text;
using System.Collections.Generic;
using System.Linq;


namespace HammingTFTP
{
    class HammingTFTP
    {
        static UdpClient client;            //Client reference
        static bool error = true;           //Whether or not we are in error mode
        static string filename;             //Requested file
        static IPEndPoint serverEndpoint;   //Endpoint of the server

        static void Main(string[] args)
        {
            //Make sure we have the correct arguments being provided
            if (args.Length != 2)
            {
                Console.WriteLine("Usage: HammingTFTP.exe [error|noerror]  tftp-host file");
                Console.ReadLine();
                return;
            }

            try
            {
                //Determine if we are requesting with or without errors
                if (args[0] == "noerror")
                    error = false;

                //Grab the requested filename
                filename = args[1];
               
                //Get the ip address of the host
                var host = Dns.GetHostEntry("kayrun.cs.rit.edu");
                String serverAddressStr = "";
                foreach (var ip in host.AddressList)
                {
                    if (ip.AddressFamily == AddressFamily.InterNetwork && ip.ToString().Substring(0, 3) == "129")
                    {
                        serverAddressStr = ip.ToString();
                    }
                }
                IPAddress serverAddress;
                IPAddress.TryParse(serverAddressStr, out serverAddress);

                //Connect to the server endpoint
                serverEndpoint = new IPEndPoint(serverAddress, 7000);
                client = new UdpClient();                
                client.Connect(serverEndpoint);

                //Write the initial request block
                byte[] octToByte = Encoding.ASCII.GetBytes("octet");
                byte[] fileToBytes = Encoding.ASCII.GetBytes(filename);
                byte[] request = new byte[4 + octToByte.Length + fileToBytes.Length];

                request[0] = 0;
                request[1] = (byte)(error ? 2 : 1);
                Array.Copy(fileToBytes, 0, request, 2, fileToBytes.Length);
                request[fileToBytes.Length + 2] = 0;
                Array.Copy(octToByte, 0, request, fileToBytes.Length + 3, octToByte.Length);
                request[request.Length - 1] = 0;                
                
                //Send the block
                client.Send(request, request.Length);

                byte[] response = client.Receive(ref serverEndpoint);

                //If we receive an error, print the message and return
                if (response[1] == 5)
                {
                    Console.WriteLine(Encoding.ASCII.GetString(response.Skip(4).ToArray()));
                    return;
                }

                //Remove the opcode and block number
                response = response.Skip(4).ToArray();

                //Download the rest of the file
                DownloadFile(response);
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
                Console.ReadLine();
            }
        }

        //Downloads the file, based on the initial server response
        static void DownloadFile(byte[] response)
        {
            byte[] fullResponse = new byte[0];
            bool complete = (response.Length < 512) ? true : false;
            bool failed = false;

            //Undo the network ordering of the response
            ReverseNetworkOrder(ref response);

            //Verify the data, if verified, copy it to the final byte array
            if (VerifyData(ref response))
            {
                response = RemoveHammingBits(response);

                fullResponse = new byte[response.Length];
                Array.Copy(response, 0, fullResponse, 0, response.Length);
            }
            else
            {
                failed = true;
            }
            short blockNumber = 1;

            //If the file isn't finished downloading, or we had a >2 bit error, request new response
            while (!complete || failed)
            {
                //If the last response was an error, request the same block
                if (failed)
                {
                    byte[] request = new byte[4];

                    request[0] = 0;
                    request[1] = 6;
                    request[2] = (byte)(blockNumber >> 8);
                    request[3] = (byte)blockNumber;

                    client.Send(request, request.Length);

                    response = client.Receive(ref serverEndpoint);

                    if (response[1] == 5)
                    {
                        Console.WriteLine(Encoding.ASCII.GetString(response.Skip(2).ToArray()));
                        return;
                    }
                }
                //If there was no error, request the next block
                else
                {
                    byte[] request = new byte[4];

                    request[0] = 0;
                    request[1] = 4;
                    request[2] = (byte)(blockNumber >> 8);
                    request[3] = (byte)blockNumber;

                    client.Send(request, request.Length);

                    response = client.Receive(ref serverEndpoint);

                    if (response[1] == 5)
                    {
                        Console.WriteLine(Encoding.ASCII.GetString(response.Skip(2).ToArray()));
                        return;
                    }

                    ++blockNumber;
                }

                //Remove the op code/block number
                response = response.Skip(4).ToArray();

                //If we are at the end of the file, don't repeat the loop
                complete = (response.Length < 512) ? true : false;

                //Undo the network order
                ReverseNetworkOrder(ref response);

                //If the data checks out, copy it to the final array
                if (VerifyData(ref response))
                {
                    response = RemoveHammingBits(response);

                    byte[] tmp = new byte[fullResponse.Length];
                    Array.Copy(fullResponse, tmp, fullResponse.Length);

                    fullResponse = new byte[fullResponse.Length + response.Length];
                    Array.Copy(tmp, 0, fullResponse, 0, tmp.Length);
                    Array.Copy(response, 0, fullResponse, tmp.Length, response.Length);

                    failed = false;
                }
                //Otherwise, loop again
                else
                {
                    failed = true;
                }
            }

            //Remove trailing 0's
            while (fullResponse[fullResponse.Length - 1] == 0)
                fullResponse = fullResponse.Take(fullResponse.Length - 2).ToArray();

            //Write the byte array to a file
            File.WriteAllBytes(filename, fullResponse);
        }

        //Undoes the network byte order, so the bytes can be used by the program
        static void ReverseNetworkOrder(ref byte[] toReverse)
        {
            //Loop through by fours, reversing each four bytes
            for (int i = 0; i < toReverse.Length - 3; i += 4)
            {
                byte tmp = toReverse[i];
                toReverse[i] = toReverse[i + 3];
                toReverse[i + 3] = tmp;

                tmp = toReverse[i + 1];
                toReverse[i + 1] = toReverse[i + 2];
                toReverse[i + 2] = tmp;
            }
        }

        //Removes the hamming bits from the supplied array
        //Returns the new array of bytes properly flipped and aligned\
        //Removes trailing 0's from the block
        static byte[] RemoveHammingBits(byte[] toCorrect)
        {
            //First, copy the array into a list of bools, for easier individual bit handling
            List<bool> bitList = new List<bool>();
            List<bool> resultBitList = new List<bool>();

            for (int i = 0; i < toCorrect.Length; ++i)
            {
                for (int offset = 7; offset >= 0; --offset)
                {
                    if ((toCorrect[i] & (1 << offset)) != 0)
                    {
                        bitList.Add(true);
                    }
                    else
                    {
                        bitList.Add(false);
                    }
                }
            }

            //Loop while we still have unmodified bits
            List<bool> previous = new List<bool>();
            while (bitList.Count > 0)
            {
                //Get the next 32 bits
                List<bool> tmpChunk = new List<bool>();

                tmpChunk.AddRange(bitList.Take(32).ToList());
                bitList.RemoveRange(0, 32);

                //Remove the hamming bits themselves
                tmpChunk.RemoveAt(31);
                tmpChunk.RemoveAt(30);
                tmpChunk.RemoveAt(28);
                tmpChunk.RemoveAt(24);
                tmpChunk.RemoveAt(16);
                tmpChunk.RemoveAt(0);

                //Prepend with any bits from the previous chunk
                tmpChunk.InsertRange(0,previous);
                previous.RemoveRange(0, previous.Count);

                //Flip the first 3 bytes, and add them to the finished list
                List<bool> tmpByte = tmpChunk.Take(8).ToList();
                tmpChunk.RemoveRange(0, 8);
                tmpByte.Reverse();

                resultBitList.AddRange(tmpByte);


                tmpByte = tmpChunk.Take(8).ToList();
                tmpChunk.RemoveRange(0, 8);
                tmpByte.Reverse();


                resultBitList.AddRange(tmpByte);


                tmpByte = tmpChunk.Take(8).ToList();
                tmpChunk.RemoveRange(0, 8);
                tmpByte.Reverse();

                resultBitList.AddRange(tmpByte);

                //If the last section is less than a byte, save it for the next chunk
                //Otherwise, flip and add it
                if (tmpChunk.Count < 8)
                {
                    previous = tmpChunk.Take(tmpChunk.Count).ToList();
                    tmpChunk.RemoveRange(0, tmpChunk.Count);
                }
                else
                {
                    tmpByte = tmpChunk.Take(8).ToList();
                    tmpChunk.RemoveRange(0, 8);
                    tmpByte.Reverse();

                    resultBitList.AddRange(tmpByte);

                    previous.RemoveRange(0, previous.Count);
                }
            }

            resultBitList.AddRange(previous);

            List<byte> corrected = new List<byte>();
            
            //Add the inidividual bits to an array of bytes
            while (resultBitList.Count >= 8)
            {
                bool[] tmpByte = resultBitList.Take(8).ToArray();
                resultBitList.RemoveRange(0, 8);

                byte insertedByte = 0;

                for (int i = 0; i < 8; ++i)
                {
                    if (tmpByte[i])
                        insertedByte += (byte)Math.Pow(2, 7 - i);
                }

                corrected.Add(insertedByte);
            }

            return corrected.ToArray();
        }
        
        //Iterates through a block of data, submitting the data 4 blocks at a time to the hamming code
        //Returns false if any have more than a 2 bit error
        //Fills the original array with the corrected values
        static bool VerifyData(ref byte[] toCorrect)
        {
            List<byte> corrected = new List<byte>();
            for (int i = 0; i < toCorrect.Length - 3; i += 4)
            {
                byte[] tmp;
                if (ErrorCorrection(toCorrect.Skip(i).Take(4).ToArray(), out tmp))
                {
                    corrected.AddRange(tmp);
                }
                else
                {
                    return false;
                }
            }

            toCorrect = corrected.ToArray();
            return true;
        }

        //Uses Hamming code to detect >2 bit errors, or correct 1 bit errors
        //Returns true if correctable/correct
        //Returns false if >2 bit error
        //out parameter contains corrected byte array, or original array if uncorrectable
        //assumes toCorrect is 4 bytes
        //assumes 1 parity bit on far left
        static bool ErrorCorrection(byte[] toCorrect, out byte[] corrected)
        {
            corrected = new byte[4];
            toCorrect.CopyTo(corrected, 0);
            //Overall check
            int count = 0;
            int errorLocation = 0;
            //1 Parity Loop
            for (int i = 1; i < 8; i += 2)
            {
                if ((toCorrect[3] & (1 << (i-1))) != 0)
                    ++count;

                if ((toCorrect[2] & (1 << (i-1))) != 0)
                    ++count;

                if ((toCorrect[1] & (1 << (i-1))) != 0)
                    ++count;

                if ((toCorrect[0] & (1 << (i-1))) != 0)
                    ++count;

            }
            if (count%2 != 0)
                errorLocation += 1;

            //2 Parity Loop       
            count = 0;
            for (int i = 1; i <= 32; i += 4)
            {
                int shift = i % 8;
                if ((toCorrect[3 - (i/8)] & (1 << shift)) != 0)
                    ++count;

                int j = i + 1;
                shift = j % 8;
                if ((toCorrect[3 - (j/8)] & (1 << shift)) != 0)
                    ++count;
            }
            if (count%2 != 0)
                errorLocation += 2;

            //4 Parity Loop
            count = 0;
            for (int i = 3; i <= 32; i += 8)
            {
                for (int k = 0; k < 4; ++k)
                {
                    int j = i + k;
                    int shift = j % 8;
                    if ((toCorrect[3 - (j/8)] & (1 << shift)) != 0)
                        ++count;
                }
            }
            if (count%2 != 0)
                errorLocation += 4;


            //8 Parity Loop
            count = 0;
            for (int i = 7; i <= 32; i += 16)
            {
                for (int k = 0; k < 8; ++k)
                {
                    int j = i + k;
                    int shift = j % 8;
                    if ((toCorrect[3 - (j/8)] & (1 << shift)) != 0)
                        ++count;
                }
            }
            if (count%2 != 0)
                errorLocation += 8;

            //16 Parity Loop
            count = 0;
            for (int i = 15; i <= 32; i += 32)
            {
                for (int k = 0; k < 16; ++k)
                {
                    int j = i + k;
                    int shift = j % 8;
                    if ((toCorrect[3 - (j/8)] & (1 << shift)) != 0)
                        ++count;
                }
            }
            if (count%2 != 0)
                errorLocation += 16;

            if (errorLocation == 0)
                return true;


            corrected[3-(errorLocation/8)] = (byte)(corrected[3-(errorLocation/8)] ^ (1 << (errorLocation%8)-1));

            //Total parity check
            count = 0;
            if ((toCorrect[3] & 1) != 0)            //1
                ++count;
            if ((toCorrect[3] & 2) != 0)            //2
                ++count;
            if ((toCorrect[3] & (1 << 3)) != 0)     //4
                ++count;
            if ((toCorrect[3] & (1 << 7)) != 0)     //8
                ++count;
            if ((toCorrect[2] & (1 << 7)) != 0)     //16
                ++count;
            if ((toCorrect[0] & (1 << 7)) != 0)     //32
                ++count;

            if (count%2 != 0)
                return false;


            return true;
        }
    }
}

