//////////////////////////////////////////////////////////////////////////////////
// Class1.cpp     : To handle doc files                                         //
// ver 1.0																		//																				//
// Application    : CSE-775 Project 1					                        //
// Platform       : Visual Studio 2017 Community Edition                        //
//                  Windows 10 Professional 64-bit, Acer Aspire R5-571TG        //
// Author         : Jaskaran Singh, EECS Department, Syracuse University        //
//                  315-925-9941, jsingh09@syr.edu							    //
//////////////////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This package is responsible for opening a doc file by creating an instance of
 * microsoft word in the background. It then reads line by line and searches for given
 * regex. If found, adds it to the List of integers.
 *
 * Public Interface:
 * -----------------
 * class1 obj = new class1();                      //Creates an instance of class1
 * obj.searchDoc(file,regularExp);                 //A function which reads a given file line by line for a given regular expression
 * 
 * Required File:
 * --------------
 * Class1.cs        
 *
 * Maintenance History:
 * --------------------
 * -April 7th, 2019
 *  first release
 */

using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using Microsoft.Office.Interop.Word;

namespace CsHelper
{
    //Class to handle doc files
    public class class1
    {
        //A function which reads a given doc file for a given regular expression
        public List<int> searchDoc(string file, string regularExp)
        {
            try
            {
                Application application = new Application();
                Document document = application.Documents.Open(file);
                Regex regex = new Regex(regularExp);
                List<int> data = new List<int>();
                int lineno = 1;
                for (int i = 0; i < document.Paragraphs.Count; i++)
                {
                    string temp = document.Paragraphs[i + 1].Range.Text.Trim();
                    if (temp != string.Empty)
                    {
                        if (regex.IsMatch(temp))
                            data.Add(lineno);
                    }
                    lineno++;
                }

                // Close word.
                document.Close();
                application.Quit();

                return data;

            }
            catch (COMException e)
            {
                Console.WriteLine(e);
                return new List<int>();
            }
        }
    };
}
