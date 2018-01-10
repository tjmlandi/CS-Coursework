// <author>Jeremy Brown</author>
// <date>18-Feb-2018</date>
// <summary>Starter Code for Copads Project 2 (2165)</summary>


using System;
using System.IO;
using CommandLine;
using System.Collections.Generic;
using System.Threading.Tasks;
using System.Linq;
using System.Diagnostics;


namespace Project2
{ 
    class Project2
    {
        /// <summary>
        /// The main entry point into the program
        /// </summary>
        /// <param name="args">Command line arguments</param>
        public static void Main(string[] args)
        {
            var parser = new Parser(with => with.HelpWriter = TextWriter.Null);
            parser.ParseArguments<TestOptions, Options>(args)
                .WithParsed(opts =>
                {
                    var testParams = (TestOptions) opts;
                    Console.WriteLine("Detected " + Environment.ProcessorCount + " Processors");
                    // run your tests

                    //Randy is always ready to supply numbers
                    Random randy = new Random();

                    if (testParams.type == TestMethods.both) 
                    {
                        //Create two lists
                        List<int> seqValues = new List<int>();
                        List<int> parValues = new List<int>();

                        //Get the specified size of the lists
                        int count = Int32.Parse(testParams.TestCount);

                        //Get some help from Randy to fill those lists
                        for (int i = 0; i < count; ++i)
                        {
                            var j = randy.Next();

                            seqValues.Add(j);
                            parValues.Add(j);
                        } 

                        //Run and time the sequential quicksort
                        Stopwatch watchSeq = new Stopwatch();
                        watchSeq.Start();
                        seqValues.QuickSortSequential();
                        watchSeq.Stop();
                        var seqTime = watchSeq.Elapsed;

                        //Run and time the parallel quicksort
                        Stopwatch watchPar = new Stopwatch();
                        watchPar.Start();
                        parValues.QuickSortParallel();
                        watchPar.Stop();
                        var parTime = watchPar.Elapsed;

                        //Display results
                        Console.WriteLine("Sequential");
                        Console.WriteLine(seqTime);
                        Console.WriteLine("Parallel");
                        Console.WriteLine(parTime);
                    }
                    //Next two cases go through the same steps, just only running Parallel or Sequential
                    else if (testParams.type == TestMethods.par)
                    {
                        List<int> parValues = new List<int>();

                        int count = Int32.Parse(testParams.TestCount);

                        for (int i = 0; i < count; ++i)
                        {
                            var j = randy.Next();
                            parValues.Add(j);
                        } 

                        Stopwatch watchPar = new Stopwatch();
                        watchPar.Start();
                        parValues.QuickSortParallel();
                        watchPar.Stop();
                        var parTime = watchPar.Elapsed;

                        Console.WriteLine("Parallel");
                        Console.WriteLine(parTime);
                    }
                    else 
                    {               
                        List<int> seqValues = new List<int>();

                        int count = Int32.Parse(testParams.TestCount);

                        for (int i = 0; i < count; ++i)
                        {
                            var j = randy.Next();

                            seqValues.Add(j);
                        } 

                        Stopwatch watchSeq = new Stopwatch();
                        watchSeq.Start();
                        seqValues.QuickSortSequential();
                        watchSeq.Stop();
                        var seqTime = watchSeq.Elapsed;


                        Console.WriteLine("Sequential");
                        Console.WriteLine(seqTime);
                    }

                })
                .WithNotParsed(err =>
                {
                    parser.ParseArguments<Options>(args)
                        .WithParsed(opts => { 
                            //Read the text file into an array (assuming that the items in the file are deliniated by newlines)
                            string[] text = File.ReadAllLines(opts.FileName);
                            if (opts.parseint) 
                            {
                                //Parse array into int list and sort
                                List<int> values = new List<int>(text.Select(int.Parse));
                                values.QuickSortParallel();
                                //Print out values
                                for (var i = 0; i < values.Count; ++i) 
                                {
                                    Console.WriteLine(values[i]);
                                }
                            }
                            else 
                            {
                                //Move array into string list and sort
                                List<string> values = new List<string>(text);
                                values.QuickSortParallel();
                                //Print out values
                                for (var i = 0; i < values.Count; ++i) 
                                {
                                    Console.WriteLine(values[i]);
                                }
                            }
                        })
                        .WithNotParsed(errs =>
                        {
                            // show the help
                            Console.WriteLine("Project2 [<command>|filename]");
                            Console.WriteLine();
                            Console.WriteLine("Commands are listed below:");
                            Console.WriteLine();
                            Console.WriteLine("testwith -t <type> <count>    Runs a test scenario for quicksort");
                            Console.WriteLine("                              type can be:");
                            Console.WriteLine("                                 seq - sequential quicksort");
                            Console.WriteLine("                                 par - parallel quicksort");
                            Console.WriteLine("                                 both - both sequential and parallel");
                            Console.WriteLine();
                            Console.WriteLine("when no command is given, you can specify a filename, and the ");
                            Console.WriteLine("optional -g.  -g will force the data to be integer types");
                            Console.WriteLine();
                            Console.WriteLine("Example:");
                            Console.WriteLine("Project2 -g test.txt");
                            Console.WriteLine("Project2 testwith -t par 10000");
                            Console.WriteLine("Project2 test.txt");
                        });
                });
        }
    }

    public static class QuickSort
    {
        //Extension method, simply starts the parallel recursion
        public static void QuickSortParallel<T>(this List<T> lst) where T : IComparable
        {
            QSPRecur<T>(lst, 0, lst.Count - 1, Environment.ProcessorCount + 1);
        }

        //Parallel Recursive method. Takes in the list, the left and right bounds it will be working with
        //and the number of logical processors left, so it knows how many threads it can spawn (depth restriction)
        public static void QSPRecur<T>(List<T> lst, int leftIndex, int rightIndex, int numProcessors) where T : IComparable
        {            
            numProcessors -= 1;
            //Handles cases where we are not yet at the bottom
            if (rightIndex - leftIndex > 2)
            {
                //Pick a pivot based on the 'median of three' method
                T first;
                T last;
                T middle;
                first = lst[leftIndex];
                last = lst[rightIndex];
                middle = lst[(rightIndex - leftIndex) / 2];

                //Compare the first, middle, and last values, and pick the median to be the pivot
                int pivot;
                if (first.CompareTo(middle) < 0 && first.CompareTo(last) > 0)
                    pivot = leftIndex;
                else if (middle.CompareTo(first) < 0 && middle.CompareTo(last) > 0) 
                    pivot = (rightIndex - leftIndex) / 2;
                else 
                    pivot = rightIndex;
                
                //Move the pivot to the beginning of the allocated space
                var tmp = lst[pivot];
                lst[pivot] = lst[leftIndex];
                lst[leftIndex] = tmp;
                
                //Setup the marker
                var marker = leftIndex;

                //Loop through from the marker + 1 till the righ boundary,
                //making sure to increment the marker and swap in case we find 
                //something lower than the pivot
                for (var i = leftIndex + 1; i <= rightIndex; ++i)
                {    
                    if (lst[i].CompareTo(lst[leftIndex]) < 0) 
                    {
                        ++marker;
                        var swap = lst[i];
                        lst[i] = lst[marker];
                        lst[marker] = swap;
                    }
                }

                //Swap the pivot with the marker
                tmp = lst[leftIndex];
                lst[leftIndex] = lst[marker];
                lst[marker] = tmp;

                
                //If we have no processors left, start doing it sequentially
                if (numProcessors <= 0)
                {
                    QSSRecur(lst, leftIndex, marker - 1);
                    QSSRecur(lst, marker + 1, rightIndex);
                }
                //If just one, do one of each
                else if (numProcessors == 1)
                {
                    Task left = new Task(()=>QSPRecur<T>(lst, leftIndex, marker - 1, 0));
                    left.Start();
                    QSSRecur<T>(lst, marker + 1, rightIndex);
                    left.Wait();
                }
                //Else, spawn two tasks (each case is just regarding the possibility of having an odd number of processors, and integer math)
                else if (numProcessors % 2 == 0)
                {
                    Task left = new Task(()=>{QSPRecur<T>(lst, leftIndex, marker - 1, numProcessors / 2);});
                    Task right = new Task(()=>{QSPRecur<T>(lst, marker + 1, rightIndex, numProcessors / 2);});
                    left.Start();
                    right.Start();
                    left.Wait();
                    right.Wait();
                }
                else
                {
                    Task left = new Task(()=>{QSPRecur<T>(lst, leftIndex, marker - 1, (numProcessors / 2) + 1);});
                    Task right = new Task(()=>{QSPRecur<T>(lst, marker + 1, rightIndex, numProcessors / 2);});   


                    left.Start();
                    right.Start();
                    left.Wait();
                    right.Wait();
                }
            }
            //Hand the case when there are three items in the partition, as we can just bubble sort that
            else if (rightIndex - leftIndex == 2)
            {
                if (lst[leftIndex].CompareTo(lst[leftIndex + 1]) > 0)
                {
                    var swap = lst[leftIndex + 1];
                    lst[leftIndex + 1] = lst[leftIndex];
                    lst[leftIndex] = swap;
                }
                if (lst[leftIndex + 1].CompareTo(lst[rightIndex]) > 0)
                {
                    var swap = lst[leftIndex + 1];
                    lst[leftIndex + 1] = lst[rightIndex];
                    lst[rightIndex] = swap;
                }
                if (lst[leftIndex].CompareTo(lst[leftIndex + 1]) > 0)
                {
                    var swap = lst[leftIndex + 1];
                    lst[leftIndex + 1] = lst[leftIndex];
                    lst[leftIndex] = swap;
                }
            }
            //If there are two left, just make sure they are in order
            else if (rightIndex - leftIndex == 1)
            {
                if (lst[rightIndex].CompareTo(lst[leftIndex]) < 0) 
                {
                    var swap = lst[rightIndex];
                    lst[rightIndex] = lst[leftIndex];
                    lst[leftIndex] = swap;
                }
            }
            //No need to handle the case of a single item, as it is sorted
        }

        //Sequentially recursive function. Works identically to the parallel, with the exception that it
        //will not create tasks for the first n=numProcessors number of recursions
        public static void QSSRecur<T>(List<T> lst, int leftIndex, int rightIndex) where T : IComparable
        {            
            if (rightIndex - leftIndex > 2)
            {
                T first;
                T last;
                T middle;
                first = lst[leftIndex];
                last = lst[rightIndex];
                middle = lst[(rightIndex - leftIndex) / 2];
                int pivot;
                if (first.CompareTo(middle) < 0 && first.CompareTo(last) > 0)
                    pivot = leftIndex;
                else if (middle.CompareTo(first) < 0 && middle.CompareTo(last) > 0) 
                    pivot = (rightIndex - leftIndex) / 2;
                else 
                    pivot = rightIndex;
                
            
                var tmp = lst[pivot];
                lst[pivot] = lst[leftIndex];
                lst[leftIndex] = tmp;

                var marker = leftIndex;

                for (var i = leftIndex + 1; i <= rightIndex; ++i)
                {    
                    if (lst[i].CompareTo(lst[leftIndex]) < 0) 
                    {
                        ++marker;
                        var swap = lst[i];
                        lst[i] = lst[marker];
                        lst[marker] = swap;
                    }
                }

                tmp = lst[leftIndex];
                lst[leftIndex] = lst[marker];
                lst[marker] = tmp;

                
                QSSRecur<T>(lst, leftIndex, marker - 1);
                QSSRecur<T>(lst, marker + 1, rightIndex);
            }
            else if (rightIndex - leftIndex == 2)
            {
                if (lst[leftIndex].CompareTo(lst[leftIndex + 1]) > 0)
                {
                    var swap = lst[leftIndex + 1];
                    lst[leftIndex + 1] = lst[leftIndex];
                    lst[leftIndex] = swap;
                }
                if (lst[leftIndex + 1].CompareTo(lst[rightIndex]) > 0)
                {
                    var swap = lst[leftIndex + 1];
                    lst[leftIndex + 1] = lst[rightIndex];
                    lst[rightIndex] = swap;
                }
                if (lst[leftIndex].CompareTo(lst[leftIndex + 1]) > 0)
                {
                    var swap = lst[leftIndex + 1];
                    lst[leftIndex + 1] = lst[leftIndex];
                    lst[leftIndex] = swap;
                }
            }
            else if (rightIndex - leftIndex == 1)
            {
                if (lst[rightIndex].CompareTo(lst[leftIndex]) < 0) 
                {
                    var swap = lst[rightIndex];
                    lst[rightIndex] = lst[leftIndex];
                    lst[leftIndex] = swap;
                }
            }
        }

        //Sequential Quicksort Extension method, which just kicks of the sequential recursion
        public static void QuickSortSequential<T>(this List<T> lst) where T : IComparable
        {
            /* TEST BLOCK
            for (var i = 0; i < Environment.ProcessorCount; ++i)
            {
                var task = new Task(()=>{});
                task.Start();
                task.Wait();
            } 
            */  
            QSSRecur(lst, 0, lst.Count - 1);
        }
    }
}