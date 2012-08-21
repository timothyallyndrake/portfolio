using System;
using System.IO;
using System.Diagnostics;
using System.Windows.Forms;

namespace Document_Publisher.Classes.Document
{
    public class Information
    {
        public static int PageCount(string file)
        {
            return int.Parse(_Process(file, "Pages", "0"));
        }

        private static string _Process(string file, string name, string defaultValue)
        {
            FileInfo fileInfo = new FileInfo(file);

            if (!fileInfo.Exists)
            { return defaultValue; }

            Process process = new Process();

            process.EnableRaisingEvents = true;
            process.StartInfo.Arguments = '"' + fileInfo.FullName + '"';
            process.StartInfo.CreateNoWindow = true;
            process.StartInfo.ErrorDialog = false;
            process.StartInfo.FileName = Directory.GetParent(Application.ExecutablePath).FullName + "\\pdfinfo.exe";
            process.StartInfo.RedirectStandardOutput = true;
            process.StartInfo.UseShellExecute = false;

            process.Start();

            string standardOutputRaw = process.StandardOutput.ReadToEnd();

            process.WaitForExit();
            process.Dispose();

            string[] standardOutputLine = standardOutputRaw.Split(Environment.NewLine.ToCharArray());

            int index; string value = defaultValue;

            for (index = 0; index < standardOutputLine.Length; index++)
            {
                if (standardOutputLine[index].Contains(name))
                {
                    string[] standardOutputLinePart = standardOutputLine[index].Split(':');
                    value = standardOutputLinePart[1].Trim();
                }
            }

            return value;
        }
    }
}
