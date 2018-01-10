using System;
using System.IO;
using System.Diagnostics;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace ConsoleApplication
{
    public class Program
    {
        public static void Main(string[] args)
        {
            if (args.Length < 2) 
            {
                Console.WriteLine("Usage: du [-s] [-p] [-b] <path>");
                return;
            }
            var isSequential = false;
            var isParallel = false;
            foreach (var arg in args) 
            {
                if (arg == "-s" || arg == "-b") 
                {
                    isSequential = true;
                }
                if (arg == "-p" || arg == "-b") 
                {
                    isParallel = true;
                }
            }
            string root = args[args.Length - 1];

            Console.WriteLine("Directory '" + root + "':\n");
            if (isParallel)
            {
                ParallelCounter pCounter = new ParallelCounter(root);
                pCounter.Run();
            }
            if (isSequential) 
            {
                SequentialCounter sCounter = new SequentialCounter(root);
                sCounter.Run();
            }
        }
    }

    public class SequentialCounter
    {
        private string root;
        private long files;
        private long folders;
        private long size;

        public SequentialCounter(string filepath) 
        {
            this.root = filepath;
        }
        public void Run () 
        {
            Stopwatch watch = new Stopwatch();
            watch.Start();
            this.SequentialRecur(this.root);
            watch.Stop();
            Console.WriteLine("Sequential Calculated in : " + watch.Elapsed.TotalSeconds + "s");
            Console.WriteLine(this.folders + " folders, " + this.files + " files, " + this.size + " bytes\n");
        }

        public void SequentialRecur(string filepath) 
        {
            try  
            {
                var subdirectories = Directory.GetDirectories(filepath);
                var currentFiles = Directory.GetFiles(filepath);
                this.folders += subdirectories.Length;
                this.files += currentFiles.Length;
                foreach (string path in currentFiles) {
                    try 
                    {
                        this.size += new FileInfo(path).Length;
                    }
                    catch (Exception e) 
                    {
                        Console.WriteLine("An error occured while reading a file size: " + e.Message);
                    }
                }

                foreach (string path in subdirectories) {
                    SequentialRecur(path);
                }
            }
            catch (Exception e) 
            {
                Console.WriteLine("An error occured while getting subdirectory information: " + e.Message);
            }
        }
    }

    public class sharedNumbers
    {
        private long folders;
        private long files;
        private long size;


        public sharedNumbers()
        {
            this.folders = 0;
            this.files = 0;
            this.size = 0;
        }
        public void IncreaseFolders(long increment)
        {   
            lock(this) 
            {
                this.folders += increment;
            }
        }
        
        public void IncreaseFiles(long increment)
        {   
            lock(this) 
            {
                this.files += increment;
            }
        }
        
        public void IncreaseSize(long increment)
        {   
            lock(this) 
            {
                this.size += increment;
            }
        }

        public long Folders {
            get {
                return folders;
            }
        }
        public long Files {
            get {
                return files;
            }
        }
        public long Size {
            get {
                return size;
            }
        }
    }


    public class ParallelCounter
    {
        private string root;
        private sharedNumbers nums;

        public ParallelCounter(string filepath)
        {
            this.root = filepath;
            nums = new sharedNumbers();
        }

        public void Run()
        {
            Stopwatch watch = new Stopwatch();
            watch.Start();
            this.ParallelRecur(this.root);
            watch.Stop();
            Console.WriteLine("Parallel Calculated in : " + watch.Elapsed.TotalSeconds + "s");
            Console.WriteLine(nums.Folders + " folders, " + nums.Files + " files, " + nums.Size + " bytes\n");
        }

        public void ParallelRecur(string filepath) 
        {
            try 
            {
                var subdirectories = Directory.GetDirectories(filepath);
                var currentFiles = Directory.GetFiles(filepath);
                nums.IncreaseFolders(subdirectories.Length);
                nums.IncreaseFiles(currentFiles.Length);
                
                foreach (string path in currentFiles) {
                    try 
                    {   
                        nums.IncreaseSize(new FileInfo(path).Length);
                    }
                    catch (Exception e)
                    {
                        Console.WriteLine("An error occured while reading a file size: " + e.Message);
                    }
                }

                Parallel.ForEach(subdirectories, path=>{ParallelRecur(path);});
            }
            catch (Exception e) 
            {
                Console.WriteLine("An error occured while getting subdirectory information: " + e.Message);
            }
        }
    }
}
