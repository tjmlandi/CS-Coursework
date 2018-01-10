// <author>Jeremy Brown</author>
// <date>18-Feb-2018</date>
// <summary>Starter Code for Copads Project 2 (2165)</summary>

using CommandLine;

namespace Project2
{

    /// <summary>
    /// Enum for the types of sorting we can do, used for TestOptions
    /// </summary>
    internal enum TestMethods
    {
        seq,
        par,
        both
    }


    /// <summary>
    /// Class for handling test options
    /// </summary>
    [Verb("testwith")]
    internal class TestOptions
    {
        [Option('t')]
        public TestMethods type { get; set; }
        [Value(0)]
        public string TestCount { get; set; }


    }

    /// <summary>
    /// Class for handling command line arguments for the file sorting
    /// </summary>
    internal class Options
    {
        [Option('g', "")]
        public bool parseint { get; set; }

        [Value(0, Required = true, MetaName = "filename", HelpText = "The name of the file whose contents you want to sort")]
        public string FileName { get; set; }
    }

}
