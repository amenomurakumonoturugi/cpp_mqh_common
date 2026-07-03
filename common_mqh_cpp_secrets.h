// ==============================================================================
//     【代替用ダミーファイル / TEMPLATE & DUMMY FILE】
// ==============================================================================
// [JP] このファイルはGitHub公開用のリポジトリ代替（ダミー）ファイルです。
//      実際の環境パスやミューテックス名などはダミー文字列に置換されています。
//      本番環境で使用する際は、自身の環境に合わせた正しい値に書き換えてください。
//
// [EN] This is a dummy template file for public GitHub repository.
//      Sensitive information (paths, mutex names, etc.) has been replaced with dummies.
//      Please configure proper values according to your local environment.
// ==============================================================================

#ifndef COMMON_SECRETS_H

#define COMMON_SECRETS_H

#define SYSTEM_VERSION 1.000



string MUTEX_LOCAL_NAME_SYSTEM_LOG_PROCESS() { return _u("Local\\System_Logger_M"); }

string MUTEX_NAME_SERVICE_LOG_PROCESS() { return _u("Local\\Service_Logger_M"); }

string MUTEX_LOCAL_NAME_ERROR_PROCESS() { return _u("Local\\Error_M"); }

string MAKER_FOLDER_DRC() { return _u("\\maker_Name"); }

string ITEM_FOLDER_DRC() { return _u("\\item_name"); }

string EXE_FOLDER_DRC() { return _u("C:\\Program Files"); }

string ITEM_ERR_MS_ERR_FILE_NAME() { return _u("\\Error_Message.bin"); }

string ITEM_ERR_MS_FOLDER_DRC() { return _u("\\Error_Message"); }

string ITEM_ERR_MS_FILES_FOLDER_DRC() { return _u("\\FILE"); }

string ITEM_ERR_MS_ERR_FILE_DRC() { return _u("\\Error_Message"); }

string ITEM_ERR_MS_VISIBLE_ERR_NUM_FILE_NAME() { return _u("\\Visible_Error.bin"); }

string ITEM_ERR_MS_VISIBLE_FILE_DRC() { return _u("\\Visible_Error"); }

string ITEM_LANGUAGE_FILE_NAME() { return _u("\\Language.bin"); }

string ITEM_LANGUAGE_FOLDER_DRC() { return _u("\\Language"); }

string ITEM_ERR_MS_EXE_NAME() { return _u("\\Error_Message.exe"); }

string ITEM_LOG_MNG_EXE_NAME() { return _u("\\Logger.exe"); }

string ITEM_LOG_FOLDER_DRC() { return _u("\\log"); }

string TERMINAL_LANGUAGE_JP() { return _u("\\Japanese"); }

string TERMINAL_LANGUAGE_EN() { return _u("\\English"); }

string ERR_MS_WINDOW_TITLE() { return _u("error"); }

string ERR_MS_WINDOW_CLASS() { return _u("class_error"); }

string ITEM_SYSTEM_LOG_FOLDER_DRC() { return _u("SYSTEM"); }

string ITEM_SYSTEM_CURRENT_LOG_FOLDER_DRC() { return _u("\\CURRENT"); }

string ITEM_SYSTEM_DAY_LOG_FOLDER_DRC() { return _u("\\DAY_LOG"); }

#endif