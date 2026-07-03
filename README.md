Markdown

# C++ WIN32 / MQL5 Cross-Environment Bridge

C++ (Win32 API) および MQL5 (MetaTrader 5) の両環境において、同一のヘッダーソースを共有してコンパイルを行うためのクロス環境ブリッジ（仲介コンポーネント）です。

MQL5環境における標準ファイル関数の制約を Win32 API の直接操作によって代替し、EAの実行スレッドと同期・非同期で動作する外部プロセスとの連携、ファイル排他制御、プロセス制御、およびエラーハンドリングを統一します。

## 主な機能 (Key Features)

- **デュアル環境・単一ソース:**
  `__CPP__` および `__MQL5__` マクロの切り替えにより、C++とMQL5で同一のデータ構造（`_vector<T>` や `CPP_TYPE_STRING`）を共有。
- **Win32 API 直接操作:**
  `_ReadFile`, `_WriteFile`, `_LockFileEx`, `_CreateProcessW` 等の呼出による、OSレベルでのファイル制御およびプロセス管理。
- **64-bit ビットマップ・シリアライズ:**
  `ulong` データを `pack(1)` 構造体および共用体・ビットキャストを用いてメモリ上のバイナリ配列（`ushort[]`）としてファイルへ出力。
- **ロガーのタイムスタンプ監査:**
  プロセスの起動時刻と共有ファイルの最終更新時刻を比較し、時系列の整合性を検証。
- **ログ自動容量管理:**
  指定されたディレクトリの総容量をクローリングし、400MB（419,430,400 バイト）を超過した際に古いファイルから順に削除を実行。
- **ミリ秒単位の実行精度（低遅延非対応）:**
  マイクロ秒未満の低遅延（Low-Latency）設計ではなく、ミリ秒（ms）単位のウェイト（Sleep）および排他ロック（Try-Lock）制御に基づき、確実なデータ同期を優先して実行。

## コンポーネント構成 (Component Matrix)

| ファイル名 | 役割 |
| :--- | :--- |
| `common_mqh_cpp_PCH.h` | C++標準環境およびMQL5 WinAPI環境のインクルードツリーの統括。 |
| `common_mqh_cpp_define.h` | 各環境における型定義（`BOOL`, `DWORD`, `__int64`）および文字列定義の差異の吸収（`_u`）。 |
| `common_mqh_cpp_types.h` | 文字列クラス`CPP_TYPE_STRING`、動的配列`_vector<T>`等の基本データ型の定義。 |
| `common_mqh_cpp_constants.h` | Win32カーネル定数（`GENERIC_READ`, `LOCKFILE_EXCLUSIVE_LOCK` 等）の定数定義。 |
| `common_mqh_cpp_enums.h` | システムエラーコード（56段階）および言語番号の定義。 |
| `common_mqh_cpp_method.h` | Win32 API関数のラッパーおよびオーバーロード関数の定義。 |
| `common_mqh_cpp_class_error.h` | システムエラーの保持・管理。 |
| `common_mqh_cpp_lock.h` | `_LockFileEx` / `_UnlockFileEx` を用いたファイル排他ロック制御。 |
| `common_mqh_cpp_class_file_base.h` | フォルダ創生、ファイルバッファのフラッシュ等のファイル操作基底クラス。 |
| `common_mqh_cpp_class_byte8.h` | 8バイト（64ビット）データ専用のファイルI/OおよびAppDataへのパス解決の執行。 |
| `common_mqh_cpp_class_file_flow.h` | ファイルオープン、ロック試行、およびログ用外部プロセスの起動。 |
| `common_mqh_cpp_class_log_base.h` | ログファイルのフォーマット生成、書き込み、および容量メンテナンスの執行。 |
| `common_mqh_cpp_class_language.h` | ターミナル言語（日・英・その他）の識別管理。 |
| `common_mqh_cpp_class_pipe.h` | プロセス間通信（IPC）用制御（拡張用）。 |
| `common_mqh_cpp_class_process.h` | ミューテックスによる重複起動防止、およびコマンドライン引数の生成・パース。 |
| `common_mqh_cpp_secrets.h` | 各種ミューテックス名や固定フォルダパスなどの定義（ダミー代替）。 |

## ビルド・導入方法 (Usage)

### 1. 配置およびインクルード
全てのファイルを、利用するソースコードと同じディレクトリに配置し、環境に応じて以下のようにインクルードを記述してください。
- C++環境: `#include "common_mqh_cpp_PCH.h"`
- MQL5環境: ファイルの拡張子を `.mqh` に変更した上で `#include "common_mqh_cpp_PCH.mqh"` と記述。

### 2. secrets.h の本番環境への適合
`common_mqh_cpp_secrets.h` はリポジトリ公開用のダミーファイルです。実際の運用環境に適合させる際は、自身のローカルパスやミューテックス名に書き換えてからコンパイルを実行してください。

## ライセンスと免責事項 (License & Disclaimer)

本プロジェクトは **Apache License 2.0** のもとで公開されています。
本プロジェクトを使用したロガー、及びエラー表示プロセスも同条件で公開しています。

### 免責事項 (AS IS)
本ソフトウェアは「現状有姿（AS IS）」で提供され、明示的・黙示的を問わず、いかなる保証も行いません。
本ライブラリまたはそれを組み込んだプログラムの実行によって発生した、いかなる直接的・間接的・商業的な損害に対しても、著作権者は一切の法的な責任を負いません。すべてのリスクは利用者が負うものとします。


# C++ WIN32 / MQL5 Cross-Environment Bridge

A cross-environment bridge (mediating component) designed to share the same header source and compile in both C++ (Win32 API) and MQL5 (MetaTrader 5) environments.

It replaces the limitations of standard file functions in the MQL5 environment through direct operation of the Win32 API, unifying coordination with external processes operating synchronously or asynchronously with the EA's execution thread, file exclusive control, process control, and error handling.

## Key Features

- **Dual-Environment Single-Source:** Shares the same data structures (`_vector<T>` and `CPP_TYPE_STRING`) between C++ and MQL5 by switching the `__CPP__` and `__MQL5__` macros.
- **Direct Win32 API Operations:** Manages file control and process management at the OS level by calling functions such as `_ReadFile`, `_WriteFile`, `_LockFileEx`, and `_CreateProcessW`.
- **64-bit Bitmap Serialization:** Outputs `ulong` data to a file as a binary array (`ushort[]`) in memory using a `pack(1)` structure, union, and bit casting.
- **Logger Timestamp Audit:** Compares the process boot time with the last write time of the shared file to verify chronological consistency.
- **Automatic Log Capacity Management:** Crawls the total capacity of the specified directory and sequentially deletes old files when it exceeds 400MB (419,430,400 bytes).
- **Millisecond-Level Execution (Not Low-Latency):** Not designed for sub-microsecond low-latency. It operates based on millisecond (ms) level wait (Sleep) and exclusive lock (Try-Lock) controls, prioritizing reliable data synchronization.

## Component Matrix

| File Name | Role |
| :--- | :--- |
| `common_mqh_cpp_PCH.h` | Manages the include tree for the C++ standard environment and MQL5 WinAPI environment. |
| `common_mqh_cpp_define.h` | Absorbs differences in type definitions (`BOOL`, `DWORD`, `__int64`) and string definitions (`_u`) across environments. |
| `common_mqh_cpp_types.h` | Defines basic data types such as the string class `CPP_TYPE_STRING` and the dynamic array `_vector<T>`. |
| `common_mqh_cpp_constants.h` | Defines Win32 kernel constants (e.g., `GENERIC_READ`, `LOCKFILE_EXCLUSIVE_LOCK`). |
| `common_mqh_cpp_enums.h` | Defines system error codes (56 stages) and language numbers. |
| `common_mqh_cpp_method.h` | Defines Win32 API function wrappers and overloaded functions. |
| `common_mqh_cpp_class_error.h` | Retains and manages system errors. |
| `common_mqh_cpp_lock.h` | Manages file exclusive lock control using `_LockFileEx` / `_UnlockFileEx`. |
| `common_mqh_cpp_class_file_base.h` | Base class for file operations, including folder creation and flushing file buffers. |
| `common_mqh_cpp_class_byte8.h` | Executes file I/O dedicated to 8-byte (64-bit) data and resolves paths to AppData. |
| `common_mqh_cpp_class_file_flow.h` | Executes file opening, lock attempts, and launching external logging processes. |
| `common_mqh_cpp_class_log_base.h` | Generates log file formatting, writes data, and executes capacity maintenance. |
| `common_mqh_cpp_class_language.h` | Identifies and manages terminal languages (Japanese, English, others). |
| `common_mqh_cpp_class_pipe.h` | Manages inter-process communication (IPC) pipelines (for future expansion). |
| `common_mqh_cpp_class_process.h` | Prevents duplicate activation using mutexes, and handles generation/parsing of command line arguments. |
| `common_mqh_cpp_secrets.h` | Defines various mutex names and fixed folder paths (dummy placeholder). |

## Installation & Usage

### 1. Placement and Inclusion
Place all files in the same directory as the source code you are using, and write the include statement as follows depending on your environment:
- C++ Environment: `#include "common_mqh_cpp_PCH.h"`
- MQL5 Environment: Change the file extension to `.mqh` and write `#include "common_mqh_cpp_PCH.mqh"`.

### 2. Adapting secrets.h to Production Environment
`common_mqh_cpp_secrets.h` is a dummy file for repository publication. When adapting to an actual production environment, rewrite it with your own local paths and mutex names before executing compilation.

## License & Disclaimer

This project is published under the **Apache License 2.0**.
Loggers and error display processes using this project are also published under the same conditions.

### Disclaimer (AS IS)
This software is provided "AS IS", without warranty of any kind, express or implied.
In no event shall the copyright holder be liable for any direct, indirect, or commercial damages arising from the execution of this library or programs incorporating it. All risks shall be assumed by the user.

---
Copyright (C) 2026 amenomurakumonoturugi. All rights reserved.
