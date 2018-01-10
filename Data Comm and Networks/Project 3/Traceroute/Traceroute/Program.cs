using System;
using System.Collections.Generic;
using System.Text;
using System.Net;
using System.Net.NetworkInformation;

namespace Traceroute
{
    class Program
    {
        static string data = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
        static int timeout = 1000;

        static void Main(string[] args)
        {
			if (args.Length < 1)
			{
				Console.WriteLine("Usage: Traceroute [hostname]");
				return;
			}

			string hostname = args[0];
			IPAddress[] addresslist = Dns.GetHostAddresses(hostname);

			IPAddress ipv4Address = null;
			IPAddress ipv6Address = null;
			
			foreach (IPAddress address in addresslist)
			{
				if (address.AddressFamily == System.Net.Sockets.AddressFamily.InterNetworkV6)
				{
					ipv6Address = address;
					break;
				}
			}

			foreach (IPAddress address in addresslist)
			{
				if (address.AddressFamily == System.Net.Sockets.AddressFamily.InterNetwork)
				{
					ipv4Address = address;
					break;
				}
			}

			if (ipv4Address != null)
			{
				Console.Write("Sending packets over IPv4...");
				List<Tuple<IPAddress, long, long, long>> results = TracerouteRecur(ipv4Address.ToString(), 1);
				Console.WriteLine();
				foreach (var result in results)
				{
					Console.WriteLine(results.IndexOf(result).ToString() + "\t"
									+ (result.Item2 <= -1 ? "*\t" : result.Item2.ToString() + "ms\t") 
									+ (result.Item2 <= -1 ? "*\t" : result.Item2.ToString() + "ms\t") 
									+ (result.Item2 <= -1 ? "*\t" : result.Item2.ToString() + "ms\t") 
									+ result.Item1.ToString());
				}
			}

			
			if (ipv6Address != null)
			{
				Console.Write("Sending packets over IPv6...");
				List<Tuple<IPAddress, long, long, long>> results = TracerouteRecur(ipv6Address.ToString(),1);
				Console.WriteLine();
				foreach (var result in results)
				{
					Console.WriteLine(results.IndexOf(result).ToString() + "\t"
								+ (result.Item2 <= -1 ? "*\t" : result.Item2.ToString() + "ms\t")
								+ (result.Item2 <= -1 ? "*\t" : result.Item2.ToString() + "ms\t")
								+ (result.Item2 <= -1 ? "*\t" : result.Item2.ToString() + "ms\t")
								+ result.Item1.ToString());
				}
			}
			else
			{
				Console.WriteLine("IPv6 connection to host not found");
			}

			Console.ReadLine();
        }

        static List<Tuple<IPAddress, long, long, long>> TracerouteRecur(string dest, int TTL)
        {
			Console.Write(".");
            Ping ping = new Ping();
			byte[] buffer = Encoding.ASCII.GetBytes(data);
			PingReply reply = ping.Send(dest, timeout, buffer, new PingOptions(TTL, true));

			List<Tuple<IPAddress, long, long, long>> result = new List<Tuple<IPAddress, long, long, long>>();
			if (reply.Status == IPStatus.Success)
			{
				
				long[] RTTs = GenerateRTT(reply.Address);
				result.Add(Tuple.Create(reply.Address, RTTs[0], RTTs[1], RTTs[2]));
			}
			else if (reply.Status == IPStatus.TtlExpired || reply.Status == IPStatus.TimedOut)
			{
				if (reply.Status == IPStatus.TtlExpired)
				{
					long[] RTTs = GenerateRTT(reply.Address);
					result.Add(Tuple.Create(reply.Address, RTTs[0], RTTs[1], RTTs[2]));
				}

				List<Tuple<IPAddress, long, long, long>> tmpResult = TracerouteRecur(dest, TTL + 1);

				result.AddRange(tmpResult);
			}
			else
			{
				Console.WriteLine("Error");
			}			
			return result;
        }

		static long[] GenerateRTT(IPAddress dest)
		{
			long[] result = new long[3];

			for (int i = 0; i < 3; ++i)
			{
				Ping RTTping = new Ping();
				byte[] buffer = Encoding.ASCII.GetBytes(data);
				PingReply reply = RTTping.Send(dest, timeout, buffer);

				if (reply.Status == IPStatus.Success)
				{
					result[i] = reply.RoundtripTime;
				}
				else
				{
					result[i] = -1;
				}
			}

			return result;
		}
    }
}
