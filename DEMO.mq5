//+------------------------------------------------------------------+
//|                                                        DEMO.mq5  |
//|                                  Copyright 2023, MetaQuotes Ltd. |
//|                                             https://www.mql5.com |
//+------------------------------------------------------------------+

// Win32 APIもこのヘッダーファイルでinclude
#include "common_mqh_cpp_PCH.mqh"


//+------------------------------------------------------------------+
//| Expert initialization function                                   |
//+------------------------------------------------------------------+
int OnInit()
  {
// ターミナルデータのディレクトリを取得
   string Path=TerminalInfoString(TERMINAL_DATA_PATH);
// txtのフルパスを作成
   string File_Path=Path+"\\demo_file.txt";
// txtの中身を定義
   string Text_Value="demonstration";

// 文字列型を書き込むクラス、関数は作成していないので直接WIN32 APIにアクセス
   HANDLE File_Handle=CreateFileW(

                         File_Path,
                         GENERIC_READ | GENERIC_WRITE,
                         CPP_MQH_FILE_SHARE_READ | CPP_MQH_FILE_SHARE_WRITE,
                         NULL,
                         OPEN_ALWAYS,
                         FILE_ATTRIBUTE_NORMAL,
                         NULL);

// MQL5のCreateFileWの書込みバッファの引数定義はushort[]なので配列に変換
// 戻り値は割愛
   ushort Text_Array[];
   StringToShortArray(Text_Value,Text_Array,0,WHOLE_ARRAY);

   uint Write_Byte_Size=0;

// 今回はデモ用なのでOVERLAPPEDは初期化で対応
   OVERLAPPED Ov_L = {};

// 戻り値は割愛
   WriteFile(File_Handle,Text_Array,ArraySize(Text_Array)*sizeof(ushort),Write_Byte_Size,Ov_L);

   CloseHandle(File_Handle);

// MQL5 C++どちらでも動作する動的配列クラス
   _vector<string> Root;

// メモ帳のexeファイルのあるディレクトリを\\ごとに配列に格納
// C:\Windows\System32\notepad.exeの場合は

   /*Root.push_back("C:\\Windows");
   Root.push_back("\\System32");*/

   Root.push_back("C:\\Windows");


   PROCESS_MANAGER Process_Mng;
   string Exe_Name, Command_Line;

// メモ帳のフルパスを取得
   Process_Mng.Get_File_Drc(Root, Root.size(), "\\notepad.exe", Exe_Name);

// メモ帳のexeファイルが実際に存在するか判定
   ulong Error_Code = Process_Mng.Proccess_Boot_File_Exist(Exe_Name);

   if(Error_Code != ERROR_SUCCESS)
      return INIT_FAILED;

   uint32_t Frag = DETACHED_PROCESS | HIGH_PRIORITY_CLASS;

// コマンドラインを定義してメモ帳が開くファイルを指定
   Command_Line = "\"app.exe\" \"" + File_Path + "\"";

// メモ帳を実行
   Error_Code = Process_Mng.Create_Proccess(

                   Exe_Name,
                   Command_Line,
                   Frag,
                   Path);

//---
   return(INIT_SUCCEEDED);
  }
//+------------------------------------------------------------------+
//| Expert deinitialization function                                 |
//+------------------------------------------------------------------+
void OnDeinit(const int reason)
  {
//--- destroy timer
   EventKillTimer();

  }
//+------------------------------------------------------------------+
//| Expert tick function                                             |
//+------------------------------------------------------------------+
void OnTick()
  {
//---

  }
//+------------------------------------------------------------------+
//| Timer function                                                   |
//+------------------------------------------------------------------+
void OnTimer()
  {
//---

  }
//+------------------------------------------------------------------+
