/*
Author: Scott Field
Date 06/14/2025
Version 1.0
Purpose:
Test the DuckDB functions
*/

#include "libraries.h"
#include "stdio.h"

#define dbPath(NAME) "/home/sfcyberbox/Desktop/Software_Projects/C/DuckDB_TEST/" #NAME

void handleError(const char * msg, char * outError);

int main(){
  duckdb_database db;
  duckdb_connection con;
  const char * dbPath = dbPath(test.db);
  char * outError = NULL;

  //Attempt to Open Database Output Any Errors
  if (duckdb_open_ext(dbPath, &db, NULL, &outError) == DuckDBError){
    //If an error occurs handle it
    handleError("Database Open Error:",outError);
  }else{
    puts("Database Opened Succssfully");
  }

  //Attempt to Connect to Database Output Any Errors
  if (duckdb_connect(db, &con) == DuckDBError) {
    //If an error occurs handle it
    handleError("Database Connection Error",outError);
  }else{
    puts("Database Connection Established Successfully");
  }

  //cleanup
  duckdb_disconnect(&con);
  duckdb_close(&db);
}

//Output Any Error
void handleError(const char * msg, char * outError){
  puts(msg);void handleError(char * msg, char * outerror);
  if (outError){
    puts(outError);
    duckdb_free(outError);
  }else{
    puts("No Error Information");
  }
}
