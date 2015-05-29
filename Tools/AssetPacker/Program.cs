using Sbatman.Serialize;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace AssetPacker
{
    internal class Program
    {
        private const string INPUTFOLDER = "Input";
        private const string OUTPUTFILE = "Dat.Pack";
        private const UInt16 PACKETID_PACK = 1412;
        private const UInt16 PACKETID_PNG = 1247;

        private static void Main(string[] args)
        {
            if (!Directory.Exists(INPUTFOLDER)) { Console.WriteLine("Input Folder Missing"); return; }
            if (File.Exists(OUTPUTFILE)) File.Delete(OUTPUTFILE);

            List<string> DetectedFiles = new List<string>();

            DetectFiles(DetectedFiles, INPUTFOLDER);

            FilterFiles(ref DetectedFiles);

            List<Packet> ProcessedFiles = new List<Packet>();

            foreach (string s in DetectedFiles)
            {
                if (s.ToLowerInvariant().EndsWith(".png")) ProcessedFiles.Add(ProcessPNG(s));
            }

            Packet Pack = new Packet(PACKETID_PACK);
            foreach (Packet p in ProcessedFiles)
            {
                Pack.Add(p.ToByteArray());
                p.Dispose();
            }

            ProcessedFiles.Clear();

            using (FileStream outStream = File.Create(OUTPUTFILE))
            {
                byte[] packData = Pack.ToByteArray();
                Pack.Dispose();
                outStream.Write(packData, 0, packData.Length);
            }
        }

        private static void FilterFiles(ref List<string> detectedFiles)
        {
            detectedFiles = detectedFiles.Where(s => s.ToLowerInvariant().EndsWith(".png")).ToList();
        }

        private static void DetectFiles(List<string> detectedFiles, string folder)
        {
            detectedFiles.AddRange(Directory.EnumerateFiles(folder));
            foreach (string s in Directory.EnumerateDirectories(folder)) DetectFiles(detectedFiles, s);
        }

        private static Tuple<Int32, Int32, byte[]> LoadPng(string fileName)
        {
            using (Bitmap bmp = new Bitmap(fileName))
            {
                int bytes = bmp.Width * bmp.Height * 4;
                byte[] returnData = new byte[bytes];
                BitmapData dat = bmp.LockBits(new Rectangle(0, 0, bmp.Width, bmp.Height), ImageLockMode.ReadOnly, PixelFormat.Format32bppArgb);
                IntPtr ptr = dat.Scan0;
                Marshal.Copy(ptr, returnData, 0, bytes);
                return new Tuple<int, int, byte[]>(bmp.Width, bmp.Height, returnData);
            }
        }

        private static Packet ProcessPNG(string fileName)
        {
            Tuple<int, int, byte[]> data = LoadPng(fileName);
            Packet p = new Packet(PACKETID_PNG);
            p.Add(fileName);
            p.Add(data.Item1);
            p.Add(data.Item2);
            p.Add(data.Item3);
            return p;
        }
    }
}