//this file is part of notepad++
//Copyright (C)2003 Don HO <donho@altern.org>
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

/*
2013
Daniele Frisanco 
ottoct@gmail.com

search news on http://dammuozz.com/

this plugin does stuff for tsql programming



*/
#include <tchar.h> 
#include <windows.h>
#include "PluginDefinition.h"
 

#include "menuCmdID.h"
//#include <string.h>
#include <string>
#include <sstream>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <locale>
//#include<list>
#include<stack>


///sql connection
#include <sqltypes.h>
#include <sql.h>
#include <sqlext.h>
////

using namespace std;

 int paren(char* input,int len);

 int paren2(char* input,int len);
size_t is_there_intestation(char * data,int lunghezza);
//private char delimitatori[] =" \t\n\r:=+-*/()[]{};\"\'\0";
//
// The plugin data that Notepad++ needs
//
FuncItem funcItem[nbFunc];

//
// The data of Notepad++ that you can use in your plugin commands
//
NppData nppData;

//
// Initialize your plugin data here
// It will be called while plugin loading   
void pluginInit(HANDLE hModule)
{
}

//
// Here you can do the clean up, save the parameters (if any) for the next session
//
void pluginCleanUp()
{
}

//
// Initialization of your plugin commands
// You should fill your plugins commands here
void commandMenuInit()
{

    //--------------------------------------------//
    //-- STEP 3. CUSTOMIZE YOUR PLUGIN COMMANDS --//
    //--------------------------------------------//
    // with function :
    // setCommand(int index,                      // zero based number to indicate the order of command
    //            TCHAR *commandName,             // the command name that you want to see in plugin menu
    //            PFUNCPLUGINCMD functionPointer, // the symbol of function (function pointer) associated with this command. The body should be defined below. See Step 4.
    //            ShortcutKey *shortcut,          // optional. Define a shortcut to trigger this command
    //            bool check0nInit                // optional. Make this menu item be checked visually
    //            );
    
	setCommand(0, TEXT("procedura SQL"), sql_proc, NULL, false);
	//setCommand(1, TEXT("ESEMPIO per procedura SQL"), esempio_proc_SQL, NULL, false);
	setCommand(1, TEXT("(vuoto)"), NULL, NULL, false); 
 	setCommand(2, TEXT("parentesi bilanciate"), parentesiBilanciate, NULL, false);
	setCommand(3, TEXT("(vuoto)"), NULL, NULL, false); 
	
    setCommand(4, TEXT("About"), about, NULL, false);
//	setCommand(5, TEXT("(vuoto)"), NULL, NULL, false); 
	//setCommand(6, TEXT("(vuoto)"), NULL, NULL, false); 
 //	setCommand(6, TEXT("sql connetti"), con_sql, NULL, false);
//	setCommand(6, TEXT("parentesi+beginend  bilanciate:mettere testo minuscolo e usare strtok"), parentesiBilanciate, NULL, false);
	
 //   setCommand(8, TEXT("About"), about, NULL, false);
	
}

//
// Here you can do the clean up (especially for the shortcut)
//
void commandMenuCleanUp()
{
	// Don't forget to deallocate your shortcut here
}


//
// This function help you to initialize your plugin commands
//
bool setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk, bool check0nInit) 
{
    if (index >= nbFunc)
        return false;

    if (!pFunc)
        return false;

    lstrcpy(funcItem[index]._itemName, cmdName);
    funcItem[index]._pFunc = pFunc;
    funcItem[index]._init2Check = check0nInit;
    funcItem[index]._pShKey = sk;

    return true;
}

//----------------------------------------------//
//-- STEP 4. DEFINE YOUR ASSOCIATED FUNCTIONS --//
//----------------------------------------------//
void hello()
{
    // Open a new document
    ::SendMessage(nppData._nppHandle, NPPM_MENUCOMMAND, 0, IDM_FILE_NEW);

    // Get the current scintilla
    int which = -1;
    ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
    if (which == -1)
        return;
    HWND curScintilla = (which == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;

    // Say hello now :
    // Scintilla control has no Unicode mode, so we use (char *) here
    ::SendMessage(curScintilla, SCI_SETTEXT, 0, (LPARAM)"Hello, Notepad++!");
}

void parentesiBilanciate()
{
	/*verifico che le parentesi siano bilanciate*/

	// Get the current scintilla
    int which = -1;
    ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
    if (which == -1)
        return;
    HWND curScintilla = (which == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;
	//mi leggo il testo
	int len = (int) SendMessage(curScintilla, SCI_GETTEXTLENGTH, 0, 0);
    char *data = new char[len+1];
    SendMessage(curScintilla, SCI_GETTEXT, (WPARAM)(len+1), (LPARAM)data);
	//lancio procedura verifica parentesi
	int res =paren(data,len);  
	//int res =paren2(data,len);  

	//se mi sono interrotto prima della fine allora c'è un mismatch
 	if (res!=len) 
	{
		//mi posiziono al punto giusto
		SendMessage(curScintilla, SCI_SETSEL, (WPARAM)(res), (LPARAM)(res+1));
		TCHAR messaggio [255];
		LRESULT riga=0,colonna=0; 
		//mi faccio dare la posizione esatta
		riga=SendMessage(nppData._nppHandle, NPPM_GETCURRENTLINE, 0, 0);
		colonna=SendMessage(nppData._nppHandle, NPPM_GETCURRENTCOLUMN, 0, 0);
		//mando un messaggio
		_stprintf(messaggio, _T("Controlla il carattere %d\nriga %d, colonna %d"), res,riga+1,colonna);
		::MessageBox(NULL, messaggio,TEXT("Parentesi non bilanciate") , MB_OK);
 
	} 
	 
}


void esempio_proc_SQL()
{
    // Open a new document
     ::SendMessage(nppData._nppHandle, NPPM_MENUCOMMAND, 0, IDM_FILE_NEW);

    // Get the current scintilla
    int which = -1;
    ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
    if (which == -1)
        return;
    HWND curScintilla = (which == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;

    
	
	//testo per esempio proc SQL
 ::SendMessage(curScintilla, SCI_SETTEXT, 0, (LPARAM) "Text\r\n---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\r\n\r\nCREATE procedure fsp_del_prex\r\n(\r\n  @p_vendor_id                       char(10),\r\n  @p_vendor_loc_code                 char(3),\r\n  @p_pmt_rqst_nbr                    char(22),\r\n  @p_pmt_rqst_date                   datetime,\r\n  @p_schld_pmt_nbr                   smallint,\r\n  @p_line_nbr                        smallint\r\n)\r\nAS\r\n\r\ndeclare @ret int\r\nexec @ret = sp_dbs_authenticate IF @ret != 0 RETURN\r\n\r\ndelete  matching_exception\r\n where  vendor_id = @p_vendor_id\r\n   and  vendor_loc_code = @p_vendor_loc_code\r\n   and  pmt_rqst_nbr = @p_pmt_rqst_nbr\r\n   and  pmt_rqst_date = @p_pmt_rqst_date\r\n   and  schld_pmt_nbr = @p_schld_pmt_nbr\r\n   and  line_nbr = @p_line_nbr\r\n\r\n\r\n");
	
	// vari messaggi sul documento
	
	
	
	
	/*	TCHAR messaggio [255];
 int len = (int) SendMessage(curScintilla, SCI_GETLINEENDPOSITION, 0, 0);
_stprintf(messaggio, _T("%d"), len); 
MessageBox(0, messaggio, TEXT("SCI_GETLINEENDPOSITION"), MB_OK);



len = (int) SendMessage(nppData._nppHandle,NPPM_GETCURRENTLINE, 0, 0); 
_stprintf(messaggio, _T("%d"), len); 
MessageBox(0, messaggio, TEXT("nppData._nppHandleNPPM_GETCURRENTLINE"), MB_OK);

len = (int) SendMessage(curScintilla, SCI_GETCURLINE, 0, 0); 
_stprintf(messaggio, _T("%d"), len); 
MessageBox(0, messaggio, TEXT("SCI_GETCURLINE"), MB_OK);

len = (int) SendMessage(curScintilla, SCI_GETLINECOUNT, 0, 0); 
_stprintf(messaggio, _T("%d"), len); 
MessageBox(0, messaggio, TEXT("SCI_GETLINECOUNT"), MB_OK);

len = (int) SendMessage(curScintilla, SCI_TEXTWIDTH, 0, 0);	 
_stprintf(messaggio, _T("%d"), len); 
MessageBox(0, messaggio, TEXT("SCI_TEXTWIDTH"), MB_OK);

len = (int) SendMessage(curScintilla, SCI_LINELENGTH, 0, 0);
_stprintf(messaggio, _T("%d"), len); 
MessageBox(0, messaggio, TEXT("SCI_LINELENGTH"), MB_OK);

len = (int) SendMessage(curScintilla, SCI_GETLINEINDENTATION, 0, 0);
_stprintf(messaggio, _T("%d"), len); 
MessageBox(0, messaggio, TEXT("SCI_GETLINEINDENTATION"), MB_OK);

len = (int) SendMessage(curScintilla, SCI_GETCURRENTPOS, 0, 0);
_stprintf(messaggio, _T("%d"), len); 
MessageBox(0, messaggio, TEXT("SCI_GETCURRENTPOS"), MB_OK);

len = (int) SendMessage(curScintilla, SCI_GETTEXTLENGTH, 0, 0);
_stprintf(messaggio, _T("%d"), len); 
MessageBox(0, messaggio, TEXT("SCI_GETTEXTLENGTH"), MB_OK);
 
*/





/*
    char *data = new char[len+1];
    SendMessage(curScintilla, SCI_GETTEXT, (WPARAM)(len+1), (LPARAM)data);
	 
#define SCI_GETLINEINDENTATION 2127
    SendMessage(curScintilla, SCI_SETTEXT, 0, (LPARAM)"head\n");
    SendMessage(curScintilla, SCI_APPENDTEXT, (WPARAM)(len), (LPARAM)data);
    free(data);
*/
 //   ::SendMessage(curScintilla, SCI_SETTEXT, 0, (LPARAM)"Daniele");
 
	   
}
void sql_proc_head(){
 // Get the current scintilla
    int which = -1;
    ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
    if (which == -1)
        return;
    HWND curScintilla = (which == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;


    int len = (int) SendMessage(curScintilla, SCI_GETTEXTLENGTH, 0, 0);
    char *data = new char[len+1];
    SendMessage(curScintilla, SCI_GETTEXT, (WPARAM)(len+1), (LPARAM)data);
	
    SendMessage(curScintilla, SCI_SETTEXT, 0, (LPARAM)"head\n");
    SendMessage(curScintilla, SCI_APPENDTEXT, (WPARAM)(len), (LPARAM)data);
    free(data);
 



}
void sql_proc_tail(){
// Get the current scintilla
    int which = -1;
    ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
    if (which == -1)
        return;
    HWND curScintilla = (which == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;
//leggo riga  per riga
 
int  len = (int) SendMessage(curScintilla, SCI_LINELENGTH, 0, 0);
   TCHAR messaggio[255];
	  

len = (int) SendMessage(curScintilla, SCI_GETEOLMODE, 0, 0);
//provare StringCchCat 
_stprintf(messaggio, _T("%d"), len); 
MessageBox(0, messaggio, TEXT("SCI_GETEOLMODE"), MB_OK);


 
_stprintf(messaggio, _T("%d"), len); 
MessageBox(0, messaggio, TEXT("SCI_LINELENGTH"), MB_OK); 

SendMessage(curScintilla,SCI_SETEOLMODE,SC_EOL_LF,0);
SendMessage(curScintilla,SCI_CONVERTEOLS,SC_EOL_LF,0); 

len = (int) SendMessage(curScintilla, SCI_GETEOLMODE, 0, 0);
_stprintf(messaggio, _T("%d"), len); 
MessageBox(0, messaggio, TEXT("SCI_GETEOLMODE"), MB_OK);
    char *data = new char[len];
    SendMessage(curScintilla, SCI_GETLINE, (WPARAM)(len), (LPARAM)data);
	 data[4]='\n';

	//riscrivo in fondo la riga
	SendMessage(curScintilla, SCI_APPENDTEXT, (WPARAM)(len), (LPARAM)data);

 


	 
  free(data);





}



void sql_proc(){
	
  
	 
	 
	// Get the current scintilla
    int which = -1;
    ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
    if (which == -1)
        return;
    HWND curScintilla = (which == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;
	//set language to SQL
	::SendMessage(nppData._nppHandle, NPPM_MENUCOMMAND, 0, IDM_LANG_SQL);
	 
	//setto \n come terminatore riga
	SendMessage(curScintilla,SCI_SETEOLMODE,SC_EOL_LF,0);
	SendMessage(curScintilla,SCI_CONVERTEOLS,SC_EOL_LF,0); 
	// copy all the text
    int len = (int) SendMessage(curScintilla, SCI_GETTEXTLENGTH, 0, 0);
    char *data = new char[len+1];
    SendMessage(curScintilla, SCI_GETTEXT, (WPARAM)(len+1), (LPARAM)data);
	//prendo il nuovo iniizio stringa 
	size_t res=  is_there_intestation(data,len);
	//elimino il primo pezzo
	char *body =  new char[len+1-res];  
	strcpy(body,data+res);
	//cerco il noem della procedura
	char * pch= NULL;;
	pch = strstr (data+res,"procedure");//puntatore alla prima occorrenza di "procedure"
	if (pch== NULL)
		{pch = strstr (data+res,"PROCEDURE");}	
 	pch = strtok (pch+9," \t\n(");
	//piazzo il nome nelel 2 stringhe
	char *head =new char[strlen(pch)*2+100];
	char *tail=new char[40+strlen(pch)];
	sprintf(head,"use  \ngo\nif exists (select * from sysobjects where id = object_id('%s') )\ndrop procedure %s \ngo\n\n",pch,pch);

	sprintf(tail,"\n\ngo\ngrant execute on %s to public\ngo",pch);

	//write new text

    SendMessage(curScintilla, SCI_SETTEXT, 0, (LPARAM)head);
    SendMessage(curScintilla, SCI_APPENDTEXT, (WPARAM)(len-res), (LPARAM)body);
	SendMessage(curScintilla, SCI_APPENDTEXT,strlen(tail), (LPARAM)tail);
  
	SendMessage(curScintilla,SCI_GOTOPOS,4,  0);
	
	//setto \r\n come terminatore riga
	SendMessage(curScintilla,SCI_SETEOLMODE,SC_EOL_CRLF,0);
	SendMessage(curScintilla,SCI_CONVERTEOLS,SC_EOL_CRLF,0); 
	//chiedo di scrivere il DB
	MessageBox(0,  TEXT("Click OK and type DB name"),TEXT("In quale DB va compilata?"), MB_OK); 
	//libero la memoria
	free(data);  
	free(head);  
	free(tail);  
	free(body);


}
   


void about(){
/*
	apro un nuovo documento per i crediti e ifo varie
*/
	// Open a new document
     ::SendMessage(nppData._nppHandle, NPPM_MENUCOMMAND, 0, IDM_FILE_NEW);

    // Get the current scintilla
    int which = -1;
    ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
    if (which == -1)
        return;
    HWND curScintilla = (which == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;

    
	
	// scrivo le info
	::SendMessage(curScintilla, SCI_SETTEXT, 0, (LPARAM) "Daniele plugin v0.1 23-04-2013\n\nSono Daniele Frisanco, contattatemi per avere altre info o per suggerire altre funzionalità,\nmandando una email a ottoct@gmail.com .\nPotete trovare altro materiale sul sito http://dammuozz.com/ . \nHo scritto questo plugin partendo dal plugin d'esempio NppPluginTemplate,\nche potete trovare sul sito di Notepad++.\nQuesto plugin  è distribuito con le condizioni di GNU General Public License.\n\nI'm Daniele Frisanco, contact me  for more info or to suggest new functionalities,\nsending an email to ottoct@gmail.com .\nYou can find out more on http://dammuozz.com/ . \ni had written this plugin starting from example plugin NppPluginTemplate,\nyou can find it on Notepad++ website.\nThis plugin is distribute under the terms of the GNU General Public License. ");
}


size_t is_there_intestation(char * data,int lunghezza){
/*
	delete_intestation delete the 2 lines with "TEXT" and "-----"(more-) 
	that sql server returns as result of DBname.dbo.sp_helptext procedure_name

	leggere il testo dopo aver chiamato
	SendMessage(curScintilla,SCI_SETEOLMODE,SC_EOL_LF,0);
	SendMessage(curScintilla,SCI_CONVERTEOLS,SC_EOL_LF,0); 
*/
	 
char* text= "Text\n---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n"; 


/*

 TCHAR messaggio[255];
_stprintf(messaggio, _T("%d"), strncmp(text,data,strlen(text))); 
MessageBox(0, messaggio, TEXT("se0uguali"), MB_OK); 
*/
if(0== strncmp(text,data,strlen(text))){
	return strlen(text);
}
else {return 0;}
 

}
 

bool gemelle(char a ,char c){

if (a=='(' && c==')')
return true;
else if (a=='[' && c==']')
return true;
else if (a=='{' && c=='}')
return true;
else return false;
}


int paren(char* input, int len ){
/*
cancello se le parentesi sono gemelle
*/
 
    stack<char> aperte;
	stack<char> chiuse;
     int i;
    unsigned int ultimaAperta=0 ;
    for (   i = 0;    i< len ; i++ )
    { 
        switch ( input[i] )
        { 
        case'(':
        case'[':
        case'{':
				aperte.push(input[i]);
				ultimaAperta=i;
				break;
		case'}':  
        case']': 
        case')': 
				chiuse.push(input[i]);
				if ((aperte.size()==0)|| !gemelle( aperte.top( ),chiuse.top())){
					//non matchano le parentesi sono sbagliate
					return i;}
				else  { 
					//matchano quindi le cancello
					chiuse.pop() ;
					aperte.pop( );
				}
				break;
		}
    } 
    if (aperte.size() > 0)
         return ultimaAperta;
    else
         return i;
 
}



bool gemelle2(char a ,char c){

	if (a=='(' && c==')')
		return true;
	else if (a=='[' && c==']')
		return true;
	else if (a=='{' && c=='}')
		return true;
	else if (a=='b' && c=='e')
		return true;
	else return false;
}
int paren2(char* input2, int len ){
/*
controllo parentesi e begin end: vafatto con strtok e con tutto minuscolo
*/
	 int i;
	char* input = new char[len];
	strcpy(input,input2); 
	//mettere input a lowerccase
	for (   i = 0;    i< len ; i++ )
	{
		tolower(input[i]);
	}


    stack<char> aperte;
	stack<char> chiuse;
    
    unsigned int ultimaAperta=0 ;
    for (   i = 0;    i< len ; i++ )
    { 
        switch ( input[i] )
        { 
		//APERTURA
		case'b'://è un begin?//+5begin+3end+tot di codice almeno 6
				if((i+5<=len)&&(strncmp(&(input[i]),"begin",5)==0)&&(strchr(" \t\n\r\0;{}",input[i+5])!=NULL)&&((i==0)||((i>0)&&(strchr(" \t\n\r\0;{}",input[i-1])!=NULL)))){
						aperte.push(input[i]);
						ultimaAperta=i;
				}
				break;
        case'(':
        case'[':
        case'{':
				aperte.push(input[i]);
				ultimaAperta=i;
				break;
		
		//CHIUSURA
		case'e'://è un end? si puo fare molti piu controlli sualla dimensione
				if((i+3<=len)&&(strncmp(&(input[i]),"end",3)==0)&&(strchr(" \t\n\r\0;{}",input[i+3])!=NULL)&&((i>5)&&(strchr(" \t\n\r\0;{}",input[i-1])!=NULL))){
					chiuse.push(input[i]);
					if ((aperte.size()==0)|| !gemelle2( aperte.top( ),chiuse.top())){
						//non matchano le parentesi sono sbagliate
						return i;}
					else{ 
						//matchano quindi le cancello
						chiuse.pop() ;
						aperte.pop();
						}
				}
				break;
		case'}':  
        case']': 
        case')': 
				chiuse.push(input[i]);
				if ((aperte.size()==0)|| !gemelle2( aperte.top( ),chiuse.top())){
					//non matchano le parentesi sono sbagliate
					return i;}
				else  { 
					//matchano quindi le cancello
					chiuse.pop() ;
					aperte.pop();
				}
				break;
		}
    } 
    if (aperte.size() > 0)
         return ultimaAperta;
    else
         return i;
 
}







/*
/////////CONNESSIONE al DB


void con_sql()
{TCHAR messaggio[255];
    SQLHANDLE henv;
    SQLRETURN rc;
    SQLHANDLE hconn;
    SQLSMALLINT bufsize=0;
    SQLINTEGER nativeerror=0;
    SQLSMALLINT textlen=0;
    unsigned char connStrOut[256];
    SQLTCHAR sqlstate[32];
    SQLTCHAR message[256];
 
    rc = SQLAllocEnv(&henv);
    if (rc != SQL_SUCCESS)
    {
       // printf("\nSQLAllocEnv call failed.");
		_stprintf(messaggio, _T("\nSQLAllocEnv call failed.")); 
	MessageBox(0, messaggio, TEXT("ciao risultatoquery"), MB_OK);
        return;
    }
 
    rc = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hconn);
    if (rc != SQL_SUCCESS)
    {
        SQLFreeHandle(SQL_HANDLE_ENV, henv);
        //printf("\nSQLAllocHandle call failed.");
		_stprintf(messaggio, _T("\nSQLAllocHandle call failed.")); 
	MessageBox(0, messaggio, TEXT("ciao risultatoquery"), MB_OK);
        return;
    }
 
    rc = SQLDriverConnect(hconn, NULL, (SQLTCHAR*)TEXT("DRIVER={SQL Server};SERVER=DBMS06;DATABASE=PADMtnrs;UID=sa;PWD=password;"), SQL_NTS, NULL, 256, &bufsize, SQL_DRIVER_NOPROMPT);
 
   if(rc== SQL_SUCCESS)MessageBox(0, TEXT("SQL_SUCCESS"), TEXT("ciao SQLDriverConnect"), MB_OK);
    if(rc== SQL_SUCCESS_WITH_INFO)MessageBox(0, TEXT("SQL_SUCCESS_WITH_INFO"), TEXT("ciao SQLDriverConnect"), MB_OK);
	 if(rc== SQL_ERROR)MessageBox(0, TEXT("SQL_ERROR"), TEXT("ciao SQLDriverConnect"), MB_OK);
	  if(rc== SQL_INVALID_HANDLE)MessageBox(0, TEXT("SQL_INVALID_HANDLE"), TEXT("ciao SQLDriverConnect"), MB_OK);
	   if(rc== SQL_NEED_DATA)MessageBox(0, TEXT("SQL_NEED_DATA"), TEXT("ciao SQLDriverConnect"), MB_OK);
	    if(rc== SQL_NO_DATA_FOUND)MessageBox(0, TEXT("SQL_NO_DATA_FOUND"), TEXT("ciao SQLDriverConnect"), MB_OK);
    if (bufsize!=0)
    {
   //     printf("Connected successfully.\n");
		_stprintf(messaggio, _T("Connected successfully.\n")); 
	MessageBox(0, messaggio, TEXT("ciao risultatoquery"), MB_OK);



SQLHSTMT StatementHandle;
 
rc = SQLAllocHandle( SQL_HANDLE_STMT, hconn, &StatementHandle );
 if(rc== SQL_SUCCESS)MessageBox(0, TEXT("SQL_SUCCESS"), TEXT("ciao SQLAllocHandle"), MB_OK);
    if(rc== SQL_SUCCESS_WITH_INFO)MessageBox(0, TEXT("SQL_SUCCESS_WITH_INFO"), TEXT("ciao SQLAllocHandle"), MB_OK);
	 if(rc== SQL_ERROR)MessageBox(0, TEXT("SQL_ERROR"), TEXT("ciao SQLAllocHandle"), MB_OK);
	  if(rc== SQL_INVALID_HANDLE)MessageBox(0, TEXT("SQL_INVALID_HANDLE"), TEXT("ciao SQLAllocHandle"), MB_OK);
	   if(rc== SQL_NEED_DATA)MessageBox(0, TEXT("SQL_NEED_DATA"), TEXT("ciao SQLAllocHandle"), MB_OK);
	    if(rc== SQL_NO_DATA_FOUND)MessageBox(0, TEXT("SQL_NO_DATA_FOUND"), TEXT("ciao SQLAllocHandle"), MB_OK);



SQLVARCHAR rtnFirstName[50];
SQLVARCHAR rtnLastName[50];

SQLVARCHAR rtnFirstNamek[3];
SQLVARCHAR rtnLastNamek[3];
SQLTCHAR   SQLStmt[1024];
strcpy((char *) SQLStmt, "SELECT top 2 [nome], [cognome] FROM [master].dbo.anagrafica_agenti; ");

//quasi rc= SQLPrepare(  StatementHandle,   (SQLTCHAR*)TEXT("SELECT nome,cognome FROM PADMtnrs.dbo.anagrafica_agenti "), strlen( "SELECT nome,cognome FROM PADMtnrs.dbo.anagrafica_agenti "));
//daprovare rc= SQLPrepare(  StatementHandle,   (SQLTCHAR*)TEXT("SELECT nome,cognome FROM PADMtnrs.dbo.anagrafica_agenti "), SQL_NTS);
rc= SQLPrepare(  StatementHandle,   (SQLTCHAR*)TEXT("SELECT 'aaa','bbb' "), strlen("SELECT 'aaa','bbb' "));

   if(rc== SQL_SUCCESS)MessageBox(0, TEXT("SQL_SUCCESS"), TEXT("ciao SQLPrepare"), MB_OK);
    if(rc== SQL_SUCCESS_WITH_INFO)MessageBox(0, TEXT("SQL_SUCCESS_WITH_INFO"), TEXT("ciao SQLPrepare"), MB_OK);
	 if(rc== SQL_ERROR)MessageBox(0, TEXT("SQL_ERROR"), TEXT("ciao SQLPrepare"), MB_OK);
	  if(rc== SQL_INVALID_HANDLE)MessageBox(0, TEXT("SQL_INVALID_HANDLE"), TEXT("ciao SQLPrepare"), MB_OK);
	   if(rc== SQL_NEED_DATA)MessageBox(0, TEXT("SQL_NEED_DATA"), TEXT("ciao SQLPrepare"), MB_OK);
	    if(rc== SQL_NO_DATA_FOUND)MessageBox(0, TEXT("SQL_NO_DATA_FOUND"), TEXT("ciao SQLPrepare"), MB_OK);

   SQLINTEGER signal;                   // (see SQLBindCol documentation below!)
  SQLBindCol(StatementHandle, 1, SQL_C_CHAR, &rtnFirstNamek, 3, &signal);
   SQLBindCol(StatementHandle, 2, SQL_C_CHAR, &rtnLastNamek, 3, &signal);
   
strcpy((char *) SQLStmt, "SELECT 'aaa','bbb'");
 rc=SQLExecDirect(StatementHandle,
                      SQLStmt ,
                       SQL_NTS);
   long jane;

   jane = SQLFetch(StatementHandle);
 
_stprintf(messaggio, _T("%s   %s"),rtnFirstNamek,rtnLastNamek); 
	MessageBox(0, messaggio, TEXT("ciao jane"), MB_OK);
 //rc = SQLExecute(StatementHandle);   quasi


//rc = SQLExecDirect(StatementHandle, SQLStmt, SQL_NTS);
//rc = SQLExecDirect(StatementHandle, SQLStmt, strlen("SELECT top 2 [nome], [cognome] FROM [master].dbo.anagrafica_agenti; "));

   if(rc== SQL_SUCCESS)MessageBox(0, TEXT("SQL_SUCCESS"), TEXT("ciao SQLExecDirect"), MB_OK);
    if(rc== SQL_SUCCESS_WITH_INFO)MessageBox(0, TEXT("SQL_SUCCESS_WITH_INFO"), TEXT("ciao SQLExecDirect"), MB_OK);
	 if(rc== SQL_ERROR)MessageBox(0, TEXT("SQL_ERROR"), TEXT("ciao SQLExecDirect"), MB_OK);
	  if(rc== SQL_INVALID_HANDLE)MessageBox(0, TEXT("SQL_INVALID_HANDLE"), TEXT("ciao SQLExecDirect"), MB_OK);
	   if(rc== SQL_NEED_DATA)MessageBox(0, TEXT("SQL_NEED_DATA"), TEXT("ciao SQLExecDirect"), MB_OK);
	    if(rc== SQL_NO_DATA_FOUND)MessageBox(0, TEXT("SQL_NO_DATA_FOUND"), TEXT("ciao SQLExecDirect"), MB_OK);
 
_stprintf(messaggio, _T("rc=%d"),rc); 
	MessageBox(0, messaggio, TEXT("ciao risultatoquery"), MB_OK);
	SQLBindCol(StatementHandle, 1, SQL_C_CHAR, &rtnFirstName, sizeof(rtnFirstName), NULL );
			SQLBindCol(StatementHandle, 2, SQL_C_CHAR, &rtnLastName, sizeof(rtnLastName), NULL );
		 
			for(int k=0;k<2;k++) 
			{
				rc  = SQLFetch(StatementHandle);
				if (rc  == SQL_NO_DATA_FOUND) break;
	 
	_stprintf(messaggio, _T("%s,%s"), rtnLastName,rtnFirstName); 
	MessageBox(0, messaggio, TEXT("ciao risultatoquery"), MB_OK);
					}

			SQLFreeStmt(StatementHandle, SQL_CLOSE);

		



        SQLDisconnect(hconn);
    }
    else
    {
        rc = SQLGetDiagRec(SQL_HANDLE_DBC, hconn, 1, sqlstate, &nativeerror, message, 256, &textlen);
 
        printf("SQLDriverConnect failed.\n");
        if (rc!=SQL_ERROR)
            
	
	//provare StringCchCat 
	_stprintf(messaggio, _T("%s=%s\n"), (CHAR *)sqlstate, (CHAR *)message); 
	MessageBox(0, messaggio, TEXT("ciao risultatoquery"), MB_OK);
			
			//printf("%s=%s\n", (CHAR *)sqlstate, (CHAR *)message);
    }
 
    SQLFreeHandle(SQL_HANDLE_DBC, hconn);
    SQLFreeHandle(SQL_HANDLE_ENV, henv);
}





















const int MAX_CHAR = 1024;

///////////////////////
void con_sql1(){
SQLTCHAR   SQLStmt[MAX_CHAR];
char      strSQL[MAX_CHAR];
char	  chrTemp;

SQLVARCHAR rtnFirstName[50];
SQLVARCHAR rtnLastName[50];
SQLVARCHAR rtnAddress[30];
SQLVARCHAR rtnCity[30];
SQLVARCHAR rtnState[3];
SQLDOUBLE  rtnSalary;
SQLVARCHAR rtnGender[2];
SQLINTEGER rtnAge;

// Get a handle to the database

SQLHENV EnvironmentHandle;
RETCODE retcode = SQLAllocHandle( SQL_HANDLE_ENV, SQL_NULL_HANDLE, &EnvironmentHandle );

// Set the SQL environment flags

retcode = SQLSetEnvAttr( EnvironmentHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC3, SQL_IS_INTEGER );

// create handle to the SQL database

SQLHDBC ConnHandle;
retcode = SQLAllocHandle( SQL_HANDLE_DBC, EnvironmentHandle, &ConnHandle );

// Open the database using a System DSN
SQLHSTMT StatementHandle;
retcode = SQLDriverConnect(ConnHandle, 
	NULL, 
	(SQLTCHAR*)"DSN=DBMS06;UID=sa;PWD=password;", 
	SQL_NTS,
	NULL, 
	SQL_NTS, 
	NULL, 
	SQL_DRIVER_NOPROMPT);
if (!retcode) 
{
	cout << "SQLConnect() Failed";
}
else
{
	// create a SQL Statement variable

	
	retcode = SQLAllocHandle(SQL_HANDLE_STMT, ConnHandle, &StatementHandle);

	
	
			strcpy((char *) SQLStmt, "SELECT top 2 [nome], [cognome] FROM PADMtnrs.dbo.anagrafica_agenti ");
	
			retcode = SQLExecDirect(StatementHandle, SQLStmt, SQL_NTS);

			//SQLGetDiagRec(SQL_HANDLE_STMT, StatementHandle, RecNumber, SQLState, NativeErrorPtr, (SQLCHAR*) MessageText, (SQLINTEGER) BufferLength, (SQLSMALLINT*) &TextLengthPtr);

			SQLBindCol(StatementHandle, 1, SQL_C_CHAR, &rtnFirstName, sizeof(rtnFirstName), NULL );
			SQLBindCol(StatementHandle, 2, SQL_C_CHAR, &rtnLastName, sizeof(rtnLastName), NULL );
		//	SQLBindCol(StatementHandle, 3, SQL_C_CHAR, &rtnAddress, sizeof(rtnAddress), NULL );
		//	SQLBindCol(StatementHandle, 4, SQL_C_CHAR, &rtnCity, sizeof(rtnCity), NULL );
		//	SQLBindCol(StatementHandle, 5, SQL_C_CHAR, &rtnState, sizeof(rtnState), NULL );
		//	SQLBindCol(StatementHandle, 6, SQL_C_DOUBLE, &rtnSalary, sizeof(rtnSalary), NULL );
		//	SQLBindCol(StatementHandle, 7, SQL_C_CHAR, &rtnGender, sizeof(rtnGender), NULL );
		//	SQLBindCol(StatementHandle, 8, SQL_C_LONG, &rtnAge, sizeof(rtnAge), NULL );

			for(;;) 
			{
				retcode = SQLFetch(StatementHandle);
				if (retcode == SQL_NO_DATA_FOUND) break;
	TCHAR messaggio[255];
	//provare StringCchCat 
	_stprintf(messaggio, _T("%s,%s"), rtnLastName,rtnFirstName); 
	MessageBox(0, messaggio, TEXT("ciao risultatoquery"), MB_OK);
				//cout << rtnFirstName << " " << rtnLastName << " " << rtnAddress << " " << rtnCity << " " << rtnState << " " << rtnSalary << " " << rtnGender << " " << rtnAge << endl;
			}

			SQLFreeStmt(StatementHandle, SQL_CLOSE);

		}
	

	SQLFreeStmt(StatementHandle, SQL_CLOSE );
	SQLFreeHandle(SQL_HANDLE_STMT, StatementHandle);

	SQLDisconnect(ConnHandle);

	SQLFreeHandle(SQL_HANDLE_DBC, ConnHandle);
	SQLFreeHandle(SQL_HANDLE_ENV, EnvironmentHandle);

	printf( "Done.\n" );


return ;

}

*/