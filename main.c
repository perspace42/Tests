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

int main(){
  duckdb_database db;
  duckdb_connection con;
  const char * dbPath = dbPath(test.db);
  char * outerror = NULL;

  //Attempt to Open Database Output Any Errors
  if (duckdb_open_ext(dbPath, &db, NULL, &outerror) == DuckDBError){
    puts("Database Open Error:");
    if (outerror){
      puts(outerror);
      duckdb_free(outerror);
    }else{
      puts("No Error Message :(");
    }
  //If Successfull
  }else{
    puts("Database Opened Succssfully");
  }

  //Attempt to Connect to Database Output Any Errors
  if (duckdb_connect(db, &con) == DuckDBError) {
    puts("Database Connection Error:");
    if (outerror){
      puts(outerror);
      duckdb_free(outerror);
    }else{
      puts("No Error Message :(");
    }
  //If Successfull
  }else{
    puts("Database Connection Established Successfully");
  }

  //cleanup
  duckdb_disconnect(&con);
  duckdb_close(&db);
}
