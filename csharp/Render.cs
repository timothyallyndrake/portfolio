using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Collections;

namespace Document_Publisher.Classes.Document
{
    class Render
    {
        [DllImport("gsdll32.dll", EntryPoint = "gsapi_new_instance")]
        private static extern int gsapi_new_instance(out IntPtr pinstance, IntPtr caller_handle);

        [DllImport("gsdll32.dll", EntryPoint = "gsapi_init_with_args")]
        private static extern int gsapi_init_with_args(IntPtr instance, int argc, IntPtr argv);

        [DllImport("gsdll32.dll", EntryPoint = "gsapi_exit")]
        private static extern int gsapi_exit(IntPtr instance);

        [DllImport("gsdll32.dll", EntryPoint = "gsapi_delete_instance")]
        private static extern void gsapi_delete_instance(IntPtr instance);

        private string _sDeviceFormat;

        private int _iWidth;
        private int _iHeight;
        private int _iResolutionX;
        private int _iResolutionY;
        private int _iJPEGQuality;

        private Boolean _bFitPage;

        private IntPtr _objHandle;

        public string OutputFormat
        {
            get { return _sDeviceFormat; }
            set { _sDeviceFormat = value; }
        }

        public int Width
        {
            get { return _iWidth; }
            set { _iWidth = value; }
        }

        public int Height
        {
            get { return _iHeight; }
            set { _iHeight = value; }
        }

        public int ResolutionX
        {
            get { return _iResolutionX; }
            set { _iResolutionX = value; }
        }

        public int ResolutionY
        {
            get { return _iResolutionY; }
            set { _iResolutionY = value; }
        }

        public Boolean FitPage
        {
            get { return _bFitPage; }
            set { _bFitPage = value; }
        }

        public int JPEGQuality
        {
            get { return _iJPEGQuality; }
            set { _iJPEGQuality = value; }
        }

        public Render(IntPtr objHandle)
        { _objHandle = objHandle; }

        public Render()
        { _objHandle = IntPtr.Zero; }

        private byte[] StringToAnsiZ(string str)
        {
            int intElementCount;
            int intCounter;
            byte[] aAnsi;
            byte bChar;

            intElementCount = str.Length;
            aAnsi = new byte[intElementCount + 1];

            for (intCounter = 0; intCounter < intElementCount; intCounter++)
            {
                bChar = (byte)str[intCounter];
                aAnsi[intCounter] = bChar;
            }

            aAnsi[intElementCount] = 0;

            return aAnsi;
        }

        public void Fullsize(string inputFile, string outputFile)
        {
            _iJPEGQuality = 100;
            _sDeviceFormat = "jpeg";
            _iResolutionX = 110;
            _iResolutionY = 110;

            _Render(inputFile, outputFile);
        }

        public void Thumbnail(string inputFile, string outputFile)
        {
            _iJPEGQuality = 100;
            _sDeviceFormat = "jpeg";
            _iResolutionX = 8;
            _iResolutionY = 8;

            if (Classes.Document.Information.PageCount(inputFile) != 1)
            {
                _iResolutionX = 24;
                _iResolutionY = 24;
            }

            _Render(inputFile, outputFile);
        }

        private void _Render(string inputFile, string outputFile)
        {
            if (!System.IO.File.Exists(inputFile))
            { return; }

            int intReturn;
            IntPtr intGSInstanceHandle;
            object[] aAnsiArgs;
            IntPtr[] aPtrArgs;
            GCHandle[] aGCHandle;
            int intCounter;
            int intElementCount;
            IntPtr callerHandle;
            GCHandle gchandleArgs;
            IntPtr intptrArgs;

            string[] sArgs = GetGeneratedArgs(inputFile, outputFile);

            intElementCount = sArgs.Length;
            aAnsiArgs = new object[intElementCount];
            aPtrArgs = new IntPtr[intElementCount];
            aGCHandle = new GCHandle[intElementCount];

            for (intCounter = 0; intCounter < intElementCount; intCounter++)
            {
                aAnsiArgs[intCounter] = StringToAnsiZ(sArgs[intCounter]);
                aGCHandle[intCounter] = GCHandle.Alloc(aAnsiArgs[intCounter], GCHandleType.Pinned);
                aPtrArgs[intCounter] = aGCHandle[intCounter].AddrOfPinnedObject();
            }

            gchandleArgs = GCHandle.Alloc(aPtrArgs, GCHandleType.Pinned);
            intptrArgs = gchandleArgs.AddrOfPinnedObject();

            intReturn = gsapi_new_instance(out intGSInstanceHandle, _objHandle);
            callerHandle = IntPtr.Zero;

            try
            { intReturn = gsapi_init_with_args(intGSInstanceHandle, intElementCount, intptrArgs); }
            finally
            {
                for (intCounter = 0; intCounter < intReturn; intCounter++)
                { aGCHandle[intCounter].Free(); }

                gchandleArgs.Free();
                gsapi_exit(intGSInstanceHandle);
                gsapi_delete_instance(intGSInstanceHandle);
            }
        }

        private string[] GetGeneratedArgs(string inputFile, string outputFile)
        {
            ArrayList lstExtraArgs = new ArrayList();

            if (_sDeviceFormat == "jpeg" && _iJPEGQuality > 0 && _iJPEGQuality < 101)
            { lstExtraArgs.Add("-dJPEGQ=" + _iJPEGQuality); }

            if (_iWidth > 0 && _iHeight > 0)
            { lstExtraArgs.Add("-g" + _iWidth + "x" + _iHeight); }

            if (_bFitPage)
            { lstExtraArgs.Add("-dPDFFitPage"); }

            if (_iResolutionX > 0)
            {
                if (_iResolutionY > 0)
                { lstExtraArgs.Add("-r" + _iResolutionX + "x" + _iResolutionY); }
                else
                { lstExtraArgs.Add("-r" + _iResolutionX); }
            }

            lstExtraArgs.Add("-dQUIET");
            lstExtraArgs.Add("-dNOPROMPT");
            lstExtraArgs.Add("-dMaxBitmap=500000000");
            lstExtraArgs.Add("-dNumRenderingThreads=4");
            lstExtraArgs.Add("-dAlignToPixels=0");
            lstExtraArgs.Add("-dGridFitTT=0");
            lstExtraArgs.Add("-dTextAlphaBits=4");
            lstExtraArgs.Add("-dGraphicsAlphaBits=4");
            lstExtraArgs.Add("-dFirstPage=1");
            lstExtraArgs.Add("-dLastPage=1");

            int iFixedCount = 7;
            int iExtraArgsCount = lstExtraArgs.Count;
            string[] args = new string[iFixedCount + lstExtraArgs.Count];

            args[0] = "pdf2img";
            args[1] = "-dNOPAUSE";
            args[2] = "-dBATCH";
            args[3] = "-dSAFER";
            args[4] = "-sDEVICE=" + _sDeviceFormat;

            for (int i = 0; i < iExtraArgsCount; i++)
            { args[5 + i] = (string)lstExtraArgs[i]; }

            args[5 + iExtraArgsCount] = string.Format("-sOutputFile={0}", outputFile);
            args[6 + iExtraArgsCount] = string.Format("{0}", inputFile);

            return args;
        }
    }
}